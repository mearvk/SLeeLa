#include "../native_ipc_protocol.h"
#include <stdio.h>
#include <string.h>
static int header_test(void){
 SLIPCHeader a={1,SL_IPC_REQUEST,UINT64_C(0x0102030405060708),9,-3},b;uint8_t w[24];
 return sl_ipc_encode_header(&a,w,sizeof(w))==24&&w[0]=='S'&&
 sl_ipc_decode_header(w,sizeof(w),&b)==24&&a.request_id==b.request_id&&
 a.payload_length==b.payload_length&&a.status==b.status;
}
static int value_test(void){
 uint8_t w[64];size_t n;SLNativeValue a,b;SLIPCCursor c;
 memset(&a,0,sizeof(a));a.tag=SL_NATIVE_VALUE_I64;a.as.i64=INT64_C(-9223372036854770000);
 if(sl_ipc_encode_value(&a,w,sizeof(w),&n))return 0;
 c=(SLIPCCursor){w,n,0};
 if(sl_ipc_decode_value(&c,&b)||a.as.i64!=b.as.i64||c.offset!=n)return 0;
 memset(&a,0,sizeof(a));a.tag=SL_NATIVE_VALUE_UTF8;a.as.utf8.data=(const uint8_t*)"Pi \xcf\x80";a.as.utf8.length=5;
 if(sl_ipc_encode_value(&a,w,sizeof(w),&n))return 0;
 c=(SLIPCCursor){w,n,0};
 if(sl_ipc_decode_value(&c,&b)||b.as.utf8.length!=5||memcmp(a.as.utf8.data,b.as.utf8.data,5))return 0;
 memset(&a,0,sizeof(a));a.tag=SL_NATIVE_VALUE_BOOL;a.as.boolean=2;
 if(sl_ipc_encode_value(&a,w,sizeof(w),&n)==0)return 0;
 w[0]=SL_NATIVE_VALUE_I64;c=(SLIPCCursor){w,1,0};return sl_ipc_decode_value(&c,&b)<0;
}
int main(void){if(!header_test()||!value_test()){fputs("native IPC codec: FAIL\n",stderr);return 1;}puts("native IPC codec: PASS");return 0;}
