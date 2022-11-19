#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <memory>
#include <iostream>

/* Files required for transport initialization. */
#include <coresrv/nk/transport-kos.h>
#include <coresrv/sl/sl_api.h>
#include <rtl/string.h>
#include <rtl/stdio.h>

/* EDL description of the server entity. */
#include <echo/NavigationController.edl.h>
#include <assert.h>

class NavigationController;

/* Type of interface implementing object. */
typedef struct INavigationCommandImpl {
    struct echo_NavigationCommand base;     /* Base interface of object */
    std::shared_ptr<NavigationController> navCtr;
} INavigationCommandImpl;


/* Ping method implementation. */
static nk_err_t NavigationCommand_impl(struct echo_NavigationCommand *self,
                          const struct echo_NavigationCommand_SetNavigationCommand_req *req,
                          const struct nk_arena *req_arena,
                          struct echo_NavigationCommand_SetNavigationCommand_res *res,
                          struct nk_arena *res_arena) {
                            
        std::cerr << "[NavigationController] Got msg cmd: " << req->value.command << " dur ms:"
            << req->value.durationMs << std::endl;

        res->result = 1;

    return NK_EOK;
}


static struct echo_NavigationCommand *CreateINavigationCommandImpl(std::shared_ptr<NavigationController> navPtr) {
    /* Table of implementations of IPing interface methods. */
    static const struct echo_NavigationCommand_ops ops = {
        .SetNavigationCommand = NavigationCommand_impl
    };

    /* Interface implementing object. */
    static struct INavigationCommandImpl impl = {
        .base = {&ops},
        .navCtr = {navPtr}
    };

    return &impl.base;
}

/* Server entry point. */
int main(void) {
    NkKosTransport transport;
    ServiceId iid;

    Handle handle = ServiceLocatorRegister("navigation_connection", NULL, 0, &iid);
    assert(handle != INVALID_HANDLE);

    NkKosTransport_Init(&transport, handle, NK_NULL, 0);

   
    echo_NavigationController_entity_req req;
    char req_buffer[echo_NavigationController_entity_req_arena_size];
    struct nk_arena req_arena = NK_ARENA_INITIALIZER(req_buffer,
                                        req_buffer + sizeof(req_buffer));

    echo_NavigationController_entity_res res;
    char res_buffer[echo_NavigationController_entity_res_arena_size];
    struct nk_arena res_arena = NK_ARENA_INITIALIZER(res_buffer,
                                        res_buffer + sizeof(res_buffer));

    echo_NavigationCommand_component component;
    echo_NavigationCommand_component_init(&component, CreateINavigationCommandImpl(nullptr));

    echo_NavigationController_entity entity;
    echo_NavigationController_entity_init(&entity, &component);

    fprintf(stderr, "Navigation server started\n");

    /* Dispatch loop implementation. */
    do
    {
        /* Flush request/response buffers. */
        nk_req_reset(&req);
        nk_arena_reset(&req_arena);
        nk_arena_reset(&res_arena);

        /* Wait for request to server entity. */
        if (nk_transport_recv(&transport.base,
                              &req.base_,
                              &req_arena) != NK_EOK) {
            fprintf(stderr, "nk_transport_recv error\n");
        } else {
            echo_NavigationController_entity_dispatch(&entity, &req.base_, &req_arena,
                                        &res.base_, &res_arena);
        }

        /* Send response. */
        if (nk_transport_reply(&transport.base,
                               &res.base_,
                               &res_arena) != NK_EOK) {
            fprintf(stderr, "nk_transport_reply error\n");
        }
    }
    while (true);

    return EXIT_SUCCESS;
}
