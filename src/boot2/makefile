TARGET = ../boot2.bin

EXTRA_ASFLAGS += -Os -I $(PWD)/h -I $(PWD)/h0
EXTRA_CFLAGS += -Os -I $(PWD)/h -I $(PWD)/h0
EXTRA_CPPFLAGS += -I $(PWD)/h -I $(PWD)/h0
EXTRA_LDFLAGS := -e_boot2 -Tboot2.lds

SRCS = entry.S boot2.c arm.c fault0.S disp.c \
	kmsg.c kmsg_disp.c kmsg_uart.c gpio.c vsprintf.c td043m.c td043m_lcdc.c \
	stdio.c fault.c nand.c nand_scan.c sys.c nand_ids.c nand_calc.c nand_read.c \
	key.c axp182.c

LIBS = bsp.a $(TOPDIR)/lib/libgcc.a $(TOPDIR)/lib/libc.a

DEPS = $(patsubst %.c, %.d,$(patsubst %.S, %.d, $(SRCS)))
OBJS = $(patsubst %.d, %.o,$(DEPS))

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $(EXTRA_LDFLAGS) -o __boot2.o $(OBJS) $(LIBS) 
	$(OBJCOPY) -O binary __boot2.o $(TARGET)
	$(RM) __boot2.o

clean: dummy
	$(RM) $(OBJS) $(TARGET)

include $(TOPDIR)/rules.mk
