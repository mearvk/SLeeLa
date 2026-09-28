#include "test_assert.h"
#include "../../http-servers/common/annotation_http_bridge.h"
static int valid_case(void){const sleela_http_forwarding f={"document-A","@next system/network/transport","system/network/transport",3U,1};SLEELA_TEST_ASSERT_EQ_INT(1,sleela_http_forwarding_validate(&f));return 0;}
static int null_case(void){SLEELA_TEST_ASSERT_EQ_INT(0,sleela_http_forwarding_validate(NULL));return 0;}
static int bad_grade(void){const sleela_http_forwarding f={"document-A","@next x","x",10U,1};SLEELA_TEST_ASSERT_EQ_INT(0,sleela_http_forwarding_validate(&f));return 0;}
static int bad_destination(void){const sleela_http_forwarding f={"document-A","@next x","../escape",3U,1};SLEELA_TEST_ASSERT_EQ_INT(0,sleela_http_forwarding_validate(&f));return 0;}
int main(void){if(valid_case()||null_case()||bad_grade()||bad_destination())return 1;puts("PASS: HTTP annotation bridge C ABI");return 0;}
