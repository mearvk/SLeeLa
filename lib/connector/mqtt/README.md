# MQTT event connector

Uses Eclipse Mosquitto C client for MQTT connection, publish, and network-loop operations. Subscription callbacks, TLS configuration, authentication helpers, and reconnect policy are not yet exposed. Until those features are added, use only with a trusted/local test broker; production deployments require broker ACLs and TLS configured through a hardened integration layer. Network access starts only when `sleela_mqtt_open` is called.
