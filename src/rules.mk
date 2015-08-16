
#
# False targets.
#
.PHONY: dummy

#
# Special variables which should not be exported
#
#unexport EXTRA_ASFLAGS
#unexport EXTRA_CFLAGS
#unexport EXTRA_LDFLAGS
#unexport EXTRA_ARFLAGS

PWD = $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)

CROSS_COMPILE = arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy

MAKE = make

CPPFLAGS = -I $(TOPDIR)/h
CFLAGS = -I $(TOPDIR)/h -march=armv5te
ASFLAGS = -I $(TOPDIR)/h -march=armv5te

%.o:%.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

%.o:%.S
	$(CC) -D__ASSEMBLY__ $(ASFLAGS) $(EXTRA_ASFLAGS) -c -o $@ $<

%.d: %.c
	@set -e;\
	$(CC) -MM $(CPPFLAGS) $(EXTRA_CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

%.d: %.S
	@set -e; $(RM) $@;\
	$(CC) -MM $(CPPFLAGS) $(EXTRA_CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

#
# include dependency files they exist
#
ifeq ($(DEPS),$(wildcard $(DEPS)))
include $(DEPS)
endif

#
# A rule to do nothing
#
dummy:
	
