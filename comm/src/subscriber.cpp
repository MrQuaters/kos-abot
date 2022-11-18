#include <iostream>
#include <string_view>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "general.h"
#include "subscriber.h"

using namespace std::literals;

constexpr auto Topic = "my/awesome/topic"sv;

Subscriber::Subscriber(const char *id, const char *host, int port)
    : mosquittopp(id)
{
    std::cout << app::AppTag << "Connecting to MQTT Broker with address "
              << host << " and port " << port << std::endl;

    const int keepAlive = 60;
    Handle handle = ServiceLocatorConnect("configuration_connection");
    assert(handle != INVALID_HANDLE);

    /* Initialize IPC transport for interaction with the server entity. */
    NkKosTransport_Init(&this->transport, handle, NK_NULL, 0);

    /**
     * Get Runtime Interface ID (RIID) for interface echo.Ping.ping.
     * Here ping is the name of the echo.Ping component instance,
     * echo.Ping.ping is the name of the Ping interface implementation.
     */
    nk_iid_t riid = ServiceLocatorGetRiid(handle, "echo.Ping.ping");
    assert(riid != INVALID_RIID);

    /**
     * Initialize proxy object by specifying transport (&transport)
     * and server interface identifier (riid). Each method of the
     * proxy object will be implemented by sending a request to the server.
     */
    echo_Ping_proxy_init(&this->proxy, &this->transport.base, riid);

    connect(host, port, keepAlive);
}

void Subscriber::on_connect(int rc)
{
    std::cout << app::AppTag << "Subscriber connected with code "
              << rc << std::endl;

    if (rc == 0)
    {
        subscribe(NULL, Topic.data());
    }
}

void Subscriber::on_message(const struct mosquitto_message *message)
{
    if (Topic == message->topic)
    {
        const std::string_view payload{reinterpret_cast<char*>(message->payload),
                                       static_cast<size_t>(message->payloadlen)};
        std::cout << app::AppTag << "Got message with topic: " << message->topic
                  << ", payload: " << payload << std::endl;
        echo_Ping_Ping_req req;
        echo_Ping_Ping_res res;     
        std::string val = {payload.begin(), payload.end()};   
        req.value = (unsigned int) std::stoi(val, NULL);
    	if (echo_Ping_Ping(&this->proxy.base, &req, NULL, &res, NULL) == rcOk) {
            std::cout << "Send to server OK" << std::endl;

        } else {
            std::cerr << "Send to server FAIL" << std::endl;
        }
    }
}

void Subscriber::on_subscribe(__rtl_unused int        mid,
                              __rtl_unused int        qos_count,
                              __rtl_unused const int *granted_qos)
{
    std::cout << app::AppTag << "Subscription succeeded." << std::endl;
}
