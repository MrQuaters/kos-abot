/* © 2021 AO Kaspersky Lab */
#ifndef _MOSQUITTO_SUBSCRIBER_H
#define _MOSQUITTO_SUBSCRIBER_H

#include <mosquittopp.h>
#include <rtl/compiler.h>

/* Files required for transport initialization. */
#include <coresrv/nk/transport-kos.h>
#include <coresrv/sl/sl_api.h>

/* Description of the server interface used by the `client` entity. */
#include <echo/Ping.idl.h>

#include <assert.h>
class Subscriber : public mosqpp::mosquittopp
{
    NkKosTransport transport;
    struct echo_Ping_proxy proxy;
    
public:
    Subscriber(const char *id, const char *host, int port);
    ~Subscriber() {};

    void on_connect(int rc) override;
    void on_message(const struct mosquitto_message *message) override;
    void on_subscribe(__rtl_unused int        mid,
                      __rtl_unused int        qos_count,
                      __rtl_unused const int *granted_qos) override;
};

#endif // _MOSQUITTO_SUBSCRIBER_H
