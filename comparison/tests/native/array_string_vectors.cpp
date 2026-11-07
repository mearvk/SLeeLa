#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
static bool cont(unsigned char b){return b>=0x80&&b<=0xBF;}
static bool valid(const std::string&s){auto*p=reinterpret_cast<const unsigned char*>(s.data());size_t n=s.size(),i=0;while(i<n){unsigned char a=p[i],b=0;if(a<=0x7F){i++;continue;}if(a>=0xC2&&a<=0xDF){if(i+1>=n||!cont(p[i+1]))return false;i+=2;continue;}if(a>=0xE0&&a<=0xEF){if(i+2>=n||!cont(p[i+2]))return false;b=p[i+1];if(a==0xE0&&(b<0xA0||b>0xBF))return false;if(a==0xED&&(b<0x80||b>0x9F))return false;if(a!=0xE0&&a!=0xED&&!cont(b))return false;i+=3;continue;}if(a>=0xF0&&a<=0xF4){if(i+3>=n||!cont(p[i+2])||!cont(p[i+3]))return false;b=p[i+1];if(a==0xF0&&(b<0x90||b>0xBF))return false;if(a==0xF4&&(b<0x80||b>0x8F))return false;if(a!=0xF0&&a!=0xF4&&!cont(b))return false;i+=4;continue;}return false;}return true;}
static size_t cps(const std::string&s){size_t n=0;for(unsigned char b:s)if((b&0xC0)!=0x80)n++;return n;}
static int get(const std::vector<int>&a,int i){if(i<0)throw std::out_of_range("negative");return a.at(static_cast<size_t>(i));}
int main(){std::vector<int> empty,one{7},a{11,22,33};std::string s=u8"Aé😀",bad("\xC0\xAF",2);
std::cout<<"EMPTY_LENGTH="<<empty.size()<<"\nSINGLE_LENGTH="<<one.size()<<"\nFIRST_VALUE="<<get(a,0)<<"\nLAST_VALUE="<<get(a,2)<<"\n";
try{get(a,-1);std::cout<<"NEGATIVE_INDEX=ACCEPTED\n";}catch(const std::out_of_range&){std::cout<<"NEGATIVE_INDEX=REJECTED\n";}
try{get(a,3);std::cout<<"ONE_PAST_END=ACCEPTED\n";}catch(const std::out_of_range&){std::cout<<"ONE_PAST_END=REJECTED\n";}
std::cout<<"UTF8_ROUNDTRIP="<<(valid(s)&&s==std::string(u8"Aé😀")?"PASS":"FAIL")<<"\nUTF8_INVALID="<<(valid(bad)?"ACCEPTED":"REJECTED")<<"\nUTF8_BYTES="<<s.size()<<"\nUNICODE_CODEPOINTS="<<cps(s)<<"\n";}
