#include <iostream>
#include <string_view>
#include <string>
#include <cstring>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "general.h"
#include "subscriber.h"

using namespace std::literals;

Subscriber::Subscriber(const subscriber_confg &config_)
    : mosquittopp("appalphabot")
{   
    config = config_;
    username_pw_set(
        config.mqttUser.c_str(), config.mqttPassword.c_str()
    );
    std::cout << "Subscriber: connectiong to " << config.host << " " << config.port << std::endl;
    connect(config.host.c_str(), config.port, 60)
}

void Subscriber::on_connect(int rc)
{
    std::cout << app::AppTag << "Subscriber connected with code "
              << rc << std::endl;

    if (rc == 0)
    {
        subscribe(NULL, config.subTopic.c_str());
    }
}

void Subscriber::on_message(const struct mosquitto_message *message)
{
    if (config.subTopic == message->topic)
    {
        const std::string_view payload{reinterpret_cast<char*>(message->payload),
                                       static_cast<size_t>(message->payloadlen)};
        std::cout << app::AppTag << "Got message with topic: " << message->topic
                  << ", payload: " << payload << std::endl;
    }
}

void Subscriber::on_subscribe(__rtl_unused int        mid,
                              __rtl_unused int        qos_count,
                              __rtl_unused const int *granted_qos)
{
    std::cout << app::AppTag << "Subscription succeeded." << std::endl;
    auto message = "alive";
    auto replyTopic = config.subTopic + "/ctl";
    int msg;
    publish(&msg, replyTopic.c_str(), strlen(message), message);
}
