.EXPORT_ALL_VARIABLES:

EXTRA_CFLAGS += -Os -I $(PWD)/h
EXTRA_ASFLAGS += -Os
EXTRA_CPPFLAGS += -I $(PWD)/h

OBJS = lib/ccmu.o lib/de.o lib/lcdvtc.o lib/lradc.o lib/nfc.o lib/pic.o lib/pio.o lib/uart.o lib/twi.o
TARGET = lib/bsp.a

$(TARGET): $(OBJS)
	$(AR)	cqs $(TARGET) $(OBJS)

lib/%.o : dummy
	$(MAKE) -C $(basename $(@F))

clean:
	for dir in $(basename $(notdir $(OBJS))); do \
		$(MAKE) -C $$dir clean; \
       done
	$(RM) $(TARGET) 

include $(TOPDIR)/rules.mk
