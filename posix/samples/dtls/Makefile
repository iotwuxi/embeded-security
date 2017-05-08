
# To compile on SunOS: add "-lsocket -lnsl" to LDFLAGS
# To compile with PKCS11: add "-lpkcs11-helper" to LDFLAGS
CFLAGS	?= -O2
WARNING_CFLAGS ?= -Wall -W -Wdeclaration-after-statement
LDFLAGS ?=

LOCAL_CFLAGS = $(WARNING_CFLAGS) -I.
LOCAL_LDFLAGS = -L.					\
				-lmbedtls			\
				-lmbedx509			\
				-lmbedcrypto

APPS = coaps_client coaps_server

.SILENT:

.PHONY: all clean list

all: $(APPS)

coaps_client: coaps_client.c 
	echo "  CC    coaps_client.c"
	$(CC) $(LOCAL_CFLAGS) $(CFLAGS) coaps_client.c  $(LOCAL_LDFLAGS) $(LDFLAGS) -o $@

coaps_server: coaps_server.c 
	echo "  CC    coaps_server.c"
	$(CC) $(LOCAL_CFLAGS) $(CFLAGS) coaps_server.c  $(LOCAL_LDFLAGS) $(LDFLAGS) -o $@

clean:
	rm -f $(APPS)

list:
	echo $(APPS)
