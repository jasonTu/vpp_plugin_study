#ifndef __included_plugin_test_h__
#define __included_plugin_test_h__

#include <vnet/vnet.h>
#include <vnet/ip/ip.h>

#include <vppinfra/hash.h>
#include <vppinfra/error.h>
#include <vppinfra/elog.h>

#define PLUGIN_TEST_VER "1.0"

typedef struct {
    /* API message ID base */
    u16 msg_id_base;

    /* convenience */
    vnet_main_t * vnet_main;
} plugin_test_main_t;

extern plugin_test_main_t plugin_test_main;

extern vlib_node_registration_t plugin_test_node;

#endif /* __included_plugin_test_h__ */
