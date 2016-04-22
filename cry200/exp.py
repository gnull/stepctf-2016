#!/usr/bin/env python

from socket import *
import subprocess
def recvtil(s,c):
	pp = s.recv(1)
	out=[]
	while pp != c:
		out.append(pp)
		pp = s.recv(1)
	return "".join(out)

s = socket(AF_INET,SOCK_STREAM)
s.connect(('spbctf.ppctf.net',7777))
print recvtil(s,"\n")
print recvtil(s,"\n")
mes = recvtil(s,"\n")
digest = recvtil(s,"\n")
cmd = "../../../../tools/hash_extender/hash_extender --data \"%s\" --secret 5 --append \";cat flag\"  --signature=%s --format md5" %(mes,digest)

process = subprocess.Popen(cmd, shell=True,
                           stdout=subprocess.PIPE, 
                           stderr=subprocess.PIPE)
out, err = process.communicate()
import re
nsig =  re.findall(r"New signature: ([0123456789abcdef]+)",out)[0]
nmes = re.findall(r"New string: ([0123456789abcdef]+)",out)[0]
s.send(nmes.decode("hex")+"\n")
s.send(nsig+"\n")

print s.recv(1024)
