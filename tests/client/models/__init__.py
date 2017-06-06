import enum
import json


class NPCTJsonMessage(object):
    class MsgType(enum.Enum):
        ENCODE = 0x01
        DECODE = 0x02

    def __init__(self, algorithm: str, data: str=None, data_key="input", msg_type: MsgType=MsgType.ENCODE):
        self._data = data
        self._algorithm = algorithm
        self._msg_type = msg_type  # type: NPCTJsonMessage.MsgType
        self._data_key = data_key

    def serialize(self) -> bytes:
        raw = json.dumps({
            "algorithm": self._algorithm,
            "flags": 0x03,  # internal
            "message_direction": self._msg_type.value,
            self._data_key: self._data
        }).encode('utf-8')
        return len(raw).to_bytes(4, 'big') + raw
