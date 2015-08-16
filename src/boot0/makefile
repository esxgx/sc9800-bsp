TARGET = ../boot0.bin
SRCS = entry.S boot0.c ccmu.c ccmu_pw.c dram.c

EXTRA_ASFLAGS += -O1
EXTRA_CFLAGS += -O1 -I $(PWD)/h
EXTRA_CPPFLAGS += -I $(PWD)/h
EXTRA_LDFLAGS := -e_boot0 -Tboot0.lds

DEPS = $(patsubst %.c, %.d,$(patsubst %.S, %.d, $(SRCS)))
OBJS = $(patsubst %.d, %.o,$(DEPS))
LIBS = $(TOPDIR)/lib/libgcc.a

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $(EXTRA_LDFLAGS) -o __boot0.o $(OBJS) $(LIBS)
	$(OBJCOPY) -O binary __boot0.o $(TARGET)
	$(RM) __boot0.o

clean: dummy
	$(RM) $(OBJS) $(TARGET)

include $(TOPDIR)/rules.mk
