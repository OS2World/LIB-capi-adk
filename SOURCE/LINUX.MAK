
CFLAGS = -O2 -W -Wall -I ../c20lib/source -g

CFLAGS := $(CFLAGS) -DTARGET_32BIT

DEMOCFILES = main.c c20msg.c capi.c connect.c contr.c \
	     data.c id.c init.c linuti.c

FAXCFILES = faxmain.c c20msg.c capi.c connect.c contr.c \
	    data.c id.c init.c fax.c linuti.c

DEMOOFILES = $(patsubst %.c,linux/%.o,$(DEMOCFILES))
FAXOFILES = $(patsubst %.c,linux/%.o,$(FAXCFILES))

all: linux/demo linux/faxmain

linux/demo: $(DEMOOFILES) ../c20lib/linux/libc20.a
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

linux/faxmain: $(FAXOFILES) ../c20lib/linux/libc20.a
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

linux/%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@
