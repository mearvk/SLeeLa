#include "slvm9_native.h"
#include <stdint.h>
#include <string.h>
#if defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/mount.h>
#include <unistd.h>
#else
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/vfs.h>
#include <unistd.h>
#endif
static uint64_t mix(uint64_t h,uint64_t v){h^=v+UINT64_C(0x9e3779b97f4a7c15)+(h<<6)+(h>>2);h^=h>>29;h*=UINT64_C(0xbf58476d1ce4e5b9);h^=h>>32;return h;}
static uint64_t hash_text(uint64_t h,const char*s){while(s&&*s)h=mix(h,(unsigned char)*s++);return h;}
static int fill(slvm9_native_filesystem_t*out,const char*path,uint64_t id,uint64_t block,uint64_t total,uint64_t freeb,const char*type,int ro){if(!out||!path||!type||!block||!total)return SLVM9_INVALID;memset(out,0,sizeof(*out));out->filesystem.filesystem_id=id?id:1;out->filesystem.block_size=block;out->filesystem.total_bytes=total;out->filesystem.free_bytes=freeb;out->filesystem.type=type;out->filesystem.uuid="native-runtime-identity";out->filesystem.mount_identity=path;out->filesystem.feature_bits=SLVM9_FS_READ|SLVM9_FS_NATIVE_HANDLES|(ro?0:SLVM9_FS_WRITE);out->filesystem.mounted=1;out->filesystem.read_only=(uint8_t)ro;out->filesystem.integrity_valid=1;out->filesystem.durability_known=1;out->generation_token=hash_text(mix(mix(UINT64_C(0x534c564d39),out->filesystem.filesystem_id),block),type);out->generation_token=hash_text(out->generation_token,path);out->filesystem.generation=out->generation_token;out->read_only=(uint8_t)ro;out->native_probe=1;return SLVM9_OK;}
#if defined(_WIN32)
static int wide(const char*in,wchar_t*out,int cap){return in&&out&&MultiByteToWideChar(CP_UTF8,0,in,-1,out,cap)>0?SLVM9_OK:SLVM9_INVALID;}
int slvm9_native_probe_filesystem(const char*path,slvm9_native_filesystem_t*out){wchar_t p[MAX_PATH],fs[MAX_PATH];ULARGE_INTEGER fr,tot,tf;DWORD serial=0,maxc=0,flags=0;char type[64];if(wide(path,p,MAX_PATH)!=SLVM9_OK||!GetVolumeInformationW(p,NULL,0,&serial,&maxc,&flags,fs,MAX_PATH)||!GetDiskFreeSpaceExW(p,&fr,&tot,&tf)||WideCharToMultiByte(CP_UTF8,0,fs,-1,type,sizeof(type),NULL,NULL)<=0)return SLVM9_INVALID;return fill(out,path,serial,4096,tot.QuadPart,fr.QuadPart,type,(flags&FILE_READ_ONLY_VOLUME)!=0);}
int slvm9_native_probe_file(const char*path,slvm9_native_file_t*out){wchar_t p[MAX_PATH];HANDLE h;BY_HANDLE_FILE_INFORMATION i;if(!out||wide(path,p,MAX_PATH)!=SLVM9_OK)return SLVM9_INVALID;h=CreateFileW(p,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);if(h==INVALID_HANDLE_VALUE)return SLVM9_INVALID;if(!GetFileInformationByHandle(h,&i)){CloseHandle(h);return SLVM9_INVALID;}CloseHandle(h);memset(out,0,sizeof(*out));out->native_identity=((uint64_t)i.nFileIndexHigh<<32)|i.nFileIndexLow;out->file.object_id=out->native_identity?out->native_identity:1;out->file.generation=1;out->file.size=((uint64_t)i.nFileSizeHigh<<32)|i.nFileSizeLow;out->file.name=path;out->file.identity_valid=1;out->file.readable=1;out->file.writable=(GetFileAttributesW(p)&FILE_ATTRIBUTE_READONLY)==0;out->native_probe=1;return SLVM9_OK;}
#else
int slvm9_native_probe_filesystem(const char*path,slvm9_native_filesystem_t*out){
 struct statvfs v;
 if(!path||!out||statvfs(path,&v)!=0)return SLVM9_INVALID;
#if defined(__APPLE__)
 struct statfs s;if(statfs(path,&s)!=0)return SLVM9_INVALID;uint64_t id=((uint64_t)(uint32_t)s.f_fsid.val[0]<<32)|(uint32_t)s.f_fsid.val[1];uint64_t b=v.f_frsize?v.f_frsize:v.f_bsize;return fill(out,path,id,b,(uint64_t)v.f_blocks*b,(uint64_t)v.f_bavail*b,s.f_fstypename,(s.f_flags&MNT_RDONLY)!=0);
#elif defined(__linux__)
 struct statfs s;if(statfs(path,&s)!=0)return SLVM9_INVALID;uint64_t id=((uint64_t)(uint32_t)s.f_fsid.val[0]<<32)|(uint32_t)s.f_fsid.val[1];uint64_t b=v.f_frsize?v.f_frsize:v.f_bsize;return fill(out,path,id,b,(uint64_t)v.f_blocks*b,(uint64_t)v.f_bavail*b,"native-linux",(s.f_flags&ST_RDONLY)!=0);
#else
 return SLVM9_INVALID;
#endif
}
int slvm9_native_probe_file(const char*path,slvm9_native_file_t*out){struct stat s;if(!path||!out||stat(path,&s)!=0)return SLVM9_INVALID;memset(out,0,sizeof(*out));out->native_identity=mix((uint64_t)s.st_dev,(uint64_t)s.st_ino);out->file.object_id=out->native_identity?out->native_identity:1;out->file.generation=1;out->file.size=(uint64_t)s.st_size;out->file.name=path;out->file.identity_valid=1;out->file.readable=(access(path,R_OK)==0);out->file.writable=(access(path,W_OK)==0);out->native_probe=1;return out->file.readable?SLVM9_OK:SLVM9_DENIED;}
#endif
int slvm9_native_validate_generation(const slvm9_native_filesystem_t*fs,const slvm9_filesystem_t*expected){return(!fs||!expected||!fs->native_probe)?SLVM9_INVALID:(fs->filesystem.filesystem_id==expected->filesystem_id&&fs->filesystem.generation==expected->generation?SLVM9_OK:SLVM9_STALE);}
const char*slvm9_native_backend_name(void){
#if defined(_WIN32)
return "SLVM/9 native Windows adapter";
#elif defined(__APPLE__)
return "SLVM/9 native macOS adapter";
#elif defined(__linux__)
return "SLVM/9 native Linux adapter";
#else
return "SLVM/9 unsupported native adapter";
#endif
}
const char*slvm9_native_filesystem_name(void){
#if defined(_WIN32)
return "Windows volume/filesystem";
#elif defined(__APPLE__)
return "Darwin filesystem";
#elif defined(__linux__)
return "Linux statfs/statvfs filesystem";
#else
return "unknown";
#endif
}
