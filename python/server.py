from socket import *

blockSize = 4096
filename = ""

def sock_init():
    s = socket()
    s.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
    s.bind(("0.0.0.0", 8890))
    s.listen(5)
    s,addr=s.accept()
    return s

s = sock_init()
filesize = int(s.recv(20), 16)
total = filesize // blockSize
residual = filesize % blockSize

res = b""
for x in range(total):
    res += s.recv(4096)
res += (s.recv(residual))

s.close()

with open("TEST.TEST.bin.cp", "wb") as f:
    f.write(res)

