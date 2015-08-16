.EXPORT_ALL_VARIABLES:

TOPDIR	:= $(PWD)

all: bsp boot0 boot2

bsp: dummy
	$(MAKE) -C bsp

boot0: dummy
	$(MAKE) -C boot0

boot2: dummy
	$(MAKE) -C boot2

clean: dummy
	$(MAKE) -C bsp clean
	$(MAKE) -C boot2 clean
	$(MAKE) -C boot0 clean


include $(TOPDIR)/rules.mk
