from socket import *
from binascii import a2b_hex as a2b
from base64 import b64encode, b64decode
from requests import get as rget
from hexdump import hexdump
from os import urandom

END_EOF = b"SN\x00\x00\x11"

def fwrite(data :bytes, filename):
    with open(filename, "wb") as f:
        f.write(data)

def fread(filename):
    with open(filename, "rb") as f:
        data = f.read()
    return data

def server_handler(server_host, server_port):
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)
    server_socket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
    server_socket.bind((server_host, server_port))
    server_socket.listen(5)
    server_socket, client_addr = server_socket.accept()
    
    res = buff = b""
    while True:
        buff = server_socket.recv(4096)
        if not buff or buff == END_EOF:
            break
        res += buff
    
    server_socket.close()
    return res

def client_handler(server_host, server_port):
    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)
    client_socket.connect((server_host, server_port))
    client_socket.sendall(b"hello, wolrd.\n")
    client_socket.sendall(END_EOF)
    client_socket.close()

def create_bytes_file(filename, size):
    data = urandom(size - 8)
    data = b"\x77SN\x00" + data + b"NS\x00\x00"
    fwrite(data, filename)
    del(data)

