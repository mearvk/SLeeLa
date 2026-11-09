#!/usr/bin/env python3
"""SLeeLa Native IPC v1 persistent worker host and reference worker."""
from __future__ import annotations
import argparse, hashlib, os, queue, struct, subprocess, sys, threading
from pathlib import Path

MAGIC=b"SLIP"; VERSION=1; HEADER=struct.Struct("<4sHHQIi"); MAX_PAYLOAD=16*1024*1024
REQUEST, RESPONSE, CANCEL, HELLO, HELLO_ACK = 1,2,3,4,5
NULL, BOOL, I64, F64, UTF8 = 0,1,2,3,4

class IPCError(RuntimeError): pass

def _read_exact(stream,n):
    chunks=[]; left=n
    while left:
        b=stream.read(left)
        if not b: raise EOFError("worker closed IPC stream")
        chunks.append(b); left-=len(b)
    return b"".join(chunks)

def _frame(kind,rid,payload=b"",status=0):
    if len(payload)>MAX_PAYLOAD: raise IPCError("IPC payload exceeds 16 MiB")
    return HEADER.pack(MAGIC,VERSION,kind,rid,len(payload),status)+payload

def _read_frame(stream):
    raw=_read_exact(stream,HEADER.size)
    magic,version,kind,rid,n,status=HEADER.unpack(raw)
    if magic!=MAGIC or version!=VERSION or kind not in (REQUEST,RESPONSE,CANCEL,HELLO,HELLO_ACK) or n>MAX_PAYLOAD:
        raise IPCError("invalid IPC frame header")
    return kind,rid,status,_read_exact(stream,n)

def _encode_value(v):
    if v is None: return bytes([NULL])
    if isinstance(v,bool): return bytes([BOOL,int(v)])
    if isinstance(v,int):
        if not -(1<<63)<=v<(1<<63): raise IPCError("integer outside scalar int64 ABI")
        return bytes([I64])+struct.pack("<q",v)
    if isinstance(v,float): return bytes([F64])+struct.pack("<d",v)
    if isinstance(v,str):
        b=v.encode("utf-8","strict")
        if len(b)>MAX_PAYLOAD-5: raise IPCError("UTF-8 value too large")
        return bytes([UTF8])+struct.pack("<I",len(b))+b
    raise IPCError("unsupported IPC scalar type")

def _decode_value(data,offset=0):
    if offset>=len(data): raise IPCError("truncated scalar")
    tag=data[offset]; offset+=1
    if tag==NULL: return None,offset
    if tag==BOOL:
        if offset>=len(data) or data[offset]>1: raise IPCError("invalid boolean")
        return bool(data[offset]),offset+1
    if tag in (I64,F64):
        if len(data)-offset<8: raise IPCError("truncated numeric scalar")
        return struct.unpack_from("<q" if tag==I64 else "<d",data,offset)[0],offset+8
    if tag==UTF8:
        if len(data)-offset<4: raise IPCError("truncated UTF-8 length")
        n=struct.unpack_from("<I",data,offset)[0]; offset+=4
        if n>MAX_PAYLOAD or len(data)-offset<n: raise IPCError("invalid UTF-8 span")
        try: value=data[offset:offset+n].decode("utf-8","strict")
        except UnicodeDecodeError as e: raise IPCError("invalid UTF-8") from e
        return value,offset+n
    raise IPCError("unknown scalar tag")

def _request_payload(method,args):
    m=method.encode("utf-8","strict")
    if not m or len(m)>1024 or b"\0" in m or len(args)>65535: raise IPCError("invalid method or argument count")
    out=bytearray(struct.pack("<H",len(m))+m+struct.pack("<H",len(args)))
    for arg in args: out.extend(_encode_value(arg))
    if len(out)>MAX_PAYLOAD: raise IPCError("request too large")
    return bytes(out)

def _parse_request(data):
    if len(data)<4: raise IPCError("truncated request")
    n=struct.unpack_from("<H",data)[0]
    if not n or n>1024 or len(data)<n+4: raise IPCError("invalid method length")
    method=data[2:2+n].decode("utf-8","strict")
    argc=struct.unpack_from("<H",data,2+n)[0]; off=4+n; args=[]
    for _ in range(argc):
        v,off=_decode_value(data,off); args.append(v)
    if off!=len(data): raise IPCError("trailing request bytes")
    return method,args

class Worker:
    """Host-side persistent child. Pin argv[0] by SHA-256 and pass argv, never a shell."""
    def __init__(self,argv,executable_sha256,*,cwd,env_allowlist=(),timeout=30.0,max_stderr=1<<20):
        if not isinstance(argv,(list,tuple)) or not argv: raise IPCError("argv must be a non-empty sequence")
        exe=Path(argv[0]).resolve(strict=True)
        if not exe.is_file(): raise IPCError("worker executable is not a regular file")
        if hashlib.sha256(exe.read_bytes()).hexdigest().lower()!=executable_sha256.lower():
            raise IPCError("worker executable integrity mismatch")
        workdir=Path(cwd).resolve(strict=True)
        if not workdir.is_dir(): raise IPCError("worker cwd is not a directory")
        env={k:os.environ[k] for k in env_allowlist if k in os.environ}
        self.proc=subprocess.Popen([str(exe),*map(str,argv[1:])],stdin=subprocess.PIPE,stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,cwd=str(workdir),env=env,shell=False,bufsize=0,close_fds=True)
        self.timeout=float(timeout); self.max_stderr=max_stderr; self._rid=0; self._lock=threading.Lock()
        self._stderr=bytearray(); self._stderr_overflow=False
        threading.Thread(target=self._drain_stderr,daemon=True).start()
        try:
            self.proc.stdin.write(_frame(HELLO,0,b"sleela-ipc/1")); self.proc.stdin.flush()
            kind,rid,status,payload=self._read_with_timeout()
            if kind!=HELLO_ACK or rid!=0 or status!=0 or payload!=b"sleela-ipc/1":
                raise IPCError("worker handshake rejected")
        except Exception:
            self.close(force=True); raise
    def _drain_stderr(self):
        while True:
            b=self.proc.stderr.read(4096)
            if not b: return
            room=self.max_stderr-len(self._stderr)
            if room>0: self._stderr.extend(b[:room])
            if len(b)>room: self._stderr_overflow=True
    def _read_with_timeout(self):
        result=queue.Queue(maxsize=1)
        def read():
            try: result.put((True,_read_frame(self.proc.stdout)))
            except BaseException as e: result.put((False,e))
        threading.Thread(target=read,daemon=True).start()
        try: ok,value=result.get(timeout=self.timeout)
        except queue.Empty: raise TimeoutError("IPC worker response timed out")
        if not ok: raise value
        if self._stderr_overflow: raise IPCError("worker stderr exceeded configured bound")
        return value
    def call(self,method,*args):
        with self._lock:
            if self.proc.poll() is not None: raise IPCError("worker is not running")
            self._rid+=1; rid=self._rid
            self.proc.stdin.write(_frame(REQUEST,rid,_request_payload(method,args))); self.proc.stdin.flush()
            kind,got,status,payload=self._read_with_timeout()
            if kind!=RESPONSE or got!=rid: raise IPCError("unexpected response frame or request id")
            if status:
                try: diagnostic=payload.decode("utf-8","strict")
                except UnicodeDecodeError: diagnostic="invalid worker diagnostic"
                raise IPCError(f"worker status {status}: {diagnostic[:2048]}")
            value,end=_decode_value(payload)
            if end!=len(payload): raise IPCError("trailing response bytes")
            return value
    @property
    def stderr(self): return bytes(self._stderr).decode("utf-8","replace")
    def close(self,force=False):
        p=getattr(self,"proc",None)
        if not p: return
        if p.poll() is None:
            if not force:
                try:
                    p.stdin.write(_frame(CANCEL,0)); p.stdin.flush(); p.stdin.close(); p.wait(timeout=2)
                except Exception: force=True
            if force and p.poll() is None:
                p.terminate()
                try: p.wait(timeout=2)
                except subprocess.TimeoutExpired: p.kill(); p.wait()
        for s in (p.stdin,p.stdout,p.stderr):
            try: s.close()
            except Exception: pass

def _bs(a,b):
    if b-a==1:
        if a==0: return 1,1,13591409
        p=(6*a-5)*(2*a-1)*(6*a-1); q=a*a*a*10939058860032000
        t=p*(13591409+545140134*a)
        if a&1: t=-t
        return p,q,t
    m=(a+b)//2; p1,q1,t1=_bs(a,m); p2,q2,t2=_bs(m,b)
    return p1*p2,q1*q2,t1*q2+p1*t2

def pi_digits(n):
    if not isinstance(n,int) or isinstance(n,bool) or not 1<=n<=200000: raise ValueError("digits must be 1..200000")
    terms=n//14+2; p,q,t=_bs(0,terms)
    scaled=(426880*__import__("math").isqrt(10005*10**(2*n))*q)//t
    s=str(scaled).zfill(n+1)
    return s[:-n]+"."+s[-n:]

def worker_main():
    while True:
        kind,rid,status,payload=_read_frame(sys.stdin.buffer)
        if kind==CANCEL: return 0
        if kind==HELLO:
            sys.stdout.buffer.write(_frame(HELLO_ACK,rid,b"sleela-ipc/1")); sys.stdout.buffer.flush(); continue
        if kind!=REQUEST: return 2
        try:
            method,args=_parse_request(payload)
            if method=="math.ping": result="pong"
            elif method=="math.capabilities": result="math.ping,bigint.add,bigint.sub,bigint.mul,pi.compute"
            elif method in ("bigint.add","bigint.sub","bigint.mul") and len(args)==2 and all(isinstance(x,str) for x in args):
                a,b=map(int,args); result=str(a+b if method.endswith("add") else a-b if method.endswith("sub") else a*b)
            elif method=="pi.compute" and len(args)==1: result=pi_digits(args[0])
            else: raise ValueError("NOT_HANDLED: unsupported method/signature")
            out=_frame(RESPONSE,rid,_encode_value(result),0)
        except Exception as e: out=_frame(RESPONSE,rid,str(e).encode("utf-8","replace")[:2048],2)
        sys.stdout.buffer.write(out); sys.stdout.buffer.flush()

def main():
    ap=argparse.ArgumentParser(); ap.add_argument("--worker",action="store_true"); args=ap.parse_args()
    if args.worker: return worker_main()
    ap.error("import Worker for host use; --worker starts the reference worker")
if __name__=="__main__": raise SystemExit(main())
