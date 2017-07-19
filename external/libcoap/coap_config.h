
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <lwip/opt.h>
#include <lwip/debug.h>
#include <lwip/def.h> /* provide ntohs, htons */

#define WITH_LWIP  1
#define COAP_RESOURCES_NOHASH

#define assert(x) LWIP_ASSERT("CoAP assert failed", x)

/* Define to 1 if you have the <arpa/inet.h> header file. */
// #define HAVE_ARPA_INET_H 0

/* Define to 1 if you have the <assert.h> header file. */
#define HAVE_ASSERT_H 1

/* Define to 1 if you have the <CUnit/CUnit.h> header file. */
/* #undef HAVE_CUNIT_CUNIT_H */

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the `getaddrinfo' function. */
#define HAVE_GETADDRINFO 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define if the system has libcunit */
/* #undef HAVE_LIBCUNIT */

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if you have the `malloc' function. */
// #define HAVE_MALLOC 0


/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET 1

/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 1


/* Define to 1 if you have the <netinet/in.h> header file. */
// #define HAVE_NETINET_IN_H 0


/* Define to 1 if you have the `select' function. */
#define HAVE_SELECT 1

/* Define to 1 if you have the `socket' function. */
#define HAVE_SOCKET 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strcasecmp' function. */
#define HAVE_STRCASECMP 1

/* Define to 1 if you have the <strings.h> header file. */
// #define HAVE_STRINGS_H 0

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strnlen' function. */
#define HAVE_STRNLEN 1

/* Define to 1 if you have the `strrchr' function. */
#define HAVE_STRRCHR 1


/* Define to 1 if you have the <syslog.h> header file. */
// #define HAVE_SYSLOG_H 0

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 0

/* Define to 1 if you have the <sys/stat.h> header file. */
// #define HAVE_SYS_STAT_H 0

/* Define to 1 if you have the <sys/time.h> header file. */
// #define HAVE_SYS_TIME_H 0

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/unistd.h> header file. */
// #define HAVE_SYS_UNISTD_H 0

/* Define to 1 if you have the <time.h> header file. */
// #define HAVE_TIME_H 0

/* Define to 1 if you have the <unistd.h> header file. */
// #define HAVE_UNISTD_H 0

#endif