from tests.client.io import *
from tests.client._test_runner import *
from tests.client.config import *
import zlib


class TestShannonFano(TestClient):
    ALGO = 'shannon_fano'

    def testEncode(self):
        data = "hello, world!"
        jmsg = NPCTJsonMessage(algorithm=self.ALGO, data=data,
                               msg_type=NPCTJsonMessage.MsgType.ENCODE)
        ok, resp = self._send_msg(jmsg, ENDPOINT)
        self.assertTrue(ok)
        self.assertTrue(
            "error_code" not in resp,
            "Server failed with error: {}".format(json.dumps(resp))
        )
        return data, resp

    def testDecode(self):
        data, response = self.testEncode()
        encoded = response['data']
        jmsg = NPCTJsonMessage(algorithm=self.ALGO, data=encoded,
                               msg_type=NPCTJsonMessage.MsgType.DECODE)
        ok, resp = self._send_msg(jmsg, ENDPOINT)
        self.assertTrue(ok)
        self.assertEqual(resp["data"], data)
