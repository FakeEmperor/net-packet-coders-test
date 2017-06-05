# send - host / port
import json	
import socket
import sys


class Unbuffered(object):

    def __init__(self, stream):
        self.stream = stream

    def write(self, data):
        self.stream.write(data)
        self.stream.flush()

    def writelines(self, datas):
        self.stream.writelines(datas)
        self.stream.flush()

    def __getattr__(self, attr):
        return getattr(self.stream, attr)

sys.stdout = Unbuffered(sys.stdout)


def _find_getch():
    try:
        import termios
    except ImportError:
        # Non-POSIX. Return msvcrt's (Windows') getch.
        import msvcrt
        return msvcrt.getch

    # POSIX system. Create and return a getch that manipulates the tty.
    import sys, tty
    def _getch():
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(fd)
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch

    return _getch

getch = _find_getch()

HOST = "127.0.0.1"
PORT = 10000


def send_json(host, port, jsdata):
    raw = json.dumps(jsdata).encode('utf-8')
    l = len(raw).to_bytes(4, 'big')
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((host, port))
    print("Sending: {}{}".format(l, raw))
    s.send(l)
    s.send(raw)
    print("Receiving...")
    while True:
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

if __name__ == "__main__":
    print("Starting tests...")
    try:
        send_json(HOST, PORT, DATA)
    except KeyboardInterrupt:
        print("Finishing...")
        exit()
