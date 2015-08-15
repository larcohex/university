import socket, os, sys, select, time, shutil
from threading import Thread,Lock
from Compile import Compile
from gdb import Debugger

HOST = '127.0.0.1'
PORT = 5000

class Server:

	def __init__(self):
		self.processes = {}
		self.ids = {}
		self.lock = Lock()
		self.sock = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
		self.sock.setsockopt (socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		self.sock.bind ((HOST, PORT))
		self.sock.listen (1)


	def _get_count (self,socket):
		buf = ''
		while True:
			c = socket.recv (1)
			if not c:
				# error or just end of connection/
				if buf:
					raise RuntimeError("underflow")
				else:
					return -1
			if c == '|':
				return int(buf)
			else:
				buf += c

	def _removeFile (self, filename):
		try:
			os.remove (filename)
		except:
			pass

	def start (self):
		while True:
			connection, addr = self.sock.accept()
			print 'FROM '+str(addr) + ' - Time: ' + time.strftime ('%H:%M:%S - %d/%m/%Y',time.localtime(time.time()))
			t = Thread (target = self.communicate, args = (connection,))
			t.daemon = True
			t.start()

		self.finish()


	def communicate (self, connection):
		
		size = int(self._get_count (connection))
		if size != -1:
			command = connection.recv (size)
		else:
			command = "wait"
		while command:
			if command != "wait":
				response = "REQUEST_BAD"
				ctemp = command
				command = command.split ('|')
				if command[1] == 'authentification':
					if command[0] in self.ids:
						response = "REQUEST_OK"
					if command[0] in os.listdir(os.path.curdir):
						response = "REQUEST_OK"
						self.ids[command[0]] = []
					else:
						response = "REQUEST_BAD"
				if command[1] == 'registration':
					if command[0] not in self.ids:
						self.ids[command[0]] = []
						directory = self.createDirectory(command[0])					
						if directory:
							response = "REQUEST_OK"
				if command[1] == 'showList':
					listOffiles = os.listdir(command[0])
					if listOffiles != []:
						ltemp = []
						for l in listOffiles:
							if l.endswith('.c') or l.endswith('.cpp'):
								ltemp += [l]
						listOffiles = ltemp					
						listOffiles = ('|').join(listOffiles)
						response = listOffiles
					else:
						response = 'EMPTY'
				print command
				if command[1] == 'open':
					ftemp = open(command[2],'r')
					response = ftemp.read()
					ftemp.close()
					response += "|"
					try:
						finp = open((command[2].split('.')[0]+'.inp'),'r')
						response += finp.read()
						finp.close()
					except IOError:
						pass
				command = ctemp
				command = command.split('|',2) 
				if command[1] == "compile":
					owner = command[0]
					com = command[1]
					command = command[2].split ("|")
					language = command[0]
					flags = command[1]
					code = command[2]
					inp = command[3]
					if owner in self.ids:
						self.lock.acquire()
						if self.ids[owner]:
							self.ids[owner].append (self.ids[owner][-1] + 1)
						else:
							self.ids[owner].append (1)
						if language == "c":
							newName = owner + "/" + str (self.ids[owner][-1]) + ".c"
						else:
							newName = owner + "/" + str (self.ids[owner][-1]) + ".cpp"
						if inp:
							inpName = owner + "/" + str (self.ids[owner][-1]) + ".inp"
							newFile = open (inpName, "w+")
							newFile.write (inp)
							newFile.close()
						else:
							inpName = ""
						newFile = open (newName, "w+")
						newFile.write (code)
						newFile.close()
						compiler = Compile()
						response = '\n'.join (compiler.compile (newName + " " + flags))
						self.lock.release()
					else:
						response = "User does not exist"
				elif command[1] == "run":
					owner = command[0]
					com = command[1]
					command = command[2].split ("|")
					language = command[0]
					flags = command[1]
					code = command[2]
					inp = command[3]
					if owner in self.ids:
						self.lock.acquire()
						if language == "c":
							newName = owner + "/" + str (self.ids[owner][-1]) + ".c"
						else:
							newName = owner + "/" + str (self.ids[owner][-1]) + ".cpp"
						if inp:
							inpName = owner + "/" + str (self.ids[owner][-1]) + ".inp"
							newFile = open (inpName, "w+")
							newFile.write (inp)
							newFile.close()
						else:
							inpName = ""
						newFile = open (newName, "w+")
						newFile.write (code)
						newFile.close()
						compiler = Compile()
						response = '\n'.join (compiler.run (newName + " " + flags, inpName))
						self.lock.release()
					else:
						response = "User does not exist"
				elif command[1] == "recompile":
					owner = command[0]
					com = command[1]
					command = command[2].split ("|")
					language = command[0]
					flags = command[1]
					code = command[2]
					inp = command[3]
					self.lock.acquire()
					self._removeFile (owner + "/" + str (self.ids[owner][-1]) + ".c")
					self._removeFile (owner + "/" + str (self.ids[owner][-1]) + ".cpp")
					self._removeFile (owner + "/" + str (self.ids[owner][-1]))
					self._removeFile (owner + "/" + str (self.ids[owner][-1]) + ".inp")
					if language == "c":
						newName = owner + "/" + str (self.ids[owner][-1]) + ".c"
					else:
						newName = owner + "/" + str (self.ids[owner][-1]) + ".cpp"
					if inp:
						inpName = owner + "/" + str (self.ids[owner][-1]) + ".inp"
						newFile = open (inpName, "w+")
						newFile.write (inp)
						newFile.close()
					else:
						inpName = ""
					newFile = open (newName, "w+")
					newFile.write (code)
					newFile.close()
					compiler = Compile()
					response = '\n'.join (compiler.compile (newName + " " + flags))
					self.lock.release()
				elif command[1] == "last":
					owner = command[0]
					filename = str (self.ids[owner][-1])
					try:
						with open ("./" + owner + "/" + filename + ".c", 'r') as newFile:
							response = newFile.read()
					except IOError:
						try:
							with open ("./" + owner + "/" + filename + ".cpp", 'r') as newFile:
								response = newFile.read()
						except IOError:
							response = "No files"

				elif command[1] == "debug":
					owner = command[0]
					com = command[1]
					commands = command[2].split ("|")
					exe = str (self.ids[owner][-1])
					self.lock.acquire()
					response = Debugger().run (owner + "/" + exe, commands)
					self.lock.release()
			
				print 'SEND\n', response, '\n'
				length = str (len (response))
				response = length + "|" + response
				connection.send (response)


			size = self._get_count (connection)
			if size != -1:
				command = connection.recv (size)
			else:
				command = "wait"

		connection.close()
		
	def createDirectory(self,clientID):
		response = False
		if not os.path.exists(clientID):
			os.mkdir(clientID)
			response = True
		return response	

	def finish (self):
		print '\nKilled with CTRL+C\nCurrent users:\n'
		for proc in self.ids:
			print proc
			shutil.rmtree (proc)

		self.sock.close()
		print 'bye :(\n'


if __name__ == '__main__':
	try:
		s = Server()
		s.start()
	except KeyboardInterrupt:
		s.finish()
