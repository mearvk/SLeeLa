#ifndef SLEELA_MQTT_CONNECTOR_HPP
#define SLEELA_MQTT_CONNECTOR_HPP
#include "mqtt_connector.h"
#include <stdexcept>
#include <string>
namespace sleela::connector {
class MqttPublisher {
 sleela_mqtt c_{};
public:
 MqttPublisher(const std::string& id,const std::string& host,int port=1883,int keepalive=30){if(!sleela_mqtt_open(&c_,id.c_str(),host.c_str(),port,keepalive))throw std::runtime_error(sleela_mqtt_error(&c_));}
 ~MqttPublisher(){sleela_mqtt_close(&c_);}
 MqttPublisher(const MqttPublisher&)=delete;MqttPublisher&operator=(const MqttPublisher&)=delete;
 void publish(const std::string& topic,const std::string& payload,int qos=0,bool retain=false){if(!sleela_mqtt_publish(&c_,topic.c_str(),payload.data(),payload.size(),qos,retain))throw std::runtime_error(sleela_mqtt_error(&c_));}
 bool loop(int timeoutMs=100){return sleela_mqtt_loop(&c_,timeoutMs)!=0;}
};
}
#endif
