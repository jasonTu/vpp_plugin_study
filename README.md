# vpp_plugin_study

## How to enable trace for this node
```
plugin test host-vpp1 enable
# NOTE: need keep the command order
trace filter include plugin_test 10
trace add af-packet-input 10
# Then tigger some PINGs, and then check the trace log
show trace
```
