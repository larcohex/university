from subprocess import Popen,PIPE
import re

class Debugger(object):
		
	def run(self,filename,command):
		execute  = ['gdb']+[filename]
		self.process = Popen(execute,stdin=PIPE,stdout=PIPE,stderr=PIPE)
		
		command = ['set confirm off\n']+command
		for ex in command:
			self.process.stdin.write( ex+'\n')
			self.process.stdin.flush()
		self.process.stdin.close()
		self.out = ''.join(self.process.stdout.readlines())
		self.out = self.out.split('(gdb)')
		temp = []
		for o in self.out:
			if o != '' and o != ' ':
				temp += [o] 
		self.out = temp		
		self.out = self.out[-2]
		self.out = self.out.split('\n')
		self.out = self.content(self.out)			
		return ('\n'.join(self.out)).lstrip()
		
	
	def content(self,out):
		i = 0
		for o in out:
			match = re.search(r'Starting program:|tcsetpgrp',o)
			if match:
				return out[i+1:]
			i+=1
		return out	
		
