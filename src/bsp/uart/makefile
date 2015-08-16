TARGET = ../lib/uart.o
SRCS = uart.c

EXTRA_CFLAGS += -I $(PWD)/h
EXTRA_CPPFLAGS += -I $(PWD)/h
EXTRA_LDFLAGS := -o $(TARGET) -S -r -x

DEPS = $(patsubst %.c, %.d,$(patsubst %.S, %.d, $(SRCS)))
OBJS = $(patsubst %.d, %.o,$(DEPS))

$(TARGET): $(OBJS)
	$(LD) $(EXTRA_LDFLAGS) $(OBJS)

clean: dummy
	$(RM) $(OBJS) $(TARGET)

include $(TOPDIR)/rules.mk
