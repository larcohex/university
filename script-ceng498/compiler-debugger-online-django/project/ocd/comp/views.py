from django.shortcuts import render, render_to_response
from django.http import HttpResponse, HttpResponseRedirect
from django.views.generic import View
from django.template import RequestContext, loader
from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.models import User



import socket, os, sys
from uuid import uuid4


HOST = '127.0.0.1'
PORT = 5000


class Client:

	def __init__ (self, owner = None):
		if owner is None:
			raise ValueError
		self.owner = owner
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.sock.connect((HOST, PORT))

	def _get_count (self,socket):
		buf = ""
		while True:
			c = socket.recv (1)
			if not c:
				# error or just end of connection/
				if buf:
					raise RuntimeError("underflow")
				else:
					return -1
			if c == "|":
				return int(buf)
			else:
				buf += c
	
	def communicate (self, command, inputFile = ''):

		length = str (len (self.owner + "|" + command))
		message = length + "|" + self.owner + "|" + command
		self.sock.send (message)
		size = self._get_count (self.sock)
		if(size!=-1):
			response = self.sock.recv (size)
		else:
			return 'No response'
		return response


	def end (self):
		bye = self.communicate ("exit")
		self.sock.close()
		return bye



def user_exists (username):
    if User.objects.filter (username = username).count():
        return True
    return False


def index (request):
	if request.user.is_authenticated():
		return comp_files (request)
	else:
		return comp_login (request)

def comp_login (request):
	username = password = ""
	if request.POST:
		if "login" in request.POST:
			username = request.POST.get ("username")
			password = request.POST.get ("password")
			if not username:
				return render (request, "login.html", {"username": username, "no_login_error": True})
			user = authenticate (username = username, password = password)
			if user:
				if user.is_active:
					login (request, user)
					return HttpResponseRedirect ("/comp_files/")
			else:
				return render (request, "login.html", {"username": username, "login_error": True})
		elif "signup" in request.POST:
			temp = request.POST._mutable
			request.POST = request.POST.copy()
			del request.POST ["signup"]
			request.POST._mutable = temp
			return HttpResponseRedirect ("/comp_signup/")
	return render (request, "login.html", {"username": username})


def comp_signup (request):
	username = password = ""
	if request.POST:
		if "create" in request.POST:
			username = request.POST.get ("username")
			if not username:
				return render (request, "signup.html", {"username": "", "empty_username_error" : True})
			password = request.POST.get ("password")
			confirm_password = request.POST.get ("confirm-password")
			if not password:
				return render (request, "signup.html", {"username": username, "empty_password_error": True})
			if password == confirm_password:
				if (user_exists (username)):
					return render (request, "signup.html", {"username": username, "user_exists_error": True})
				else:
					new_user = User.objects.create_user (username = username, password = password)
					new_user.save()
					new_user.backend = "django.contrib.auth.backends.ModelBackend"
					login (request, new_user)
					c = Client (str (username))
					command = "registration|" + str (username)
					c.communicate (command)
					return HttpResponseRedirect ("/")
			else:
				return render (request, "signup.html", {"username": username, "password_error": True})


		elif "back" in request.POST:
			return comp_login (request)

	return render (request, "signup.html", {"username": username})

def comp_logout (request):
	if request.POST:
		logout (request)
		request.session.clear()
		return HttpResponseRedirect ("/")


def comp_files (request):
	if request.POST:
		if "new" in request.POST:
			return HttpResponseRedirect ("/comp_new/")
	c = Client (request.user.username)
	command = "showList|" + str (request.user.username)
	response = c.communicate(command)
	if response == "EMPTY":
		return render (request, "files.html")
	else:
		response = response.split('|')
		return render (request, "files.html", {"files": response})



def comp_new (request):
	if request.POST:
		if "compile" in request.POST:
			language = request.POST.get ("language")
			code = request.POST.get ("code")
			inp = request.POST.get ("inp")
			flags = request.POST.get ("flags")
			if not language:
				return render (request, "compile.html", {"code": code, "inp": inp, "flags": flags, "no_language_error": True})
			if not code:
				return render (request, "compile.html", {"no_code_error": True})
			c = Client (request.user.username)
			command = "compile|" + language + "|" + flags + "|" + code + "|" + inp
			response = c.communicate (command)
			request.session["language"] = language
			request.session["code"] = code
			request.session["inp"] = inp
			request.session["flags"] = flags
			request.session["response"] = response
			return HttpResponseRedirect ("/comp_old/")
	return render (request, "compile.html")


def comp_old (request):
	if request.POST:
		if "recompile" in request.POST:
			language = request.POST.get ("language")
			code = request.POST.get ("code")
			inp = request.POST.get ("inp")
			flags = request.POST.get ("flags")
			c = Client (request.user.username)
			command = "recompile|" + language + "|" + flags + "|" + code + "|" + inp
			response = c.communicate (command)
			request.session["language"] = language
			request.session["code"] = code
			request.session["inp"] = inp
			request.session["flags"] = flags
			request.session["response"] = response
			if response == "Successfully compiled":
				return render (request, "recompile.html", {"language": language, "code": code, "inp": inp, "flags": flags, "response": response, "run_access": True})
			else:
				return render (request, "recompile.html", {"language": language, "code": code, "inp": inp, "flags": flags, "response": response})
		elif "finish" in request.POST:
			if "debug_commands" in request.session:
				del request.session["debug_commands"]
		elif "run" in request.POST:
			language = request.POST.get ("language")
			code = request.POST.get ("code")
			inp = request.POST.get ("inp")
			flags = request.POST.get ("flags")
			c = Client (request.user.username)
			command = "run|" + language + "|" + flags + "|" + code + "|" + inp
			response = c.communicate (command)
			request.session["language"] = language
			request.session["code"] = code
			request.session["inp"] = inp
			request.session["flags"] = flags
			request.session["response"] = response
			return render (request, "recompile.html", {"language": language, "code": code, "inp": inp, "flags": flags, "response": response, "run_access": True, "debug_access": True})
		else:
			for i in request.POST:
				if i.endswith (".c") or i.endswith (".cpp"):
					c = Client (request.user.username)
					command = "open|"+request.user.username + "/" + i
					response = c.communicate(command)
					response = response.split ("|")
					code = response[0]
					inp = response[1]
					if i.endswith (".c"):
						language = "c"
					else:
						language = "c++"
					flags = response = ""
					return render (request, "recompile.html", {"language": language, "code": code, "inp": inp, "flags": flags, "response": response})
	language = request.session["language"]
	code = request.session["code"]
	inp = request.session["inp"]
	flags = request.session["flags"]
	response = request.session["response"]
	return render (request, "recompile.html", {"language": language, "code": code, "inp": inp, "flags": flags, "response": response})


def comp_debug (request):
	if request.POST:
		if "do" in request.POST:
			command = request.POST.get ("command")
			if command:
				if "debug_commands" not in request.session:
					new_commands = []
				else:
					new_commands = request.session["debug_commands"]
				command = request.POST.get ("command")
				new_commands.append (command)
				request.session["debug_commands"] = new_commands
				message = "debug"
				for i in request.session["debug_commands"]:
					message += "|" + i
				c = Client (request.user.username)
				response = c.communicate (message)
				code = request.session["code"]
				inp = request.session["inp"]
				return render (request, "debug.html", {"code": code, "inp": inp, "response": response})
			else:
				code = request.session["code"]
				inp = request.session["inp"]
				return render (request, "debug.html", {"code": code, "inp": inp, "no_command_error": True})
		
			
	code = request.session["code"]
	inp = request.session["inp"]
	return render (request, "debug.html", {"code": code, "inp": inp})