from tests.client.io import *
from tests.client._test_runner import *
from tests.client.config import *
import zlib


class TestCrc32(TestClient):
    ALGO = 'crc32'

    def testCrc32(self):
        data = "hello, world!"
        jmsg = NPCTJsonMessage(algorithm=self.ALGO, data=data, data_key="input",
                               msg_type=NPCTJsonMessage.MsgType.ENCODE)
        ok, resp = self._send_msg(jmsg, ENDPOINT)
        self.assertTrue(ok)
        self.assertEqual(resp["checksum"], zlib.crc32(data.encode('utf-8')))
