import unittest
from h21_flow import Envelope, Response, Status, RetryClass, Pipeline, MAX_PAYLOAD

class EnvelopeTests(unittest.TestCase):
    def test_binary_safe_round_trip(self):
        e=Envelope(1,2,99,b"a:\x00\n")
        self.assertEqual(Envelope.unpack_text(e.pack_text()),e)
    def test_reject_length_mismatch_and_trailing_bytes(self):
        w=Envelope(1,1,1,b"x").pack_text()
        for bad in (w+b"junk",w.replace(b"1:x",b"2:x")):
            with self.assertRaises(ValueError): Envelope.unpack_text(bad)
    def test_reject_version_flags_and_signed_values(self):
        for bad in (b"H21 3 0 1 1 1 0:\n",b"H21 2 128 1 1 1 0:\n",b"H21 -2 0 1 1 1 0:\n"):
            with self.assertRaises(ValueError): Envelope.unpack_text(bad)
    def test_response_length(self):
        w=Response(Status.OK,4,b"ok").pack_text()
        self.assertEqual(Response.unpack_text(w).result,b"ok")
        with self.assertRaises(ValueError): Response.unpack_text(w+b"x")
    def test_payload_limit(self):
        with self.assertRaises(ValueError): Envelope(1,1,1,b"x"*(MAX_PAYLOAD+1)).pack_text()

class PipelineTests(unittest.TestCase):
    def test_dispatch(self):
        p=Pipeline(); sid,oid=p.register("echo","echo",RetryClass.READ,lambda d,c:(Status.OK,d))
        r=Response.unpack_text(p.handle_wire(Envelope(sid,oid,42,b"hello").pack_text()))
        self.assertEqual((r.status,r.request_id,r.result),(Status.OK,42,b"hello"))
    def test_long_name_rejected_not_truncated(self):
        p=Pipeline()
        with self.assertRaises(ValueError): p.register("x"*64,"op",RetryClass.READ,lambda d,c:(Status.OK,d))

if __name__=="__main__": unittest.main()
