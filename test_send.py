# send - host / port
import json
import socket

HOST = "127.0.0.1"
PORT = 10000

def send_json(host, port, jsdata):
    raw = json.dumps(jsdata).encode('utf-8')
    l = len(raw).to_bytes(4, 'big')
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((host, port))

    s.send(l)
    s.send(raw)
    
    while(True):
        s.recv(4)
    s.close()

DATA = {
   "data": "Hello!!" * 1000,
   "flags": 0x03,
}

send_json(HOST, PORT, DATA)