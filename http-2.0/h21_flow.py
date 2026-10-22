"""Strict Python reference for SLeeLa HTTP 2.1 application envelopes.
The current native server uses HTTP/1.x transport; this is not IETF HTTP/2.
"""
from __future__ import annotations
from dataclasses import dataclass
from enum import IntEnum
from typing import Callable, Dict, Tuple

ENVELOPE_VERSION=2
MAX_PAYLOAD=4096
MAX_SERVICES=64
MAX_OPS_PER_SERVICE=64
MAX_NAME_BYTES=63
KNOWN_FLAGS=0x0B

class Flag(IntEnum):
    NONE=0x00
    BINARY=0x01
    COMPRESSED=0x02
    IDEMPOTENT=0x08
class Status(IntEnum):
    OK=0
    APP_ERROR=1
    UNKNOWN_SERVICE=2
    UNKNOWN_OPERATION=3
    BAD_ENVELOPE=4
    TOO_LARGE=5
    RETRY_DENIED=6
class RetryClass(IntEnum):
    READ=0
    IDEMPOTENT=1
    MUTATING=2

def _uint(value, maximum, label):
    if isinstance(value,bool) or not isinstance(value,int) or not 0<=value<=maximum:
        raise ValueError("invalid "+label)
    return value

def _name(value,label):
    if not isinstance(value,str): raise ValueError("invalid "+label)
    raw=value.encode("utf-8","strict")
    if not raw or len(raw)>MAX_NAME_BYTES or any(b<0x21 or b==0x7f for b in raw):
        raise ValueError("invalid "+label)
    return value

def _text_parts(wire,prefix,field_count):
    if not isinstance(wire,bytes): raise ValueError("wire must be bytes")
    colon=wire.find(b":")
    if colon<0 or colon>127: raise ValueError("malformed header")
    try: header=wire[:colon].decode("ascii")
    except UnicodeDecodeError as exc: raise ValueError("non-ASCII header") from exc
    parts=header.split(" ")
    if len(parts)!=field_count or parts[0]!=prefix or any(not p for p in parts):
        raise ValueError("malformed header")
    tail=wire[colon+1:]
    if tail.endswith(b"\n"): tail=tail[:-1]
    return parts[1:],tail

@dataclass
class Envelope:
    service_id:int
    op_id:int
    request_id:int
    payload:bytes=b""
    flags:int=Flag.NONE
    version:int=ENVELOPE_VERSION
    def pack_text(self):
        _uint(self.version,255,"version")
        if self.version!=ENVELOPE_VERSION: raise ValueError("unsupported version")
        _uint(self.flags,255,"flags")
        if self.flags & ~KNOWN_FLAGS: raise ValueError("unknown flags")
        _uint(self.service_id,0xffffffff,"service id")
        _uint(self.op_id,0xffffffff,"operation id")
        _uint(self.request_id,0xffffffffffffffff,"request id")
        if not isinstance(self.payload,bytes) or len(self.payload)>MAX_PAYLOAD:
            raise ValueError("payload too large or not bytes")
        head=(f"H21 {self.version} {self.flags} {self.service_id} {self.op_id} "
              f"{self.request_id} {len(self.payload)}:").encode("ascii")
        return head+self.payload+b"\n"
    @staticmethod
    def unpack_text(wire):
        fields,payload=_text_parts(wire,"H21",7)
        ver,flags,sid,oid,rid,plen=fields
        if not all(v.isascii() and v.isdecimal() for v in fields):
            raise ValueError("numeric fields must be unsigned decimal")
        n=int(plen)
        if n>MAX_PAYLOAD or len(payload)!=n: raise ValueError("payload length mismatch")
        env=Envelope(int(sid),int(oid),int(rid),payload,int(flags),int(ver))
        env.pack_text()
        return env

@dataclass
class Response:
    status:Status
    request_id:int
    result:bytes=b""
    def pack_text(self):
        try: status=Status(self.status)
        except (ValueError,TypeError) as exc: raise ValueError("invalid status") from exc
        _uint(self.request_id,0xffffffffffffffff,"request id")
        if not isinstance(self.result,bytes) or len(self.result)>MAX_PAYLOAD:
            raise ValueError("result too large or not bytes")
        return f"H21R {int(status)} {self.request_id} {len(self.result)}:".encode("ascii")+self.result+b"\n"
    @staticmethod
    def unpack_text(wire):
        fields,result=_text_parts(wire,"H21R",4)
        status,rid,rlen=fields
        if not all(v.isascii() and v.isdecimal() for v in fields):
            raise ValueError("numeric fields must be unsigned decimal")
        n=int(rlen)
        if n>MAX_PAYLOAD or len(result)!=n: raise ValueError("result length mismatch")
        return Response(Status(int(status)),int(rid),result)

Handler=Callable[[bytes,object],Tuple[Status,bytes]]
class Naming:
    def __init__(self):
        self._services:Dict[str,int]={}
        self._ops:Dict[Tuple[int,str],int]={}
        self._next_service=1
    def intern_service(self,name):
        name=_name(name,"service name")
        if name in self._services: return self._services[name]
        if len(self._services)>=MAX_SERVICES: return 0
        sid=self._next_service; self._next_service+=1; self._services[name]=sid
        return sid
    def intern_op(self,service,op):
        sid=self.intern_service(service)
        if not sid:return 0
        op=_name(op,"operation name"); key=(sid,op)
        if key in self._ops:return self._ops[key]
        count=sum(1 for service_id,_ in self._ops if service_id==sid)
        if count>=MAX_OPS_PER_SERVICE:return 0
        self._ops[key]=count+1
        return count+1

class Pipeline:
    def __init__(self):
        self.naming=Naming()
        self._handlers:Dict[Tuple[int,int],Tuple[RetryClass,Handler,object]]={}
        self.requests_handled=0
    def register(self,service,op,retry,handler,ctx=None):
        if not callable(handler):raise ValueError("handler must be callable")
        try: retry=RetryClass(retry)
        except (ValueError,TypeError) as exc: raise ValueError("invalid retry class") from exc
        sid=self.naming.intern_service(service)
        oid=self.naming.intern_op(service,op) if sid else 0
        if not sid or not oid:raise ValueError("naming capacity or name validation failed")
        self._handlers[(sid,oid)]=(retry,handler,ctx)
        return sid,oid
    def dispatch(self,env):
        binding=self._handlers.get((env.service_id,env.op_id))
        if binding is None:
            known=env.service_id in self.naming._services.values()
            return Response(Status.UNKNOWN_OPERATION if known else Status.UNKNOWN_SERVICE,env.request_id)
        _,handler,ctx=binding
        try:
            status,result=handler(env.payload,ctx); status=Status(status)
            if not isinstance(result,bytes) or len(result)>MAX_PAYLOAD:
                return Response(Status.TOO_LARGE,env.request_id)
            response=Response(status,env.request_id,result)
        except Exception:
            response=Response(Status.APP_ERROR,env.request_id)
        self.requests_handled+=1
        return response
    def handle_wire(self,wire):
        try: response=self.dispatch(Envelope.unpack_text(wire))
        except (ValueError,UnicodeError,OverflowError): response=Response(Status.BAD_ENVELOPE,0)
        return response.pack_text()
