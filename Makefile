CC = i686-w64-mingw32-gcc-win32

.PHONY: all clean

all: bmprnt-cli.exe

bmprnt-cli.exe: main.c
	$(CC) -static -mwindows -o $@ $<

clean:
	rm bmprnt-cli.exe
