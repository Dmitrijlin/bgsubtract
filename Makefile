# Source: http://stackoverflow.com/questions/9870297/makefile-to-compile-opencv-code-in-c-on-ubuntu-linux
# usage: make filename

CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

MACHINE=`uname -m`
ifeq ($(MACHINE), armv6l)
	CFLAGS += -O2 -pipe -mcpu=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard
endif

% : %.cpp
	g++ $(CFLAGS) $< -o $@ $(LIBS)
