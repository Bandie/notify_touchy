# Makefile for notify_touchy
# Author: Bandie Canis 
# Licence: GNU-GPLv3

all: 
	mkdir -p bin
	cc src/notify_touchy.c -o bin/notify_touchy `pkg-config --cflags --libs libnotify` 

install:
	mkdir -p /usr/local/bin
	cp ./bin/notify_touchy /usr/local/bin/
	cp ./systemd/notify_touchy.service /etc/systemd/system
	chown root:root /usr/local/bin/notify_touchy
	chmod 740 /usr/local/bin/notify_touchy
	@printf "Done.\n"
	@printf "You may want to enable and start the service: \"systemctl enable notify_touchy ; systemctl start notify_touchy\"\n"	@printf "The file will be called /verified"

uninstall:
	rm /usr/sbin/local/notify_touchy
	@printf "Done.\n"

clean:
	rm bin/notify_touchy
	rmdir bin

