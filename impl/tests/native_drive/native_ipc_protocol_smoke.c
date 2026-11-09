#include "../native_ipc_protocol.h"
#include <stdio.h>
#include <string.h>
static int header_test(void){
 SLIPCHeader a={1,SL_IPC_REQUEST,UINT64_C(0x0102030405060708),9,-3},b;uint8_t w[24];
 return sl_ipc_encode_header(&a,w,sizeof(w))==24&&w[0]=='S'&&
 sl_ipc_decode_header(w,sizeof(w),&b)==24&&a.request_id==b.request_id&&
 a.payload_length==b.payload_length&&a.status==b.status;
}
static int request_test(void){
 uint8_t payload[128];size_t n;SLNativeValue args[2],value;SLIPCRequestView view;
 memset(args,0,sizeof(args));args[0].tag=SL_NATIVE_VALUE_I64;args[0].as.i64=640320;
 args[1].tag=SL_NATIVE_VALUE_UTF8;args[1].as.utf8.data=(const uint8_t*)"pi";args[1].as.utf8.length=2;
 if(sl_ipc_encode_request((const uint8_t*)"bigint.mul",10,args,2,payload,sizeof(payload),&n))return 0;
 if(sl_ipc_decode_request(payload,n,&view)||view.method_length!=10||
    memcmp(view.method,"bigint.mul",10)||view.argument_count!=2)return 0;
 if(sl_ipc_decode_value(&view.arguments,&value)||value.tag!=SL_NATIVE_VALUE_I64||value.as.i64!=640320)return 0;
 if(sl_ipc_decode_value(&view.arguments,&value)||value.tag!=SL_NATIVE_VALUE_UTF8||
    value.as.utf8.length!=2||memcmp(value.as.utf8.data,"pi",2)||view.arguments.offset!=n)return 0;
 payload[n]=0;return sl_ipc_decode_request(payload,n+1,&view)<0;
}
static int value_test(void){
 uint8_t w[64];size_t n;SLNativeValue a,b;SLIPCCursor c;
 memset(&a,0,sizeof(a));a.tag=SL_NATIVE_VALUE_I64;a.as.i64=INT64_C(-9223372036854770000);
 if(sl_ipc_encode_value(&a,w,sizeof(w),&n))return 0;
 c.data=w;c.length=n;c.offset=0;
 if(sl_ipc_decode_value(&c,&b)||a.as.i64!=b.as.i64||c.offset!=n)return 0;
 memset(&a,0,sizeof(a));a.tag=SL_NATIVE_VALUE_UTF8;a.as.utf8.data=(const uint8_t*)"Pi \xcf\x80";a.as.utf8.length=5;
 if(sl_ipc_encode_value(&a,w,sizeof(w),&n))return 0;
 c.data=w;c.length=n;c.offset=0;
 if(sl_ipc_decode_value(&c,&b)||b.as.utf8.length!=5||memcmp(a.as.utf8.data,b.as.utf8.data,5))return 0;
 memset(&a,0,sizeof(a));a.tag=SL_NATIVE_VALUE_F64;a.as.f64=3.141592653589793;
 if(sl_ipc_encode_value(&a,w,sizeof(w),&n))return 0;
 c.data=w;c.length=n;c.offset=0;
 if(sl_ipc_decode_value(&c,&b)||b.as.f64!=a.as.f64)return 0;
 memset(&a,0,sizeof(a));a.tag=SL_NATIVE_VALUE_BOOL;a.as.boolean=1;
 if(sl_ipc_encode_value(&a,w,sizeof(w),&n))return 0;
 w[1]=2;c.data=w;c.length=n;c.offset=0;
 if(sl_ipc_decode_value(&c,&b)==0)return 0;
 w[0]=SL_NATIVE_VALUE_I64;c.data=w;c.length=1;c.offset=0;
 return sl_ipc_decode_value(&c,&b)<0;
}
int main(void){if(!header_test()||!value_test()||!request_test()){fputs("native IPC codec: FAIL\n",stderr);return 1;}puts("native IPC codec: PASS");return 0;}
