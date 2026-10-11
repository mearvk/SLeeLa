#ifndef SLEELA_MQTT_CONNECTOR_HPP
#define SLEELA_MQTT_CONNECTOR_HPP
#include "mqtt_connector.h"
#include <functional>
#include <stdexcept>
#include <string>
namespace sleela::connector {
class MqttClient {
 sleela_mqtt c_{};
 std::function<void(const std::string&,const std::string&,int)> callback_;
 static void dispatch(void*ctx,const char*topic,const void*payload,size_t size,int qos){auto*self=static_cast<MqttClient*>(ctx);if(self&&self->callback_)self->callback_(topic?topic:"",std::string(static_cast<const char*>(payload),size),qos);}
public:
 MqttClient(const std::string&id,const std::string&host,int port=1883,int keepalive=30){if(!sleela_mqtt_open(&c_,id.c_str(),host.c_str(),port,keepalive))throw std::runtime_error(sleela_mqtt_error(&c_));}
 ~MqttClient(){sleela_mqtt_close(&c_);}
 MqttClient(const MqttClient&)=delete;MqttClient&operator=(const MqttClient&)=delete;
 void publish(const std::string&topic,const std::string&payload,int qos=0,bool retain=false){if(!sleela_mqtt_publish(&c_,topic.c_str(),payload.data(),payload.size(),qos,retain))throw std::runtime_error(sleela_mqtt_error(&c_));}
 void subscribe(const std::string&topic,std::function<void(const std::string&,const std::string&,int)> cb,int qos=0){callback_=std::move(cb);if(!sleela_mqtt_set_message_callback(&c_,dispatch,this)||!sleela_mqtt_subscribe(&c_,topic.c_str(),qos))throw std::runtime_error(sleela_mqtt_error(&c_));}
 bool loop(int timeoutMs=100){return sleela_mqtt_loop(&c_,timeoutMs)!=0;}
};
}
#endif
