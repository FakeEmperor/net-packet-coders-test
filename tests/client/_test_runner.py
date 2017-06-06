from typing import Tuple, Optional
import unittest
import socket
import select
import json

from ..client.models import NPCTJsonMessage


class TestClient(unittest.TestCase):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    @staticmethod
    def _recv_result(s: socket.socket):
        s.setblocking(0) # disable blocking receive
        scope_balance = 0
        received_data = ""
        has_received = False
        received_json = None
        while not has_received:
            try:
                ready = select.select([s], [], [], 5)
                if not ready[0]:
                    print("recv() timeout!")
                    break
                # receive byte when ready
                msg_part = s.recv(1).decode('utf-8')
                received_data += msg_part
                print(msg_part, end='')
                # check if full JSON object received
                for i in range(len(msg_part)):
                    if msg_part[i] == '{':
                        scope_balance += 1
                    elif msg_part[i] == '}':
                        scope_balance -= 1
                # if JSON is invalid
                if scope_balance < 0:
                    print("Received object is not a JSON object: '{}'".format(received_data))
                    break
                elif scope_balance == 0:
                    has_received = True
                    received_json = json.loads(received_data)
            except ConnectionResetError as e:
                print("Connection was reset by peer: {}".format(e), end='')
        s.setblocking(1) # restore blocking
        return has_received, received_json

    @staticmethod
    def _send_msg(msg: NPCTJsonMessage, endpoint: Tuple[str, int]) -> Tuple[bool, Optional[dict]]:

        print("Connecting to {}:{}".format(*endpoint))
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(endpoint)
        print("Connected!")
        msg_serialized = msg.serialize()
        print("Sending data '{}'...".format(msg_serialized))
        s.send(msg_serialized)
        print("Sent data. Receiving response...")
        has_received, recevied_json = TestClient._recv_result(s)
        s.close()
        return has_received, recevied_json
