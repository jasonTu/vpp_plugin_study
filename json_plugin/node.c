#include <vlib/vlib.h>
#include <vnet/vnet.h>
#include <vnet/pg/pg.h>
#include <vnet/ethernet/ethernet.h>
#include <vppinfra/error.h>

#include <json_plugin/plugin.h>

typedef enum
{
  PLUGIN_TEST_NEXT_IP4,
  PLUGIN_TEST_DROP,
  PLUGIN_TEST_NEXT_N,
} plugin_test_next_t;

extern vlib_node_registration_t plugin_test_node;

/*
typedef struct
{
  u32 next_index;
  u32 sw_if_index;
  u8 new_src_mac[6];
  u8 new_dst_mac[6];
} plugin_test_trace_t;

#define foreach_plugin_test_error \
    _(SHOWED, "show packets processed")

typedef enum
{
#define _(sym,str) TEST_ERROR_##sym,
    foreach_plugin_test_error
#undef _
    TEST_N_ERROR,
} plugin_test_error_t;

static char *plugin_test_error_strings[] = {
#define _(sym, str) str,
    foreach_plugin_test_error
#undef _
};

static u8 *format_plugin_test_trace(u8 * s, va_list * args)
{
    s = format(s, "To Do!\n");
    return s;
}
*/
typedef struct {
    u32 sw_if_index;
    u32 next_index;
    ip4_address_t src_address;
    ip4_address_t dst_address;
} defend_land_attack_trace_t;

static u8* format_defend_land_attack_trace(u8 *s, va_list *args)
{
    CLIB_UNUSED(vlib_main_t * vm) = va_arg (*args, vlib_main_t *);
    CLIB_UNUSED(vlib_node_t * node) = va_arg (*args, vlib_node_t *);
    defend_land_attack_trace_t *t = va_arg(*args, defend_land_attack_trace_t *);
    s = format(s, "DEFEND_LAND_ATTACK: sw_if_index %d, next index %d, src %U, dst %U",
        t->sw_if_index, t->next_index,
        format_ip4_address, &t->src_address,
        format_ip4_address, &t->dst_address);
    return s;
}

// Packet handler
static uword plugin_test_node_fn(vlib_main_t *vm, vlib_node_runtime_t *node,
        vlib_frame_t * frame)
{
    u32 n_left_from, *from, *to_next;
    plugin_test_next_t next_index;

    from = vlib_frame_vector_args(frame);
    n_left_from = frame->n_vectors;
    // node index: ip4-lookup
    next_index = node->cached_next_index;

    while (n_left_from > 0) {
        u32 n_left_to_next;
        // Get next node vectors
        vlib_get_next_frame(vm, node, next_index, to_next, n_left_to_next);

        while (n_left_from > 0 && n_left_to_next > 0) {
            vlib_buffer_t *b0;
            ip4_header_t *ip0;
            u32 bi0, next0 = PLUGIN_TEST_NEXT_IP4;

            bi0 = to_next[0] = from[0];
            from += 1;
            n_left_from -= 1;
            to_next += 1;
            n_left_to_next -= 1;

            b0 = vlib_get_buffer(vm, bi0);
            // Get IP header
            void *en0 = vlib_buffer_get_current(b0);
            int i = 0;
            // Print the IPv4 header, 20 bytes
            for (i = 0; i < 20; i++)
            {
                printf("%02x ", *(u8*)(en0+i));
            }
            printf("\n");

            // Check if the packet is not an attack
            ip0 = (ip4_header_t *)en0;
            if (ip0->src_address.as_u32 == ip0->dst_address.as_u32) {
                printf("This is a land attack packet!\n");
                // Drop the packet
                next0 = PLUGIN_TEST_DROP;
            }

            if (PREDICT_FALSE( (node->flags && VLIB_NODE_FLAG_TRACE) &&
                (b0->flags && VLIB_BUFFER_IS_TRACED)) ) {
                defend_land_attack_trace_t *t = vlib_add_trace(vm, node, b0, sizeof(*t));
                t->sw_if_index = vnet_buffer(b0)->sw_if_index[VLIB_RX];
                t->next_index = next_index;
                t->src_address = ip0->src_address;
                t->dst_address = ip0->dst_address;
            }
            vlib_validate_buffer_enqueue_x1(vm, node, next_index,
                    to_next, n_left_to_next, bi0, next0);
        }
        // Put the next frame into the node
        vlib_put_next_frame(vm, node, next_index, n_left_to_next);
    }

    return frame->n_vectors;
}

VLIB_REGISTER_NODE (plugin_test_node) = {
    .name = "plugin_test",
    .function = plugin_test_node_fn,
    // The following flag is not mandatory.
    //.flags = VLIB_NODE_FLAG_TRACE_SUPPORTED,
    .vector_size = sizeof(u32),
    .format_trace = format_defend_land_attack_trace,
    .type = VLIB_NODE_TYPE_INTERNAL,
    //.n_errors = ARRAY_LEN(plugin_test_error_strings),
    .n_next_nodes = PLUGIN_TEST_NEXT_N,
    .next_nodes = {
        [PLUGIN_TEST_NEXT_IP4] = "ip4-lookup",
        [PLUGIN_TEST_DROP] = "error-drop",
    },
};
