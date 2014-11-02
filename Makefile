TARGET = h264player

SRC = main.c context.c h264decoder.c
CFLAGS = -Wall -O3 -std=c11 --sysroot=$(SYSROOT) -I$(SYSROOT)usr/include

MAKEFLAGS += -rR --no-print-directory

DEP_CFLAGS = -MD -MP -MQ $@
LIB_LDFLAGS = -Wl,-soname,$(TARGET) -Wl,--as-needed -L$(SYSROOT) -L$(SYSROOT)lib -L$(SYSROOT)usr -L$(SYSROOT)usr/lib
LIBS = -lvdpau -lavcodec -lavutil

OBJ = $(addsuffix .o,$(basename $(SRC)))
DEP = $(addsuffix .d,$(basename $(SRC)))

MODULEDIR = $(shell pkg-config --variable=moduledir vdpau)

ifeq ($(MODULEDIR),)
MODULEDIR=/usr/lib/vdpau
endif

.PHONY: clean all install

all: $(TARGET)
$(TARGET): $(OBJ)
	$(CC) $(LIB_LDFLAGS) $(LDFLAGS) $(OBJ) $(LIBS) -o $@

clean:
	rm -f $(OBJ)
	rm -f $(DEP)
	rm -f $(TARGET)

install: $(TARGET)
	install -D $(TARGET) $(DESTDIR)$(MODULEDIR)/$(TARGET)

uninstall:
	rm -f $(DESTDIR)$(MODULEDIR)/$(TARGET)

%.o: %.c
	$(CC) $(DEP_CFLAGS) $(LIB_CFLAGS) $(CFLAGS) -c $< -o $@

include $(wildcard $(DEP))
