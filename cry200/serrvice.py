#!/usr/bin/env python

import random
import md5
import random
import string
import os
import sys
secret = ''.join(random.choice(string.ascii_uppercase) for _ in range(5))
command = "cal"
sys.stdout.write("Welcome to calendar. Enter command and signature\n")
sys.stdout.write("Available command\n%s\n%s\n" % (command,md5.new(secret+command).hexdigest()))
sys.stdout.flush()
message = raw_input()
digest = raw_input()
if md5.new(secret+message).hexdigest() == digest:
	os.system(message.replace('\x00',""))
else:
	print "Invalid signature :("



