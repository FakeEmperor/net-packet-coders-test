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
        try:
            msg_part = s.recv(4)
            print(msg_part.decode('utf-8'), end='', flush=True)
        except ConnectionResetError as e:
            print("Connection was reset by peer: {}".format(e), end='', flush=True)

            break
    s.close()

DATA = {
   "algorithm": "crc32",
   "flags": 0x03,
   "message_direction": 0x01,
    "input": "Hello!!"
}

send_json(HOST, PORT, DATA)