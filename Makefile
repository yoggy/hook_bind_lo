TARGET=hook_bind_lo.so

hook_bind_lo.so: hook_bind_lo.c
	g++ -Wall -fPIC -shared -o hook_bind_lo.so hook_bind_lo.c -ldl

clean:
	rm -rf hook_bind_lo.so

