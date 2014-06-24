# dwmsd
A simple dwm status daemon, for the dwm window manager

### dwmsd: dwm status daemon
dwmsd listens on 127.0.0.1:57475, and passes strings to XStoreName

### dwmsc: dwm status client
dwmsc passes strings to 127.0.0.1:57475

### Convenience
* you can: `uptime | dwmsc` or `dwmsc "hello world"`

### Thanks to
* http://dwm.suckless.org
* dwmstatus on suckless
* AndiDog on stackoverflow
* http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
* github user: 0xACE
