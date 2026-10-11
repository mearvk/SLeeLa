#include "mqtt_connector.h"
#include <assert.h>

int main(void) {
    sleela_mqtt client = {0};
    assert(sleela_mqtt_open(NULL, "test", "127.0.0.1", 1883, 5) == 0);
    assert(sleela_mqtt_open(&client, NULL, "127.0.0.1", 1883, 5) == 0);
    assert(sleela_mqtt_open(&client, "test", NULL, 1883, 5) == 0);
    assert(sleela_mqtt_set_message_callback(&client, NULL, NULL) == 0);
    assert(sleela_mqtt_subscribe(&client, "test/topic", 0) == 0);
    assert(sleela_mqtt_publish(&client, "test/topic", "x", 1, 0, 0) == 0);
    assert(sleela_mqtt_loop(&client, 1) == 0);
    sleela_mqtt_close(&client);
    sleela_mqtt_close(NULL);
    assert(sleela_mqtt_error(NULL) != NULL);
    return 0;
}
