from tests.client.io import *
from tests.client._test_runner import *
from tests.client.config import *
import zlib
import random
import base64


def get_error_vector(length: int, errors: int, error_range: range):
    # length in bytes
    bts = []
    real_errors = 0
    for i in range(length):
        val = 0
        for j in error_range:
            error_probability = float(errors)/float(length-i)
            val <<= 1
            if random.random() <= error_probability:
                val |= 1
                errors -= 1
                real_errors += 1
        bts.append(val)
    return bytes(bts), real_errors


class TestHamming(TestClient):
    ALGO = 'hamming'

    def testEncode(self):
        data = "hello world!"
        jmsg = NPCTJsonMessage(algorithm=self.ALGO, data=data,
                               msg_type=NPCTJsonMessage.MsgType.ENCODE)
        ok, resp = self._send_msg(jmsg, ENDPOINT)
        self.assertTrue(ok)
        self.assertTrue(
            "error_code" not in resp,
            "Server failed with error: {}".format(json.dumps(resp))
        )
        return data, resp

    def _baseTestDecodeErrors(self, errors: int = 0):
        data, response = self.testEncode()
        encoded = base64.b64decode(response['input'])
        if errors > 0:
            e_vec, errors = get_error_vector(len(encoded), errors, range(7))
            encoded = bytes([(a ^ b) for (a, b) in zip(encoded, e_vec)])
        jmsg = NPCTJsonMessage(algorithm=self.ALGO, data=base64.b64encode(encoded).decode('utf-8'),
                               msg_type=NPCTJsonMessage.MsgType.DECODE)
        ok, resp = self._send_msg(jmsg, ENDPOINT)
        self.assertTrue(ok)
        self.assertEqual(resp["input"], data)
        self.assertEqual(resp["hamming"]["decode_errors"], errors)

    def testDecode(self):
        self._baseTestDecodeErrors()

    def testDecodeSingle(self):
        for i in range(255):
            self._baseTestDecodeErrors(1)
