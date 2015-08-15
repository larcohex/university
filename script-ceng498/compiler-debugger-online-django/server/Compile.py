from subprocess import Popen,PIPE
import re,sys,shlex,pty,os,time
from Execute import Execute

class Compile(object):
	
	def __init__(self):
		self.command = ''
	
	def compile (self, flag):
		self.executableFileName = ''
		self.command = self.generateCommand(flag) # the compiler will be selected according to supplied source file
		if self.command != 'Command not found!':
			self.compile = Popen(self.command,stdin=PIPE,stdout=PIPE,stderr=PIPE)
			out, err = self.compile.communicate()
			#  Parsing errors while compiling source file. Start
			errorList = []
			err = err.split(':')
			counter = 0
			errorType = "[Error*] -> {Position: %s/%s} {Explanation: %s }"
			warningType =  "[Warning*] -> {Position: %s/%s} {Explanation: %s}"
			notFileType = "[Error*] -> {Explanation: No such file or directory exists }"
			flag = False
			for ex in err:
				ex = ex.lstrip(' ')
				print ex
				if counter == 0 : 
					for ex2 in err:
						if re.search ('No such file or directory', ex2):
							errorList = errorList + [notFileType]
							flag = True
							break
					if flag:
						return errorList
				elif ex.__eq__('error'):
					errorList = errorList + [(errorType % (err[counter-2],err[counter-1],re.search(r'[^[\n]*',err[counter+1]).group()))]

				elif ex.__eq__('warning'):
					errorList = errorList + [(warningType % (err[counter-2],err[counter-1],re.search(r'[^[\n]*',err[counter+1]).group()))]
				counter += 1
			print errorList
			if errorList == []:
				errorList = ["Successfully compiled"]
			return errorList
	
			#	End of Part
		else:
			return ['Command not found!']


	def run (self, flag, inputFile = ''):
		self.executableFileName = ''
		self.command = self.generateCommand(flag) # the compiler will be selected according to supplied source file
		if self.command != 'Command not found!':
			exe = './' + self.executableFileName				
			if inputFile:
				exe = exe + '< ' + inputFile
			e = Execute().run(exe)
			return [e]	
	
			#	End of Part
		else:
			return ['Command not found!']			
	
	def generateCommand(self,command):
		command = shlex.split(command)
		for ex in command:
			if re.search(r'^.*\.c$',ex):
				command = ['gcc']+command+['-o']+[ex.split('.')[0]]+['-g']
				self.executableFileName = ex.split('.')[0]
				return command
			elif re.search(r'.*\.c.*',ex):
				command = ['g++']+command+['-o']+[ex.split('.')[0]]+['-g']
				self.executableFileName = ex.split('.')[0]
				return command
			else:
				return 'Command not found!'			
			
