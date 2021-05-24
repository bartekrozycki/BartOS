# BartOS

Special thanks for
* [OSDev](https://wiki.osdev.org/Expanded_Main_Page)
* [OSDev Forum](https://forum.osdev.org/)

## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Compile](#setup)
* [Run using bochs](#Bochs)
* [Run using qemu](#Qemu)
* [Run on real hardware](#Real-Hardware)

## General info
BartOS is simple operating system made as university project.

* [x] Memory detection
* [x] Memory mapping
* [x] Virtual memory
* [x] Heap & Heap Allocator
* [x] Multi-Tasking
* [x] Keyboard driver
* [x] Serial Driver
* [x] VGA 80x25 Driver

## Technologies
Project is created with:
* GNU GCC-I686
* NASM
* Makefile

## Setup
To run this project [GCC Cross Compiler](https://wiki.osdev.org/GCC_Cross-Compiler) is required 

Follow this link to install it
```https://wiki.osdev.org/GCC_Cross-Compiler```

Compile project
```
$ make all
```

### Emulator
System could be emulated using [Bochs](https://wiki.osdev.org/Bochs) or [Qemu](https://wiki.osdev.org/Qemu)
#### Bochs
```shell
$ make run-terminal-boch
```
#### Qemu
```shell
$ make run-terminal-qemu
```
### Real Hardware

```shell
$ sudo dd if=os.iso of=/dev/SDX && sync
```
where **SDX** is USB block name for example **SDB**
