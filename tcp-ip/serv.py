#!/usr/bin/python

#server

import socket

address = ('127.0.0.0', 31500)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(address)
s.listen(5)

ss, addr = s.accept()
print("Get connect from: ", addr)
ss.send("Byebye")
ra = ss.recv(512)
print(ra)

ss.close()
s.close()

