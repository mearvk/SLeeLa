import hashlib, sys, unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
import sleela_ipc_worker as ipc

class IPCWorkerTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.script=Path(ipc.__file__).resolve()
        cls.python=Path(sys.executable).resolve()
        cls.digest=hashlib.sha256(cls.python.read_bytes()).hexdigest()
    def worker(self):
        return ipc.Worker([str(self.python),str(self.script),"--worker"],self.digest,
                          cwd=str(self.script.parent),timeout=10)
    def test_handshake_and_multiple_requests(self):
        w=self.worker()
        try:
            self.assertEqual(w.call("math.ping"),"pong")
            self.assertIn("pi.compute",w.call("math.capabilities"))
            self.assertEqual(w.call("bigint.add","12345678901234567890","10"),"12345678901234567900")
            self.assertEqual(w.call("bigint.mul","123456789","987654321"),"121932631112635269")
        finally: w.close()
    def test_pi_reference(self):
        w=self.worker()
        try: self.assertEqual(w.call("pi.compute",50),"3.14159265358979323846264338327950288419716939937510")
        finally: w.close()
    def test_executable_integrity_fails_closed(self):
        with self.assertRaises(ipc.IPCError):
            ipc.Worker([str(self.python),str(self.script),"--worker"],"0"*64,cwd=str(self.script.parent))
    def test_frame_bounds_and_bad_magic(self):
        import io
        with self.assertRaises(ipc.IPCError): ipc._frame(ipc.REQUEST,1,b"x"*(ipc.MAX_PAYLOAD+1))
        with self.assertRaises(ipc.IPCError): ipc._read_frame(io.BytesIO(b"NOPE"+b"\0"*20))

if __name__=="__main__": unittest.main()
