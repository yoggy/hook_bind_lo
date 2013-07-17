hook_bind_lo 
=====================
hook_bind_lo.cpp - bind() function hook sample

how to use
=====================
IPv6
<pre>
  $ HOOK_BIND_LO_PORT=23456 LD_PRELOAD=./hook_bind_lo.so nc -l -p 12345 -6
  hook_bind_lo.so : [INFO] HOOK_BIND_LO_PORT=23456
  hook_bind_lo.so : [INFO] hook bind() : addr=::1, port=23456
</pre>

IPv4
<pre>
  $ make && HOOK_BIND_LO_PORT=23456 LD_PRELOAD=./hook_bind_lo.so nc -l -p 12345 -4
  hook_bind_lo.so : [INFO] HOOK_BIND_LO_PORT=23456
  hook_bind_lo.so : [INFO] hook bind() : addr=127.0.0.1, port=23456
</pre>
