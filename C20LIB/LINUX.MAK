
vpath %.c source

CFLAGS := $(CFLAGS) -DTARGET_32BIT -DCPROT -DCPROT_LINUX -g -Wall

CFILES=clinux.c func.c msg.c gcmsglin.c

OFILES=$(patsubst %.c,linux/%.o,$(CFILES))

LIB=linux/libc20.a

all: $(LIB)

$(LIB): $(OFILES)
	$(RM) $@
	$(AR) cq $@ $^

linux/%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@
