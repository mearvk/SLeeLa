#include "rest_connector.h"
#include <curl/curl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static size_t write_cb(char*p,size_t s,size_t n,void*u){sleela_http_response*r=(sleela_http_response*)u;size_t z=s*n;if(z>SIZE_MAX-r->size-1)return 0;char*q=(char*)realloc(r->body,r->size+z+1);if(!q)return 0;r->body=q;memcpy(r->body+r->size,p,z);r->size+=z;r->body[r->size]=0;return z;}
static int request(const char*url,const char*body,long timeout,sleela_http_response*out){if(!out)return 0;memset(out,0,sizeof *out);if(!url){snprintf(out->error,sizeof out->error,"URL is required");return 0;}if(curl_global_init(CURL_GLOBAL_DEFAULT)!=CURLE_OK){snprintf(out->error,sizeof out->error,"libcurl initialization failed");return 0;}CURL*c=curl_easy_init();if(!c){snprintf(out->error,sizeof out->error,"libcurl handle allocation failed");return 0;}curl_easy_setopt(c,CURLOPT_URL,url);curl_easy_setopt(c,CURLOPT_FOLLOWLOCATION,0L);curl_easy_setopt(c,CURLOPT_CONNECTTIMEOUT,timeout>0?timeout:10L);curl_easy_setopt(c,CURLOPT_TIMEOUT,timeout>0?timeout:30L);curl_easy_setopt(c,CURLOPT_WRITEFUNCTION,write_cb);curl_easy_setopt(c,CURLOPT_WRITEDATA,out);curl_easy_setopt(c,CURLOPT_SSL_VERIFYPEER,1L);curl_easy_setopt(c,CURLOPT_SSL_VERIFYHOST,2L);struct curl_slist*h=NULL;if(body){h=curl_slist_append(h,"Content-Type: application/json");if(!h){snprintf(out->error,sizeof out->error,"HTTP header allocation failed");curl_easy_cleanup(c);return 0;}curl_easy_setopt(c,CURLOPT_POST,1L);curl_easy_setopt(c,CURLOPT_POSTFIELDS,body);curl_easy_setopt(c,CURLOPT_POSTFIELDSIZE,(long)strlen(body));curl_easy_setopt(c,CURLOPT_HTTPHEADER,h);}CURLcode rc=curl_easy_perform(c);curl_easy_getinfo(c,CURLINFO_RESPONSE_CODE,&out->status);if(rc!=CURLE_OK)snprintf(out->error,sizeof out->error,"%s",curl_easy_strerror(rc));if(h)curl_slist_free_all(h);curl_easy_cleanup(c);return rc==CURLE_OK;}
int sleela_http_get(const char*u,long t,sleela_http_response*r){return request(u,NULL,t,r);}
int sleela_http_post_json(const char*u,const char*b,long t,sleela_http_response*r){return request(u,b?b:"",t,r);}
void sleela_http_response_free(sleela_http_response*r){if(r){free(r->body);memset(r,0,sizeof *r);}}
