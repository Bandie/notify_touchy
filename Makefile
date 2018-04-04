# Makefile for notify_touchy
# Author: Bandie Canis 
# Licence: GNU-GPLv3

all: 
	mkdir -p bin
	cc --std=c99 src/notify_touchy.c -o bin/notify_touchy `pkg-config --cflags --libs libnotify` 

install:
	mkdir -p /usr/local/bin
	cp ./bin/notify_touchy /usr/local/bin/
	chown root:root /usr/local/bin/notify_touchy
	chmod 740 /usr/local/bin/notify_touchy
	@printf "Done.\n"
	@printf "You may want to configure a service for this program. Look into the systemd directory for a systemd example.\n"

uninstall:
	rm /usr/sbin/local/notify_touchy
	@printf "Done.\n"

clean:
	rm bin/notify_touchy
	rmdir bin

test:
	@printf "What to test?\n"
