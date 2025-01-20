#include <vnet/plugin/plugin.h>

#include <json_plugin/plugin.h>

plugin_test_main_t plugin_test_main;

int plugin_test_enable_disable(u32 sw_if_index, int is_enable) {
    vnet_sw_interface_t *sw;
    int ret = 0;

    if (pool_is_free_index(plugin_test_main.vnet_main->interface_main.sw_interfaces,
                sw_if_index))
        return VNET_API_ERROR_INVALID_SW_IF_INDEX;

    /* Not a physical port? */
    sw = vnet_get_sw_interface(plugin_test_main.vnet_main, sw_if_index);
    if (sw->type != VNET_SW_INTERFACE_TYPE_HARDWARE)
        return VNET_API_ERROR_INVALID_SW_IF_INDEX;

    /* "ipv4-unicast" is an archive of features */
    vnet_feature_enable_disable("ip4-unicast", "plugin_test",
            sw_if_index, is_enable, 0, 0);

    return ret;
}

static clib_error_t* plugin_test_enable_disable_cmd_fn(vlib_main_t *vm,
        unformat_input_t *input, vlib_cli_command_t *cmd) {
    u32 sw_if_index = ~0;
    int is_enable = 1;

    while (unformat_check_input(input) != UNFORMAT_END_OF_INPUT) {
        if (unformat(input, "disable"))
            is_enable = 0;
        else if (unformat(input, "enable"))
            is_enable = 1;
        /* Get the interface index */
        else if (unformat(input, "%U", unformat_vnet_sw_interface,
                    plugin_test_main.vnet_main, &sw_if_index))
            ;
        else
            break;
    }
    if (sw_if_index == ~0)
        return clib_error_return(0, "Please specify an interface...");

    plugin_test_enable_disable(sw_if_index, is_enable);

    return 0;
}

static clib_error_t *plugin_test_init(vlib_main_t *vm) {
    plugin_test_main.vnet_main = vnet_get_main();

    return 0;
}

VLIB_CLI_COMMAND (ping_command, static) = {
    .path = "plugin test",
    .short_help = "plugin test <interface-name> [enable/disable]",
    .function = plugin_test_enable_disable_cmd_fn,
};

VLIB_PLUGIN_REGISTER () = {
    .version = PLUGIN_TEST_VER,
    .description = "Jason (Test Plugin)",
};

VLIB_INIT_FUNCTION(plugin_test_init);

VNET_FEATURE_INIT(plugin_test, static) = {
    .arc_name = "ip4-unicast",
    .node_name = "plugin_test",
    .runs_before = VNET_FEATURES("ip4-lookup"),
};
