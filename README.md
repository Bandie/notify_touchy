# notify\_touchy


## Purpose
A fork of [i3status-touchy](https://github.com/Bandie/i3status-touchy). 
A service which checks the return code of a program/script and notifies you if it has a bad return code. 
In i3status-touchy there will be a file created. This doesn't happen here anymore. Instead it uses libnotify.


## Installation
You need gcc or something similar. There is a *not finished* systemd servicefile included. 
To compile and install it you may want to do the following within this project directory:

```
make
sudo make install
```


## Syntax

```
notify_touchy ( arguments )

It checks if a program has a specific return code within some time. If it has not a notification will appear.

Arguments:

-h, --help                         Shows this here.
-p, --program [program or script]  Executes and checks this program for the return code. (Required)
-r, --return [return code]         Listen to this return code. (Default: 0)
-s, --sleep [seconds]              Interval of when the next check should start. (Default: 20)
-v, --verbose                      Show some unnecessary stuff.

```

`notify_touchy --help` will tell you the same btw.


