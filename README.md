## fastlog

version: 0.0.1

description: Fastlog is a C/C++ library for fast/realtime logging

website: https://veltzer.github.io/fastlog

## build

![build](https://github.com/veltzer/fastlog/workflows/build/badge.svg)


Fast log is a fast system logger, faster than syslog or file based.
It is designed for Linux based, user space, high performance, real time
systems. Have a look at doc/DESIGN.txt for more info.

Some of the goals of the project:
---------------------------------
* do utilities for real time logging and performance monitoring on linux.
- check that TSC is synchronized between CPUS.
- tools for measuring performance via TSC.
* syslog logger which matches the syslog API but which is tuned for real time
(stores all logs in RAM).
It guarantees order between logs and can add TSC to logs.
* a utility to recover the log from a coredump of a failed process that used the
logger (Yay!).
* fast log library which logs everything in preallocated RAM and possibly
a user space tool to peek into that RAM.
It gives you an API to clear the log so that it doesn"t become congested.
* maybe a kernel module to give you contiguous space to log to?

chat with me at [![gitter](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/veltzer/mark.veltzer)

Mark Veltzer, Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022
