//
//	hook_bind_lo.cpp - bind() function hook sample
//
//  how to use:
//    
//    IPv6
//        $ HOOK_BIND_LO_PORT=23456 LD_PRELOAD=./hook_bind_lo.so nc -l -p 12345 -6
//        hook_bind_lo.so : [INFO] HOOK_BIND_LO_PORT=23456
//        hook_bind_lo.so : [INFO] hook bind() : addr=::1, port=23456
//
//    IPv4
//        $ make && HOOK_BIND_LO_PORT=23456 LD_PRELOAD=./hook_bind_lo.so nc -l -p 12345 -4
//        hook_bind_lo.so : [INFO] HOOK_BIND_LO_PORT=23456
//        hook_bind_lo.so : [INFO] hook bind() : addr=127.0.0.1, port=23456
//    
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

// listen port (get value from HOOK_BIND_LO_PORT)
int port;

// original function
static int (*original_bind)(int sockfd, const struct sockaddr *addr, socklen_t addrlen) = NULL;

// for logging
#define LOG_E(...)                             \
	do{                                        \
		fprintf(stderr, "hook_bind_lo.so : [ERROR] "); \
		fprintf(stderr, __VA_ARGS__);          \
		fprintf(stderr, "\n");                 \
	}while(0);

#define LOG_I(...)                             \
	do{                                        \
		fprintf(stderr, "hook_bind_lo.so : [INFO] "); \
		fprintf(stderr, __VA_ARGS__);          \
		fprintf(stderr, "\n");                 \
	}while(0);

#if defined(RTLD_NEXT)
#define REAL_LIBC RTLD_NEXT
#else
#define REAL_LIBC ((void *) -1L)
#endif

static void __attribute__ ((constructor)) _constructor()
{
	original_bind  = (int(*)(int, const struct sockaddr *, socklen_t)) dlsym(REAL_LIBC, "bind");

	char *port_str = getenv("HOOK_BIND_LO_PORT");
	if (port_str == NULL) {
		LOG_E("HOOK_BIND_LO_PORT is not defined...");
		abort();
	}

	port = atoi(port_str);
	if (port < 0 || 65535 < port) {
		LOG_E("HOOK_BIND_LO_PORT range is 0-65535...");
	}

	LOG_I("HOOK_BIND_LO_PORT=%d", port);
}

static void __attribute__ ((destructor)) _destructor()
{
}

int bind_localhost_ipv4(int sockfd);
int bind_localhost_ipv6(int sockfd);

// hook function using LD_PRELOAD
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int rv;

	switch(addr->sa_family) {
		case AF_INET:
			rv = bind_localhost_ipv4(sockfd);
			break;
		case AF_INET6:
			rv = bind_localhost_ipv6(sockfd);
			break;
		default:
			LOG_E("unknown address family...addr->sa_family=%d", addr->sa_family);
			break;
	}

	return rv;
}

int bind_localhost_ipv4(int sockfd)
{
	LOG_I("hook bind() : addr=127.0.0.1, port=%d", port);

	struct sockaddr_in ai;
	memset(&ai, 0, sizeof(struct sockaddr_in));
	ai.sin_family      = AF_INET;
	ai.sin_addr.s_addr = inet_addr("127.0.0.1");
	ai.sin_port        = htons(port);

	return original_bind(sockfd, (struct sockaddr*)&ai, sizeof(struct sockaddr_in));
}

int bind_localhost_ipv6(int sockfd)
{
	LOG_I("hook bind() : addr=::1, port=%d", port);

	struct sockaddr_in6 ai;
	memset(&ai, 0, sizeof(struct sockaddr_in6));
	ai.sin6_family           = AF_INET6;
	ai.sin6_addr.s6_addr[15] = 1; // ::1
	ai.sin6_port             = htons(port);

	return original_bind(sockfd, (struct sockaddr*)&ai, sizeof(struct sockaddr_in6));
}
