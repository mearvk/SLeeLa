#include "rest_connector.h"
#include <assert.h>

int main(void) {
    sleela_http_response response = {0};
    assert(sleela_http_get(NULL, 1, &response) == 0);
    assert(response.status == 0);
    assert(response.body == NULL);
    assert(response.size == 0);
    assert(response.error[0] != '\0');
    assert(sleela_http_post_json(NULL, "{}", 1, &response) == 0);
    assert(response.error[0] != '\0');
    sleela_http_response_free(&response);
    sleela_http_response_free(NULL);
    return 0;
}
