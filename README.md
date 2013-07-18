hook_bind_lo 
=====================
hook_bind_lo - bind() function hook sample

compile
====================
<pre>
  $ git clone https://github.com/yoggy/hook_bind_lo.git
  $ cd hook_bind_lo
  $ make
</pre>

how to use
=====================
IPv6
<pre>
  $ ./hook_bind_lo 23456 nc -l -p 12345 -6
  hook_bind_lo.so : [INFO] HOOK_BIND_LO_PORT=23456
  hook_bind_lo.so : [INFO] hook bind() : addr=::1, port=23456
</pre>

IPv4
<pre>
  $ ./hook_bind_lo 23456 nc -l -p 12345 -6
  hook_bind_lo.so : [INFO] HOOK_BIND_LO_PORT=23456
  hook_bind_lo.so : [INFO] hook bind() : addr=127.0.0.1, port=23456
</pre>
