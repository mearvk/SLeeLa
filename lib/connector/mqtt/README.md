<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

# MQTT event connector

Uses Eclipse Mosquitto's C client library for MQTT connect, publish, subscribe, and message callback dispatch. The C++17 `MqttClient` facade supports callback-based subscriptions. Call `sleela_mqtt_global_cleanup()` once, after all clients have been closed. TLS/authentication configuration helpers and automatic reconnect policy are not exposed yet; configure a trusted test broker or provide an externally hardened deployment setup before production use. Network access starts only when `sleela_mqtt_open` is called.

Build from the repository root with `make -C lib/connector/mqtt build`, or from this directory with `make build`. Install the Mosquitto client development package first. The default group smoke tests do not connect to a live broker.