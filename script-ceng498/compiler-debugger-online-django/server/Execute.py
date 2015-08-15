from subprocess import Popen,PIPE
import re

class Execute(object):
	
	def run(self,command):
		command  = "./timeout -m 102400 -t 180 " + command
		self.p = Popen(command,stdin=PIPE,stdout=PIPE,stderr=PIPE,shell=True)
		pout,perr = self.p.communicate()			
		if not re.search('^FINISHED CPU ([0-9])*.([0-9]){1,2} MEM ([0-9])* MAXMEM ([-][0-9]+)* STALE ([0-9])*$',perr):
			return perr		
		return pout
	
