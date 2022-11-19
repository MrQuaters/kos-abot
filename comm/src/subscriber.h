/* © 2021 AO Kaspersky Lab */
#ifndef _MOSQUITTO_SUBSCRIBER_H
#define _MOSQUITTO_SUBSCRIBER_H

#include <mosquittopp.h>
#include <rtl/compiler.h>
#include <string>


typedef struct {
    std::string host;
    std::string subTopic;
    std::string mqttUser;
    std::string mqttPassword;
    int port;
} subscriber_confg; 


class Subscriber : public mosqpp::mosquittopp
{
    subscriber_confg config;
public:
    Subscriber(const subscriber_confg &config);
    ~Subscriber() {};

    void on_connect(int rc) override;
    void on_message(const struct mosquitto_message *message) override;
    void on_subscribe(__rtl_unused int        mid,
                      __rtl_unused int        qos_count,
                      __rtl_unused const int *granted_qos) override;
};

#endif // _MOSQUITTO_SUBSCRIBER_H
