Fast log is a fast system logger, faster than syslog or file based.
It is designed for Linux based, user space, high performance, real time
systems.

Have a look at doc/DESIGN.txt for more info.

Some of the goals of the project:
- do utilities for real time logging and performance monitoring on linux.
	- check that TSC is synchronized between CPUS.
	- tools for measuring performance via TSC.
- syslog logger which matches the syslog API but which is tuned for real time
	(stores all logs in RAM).
	It guarantees order between logs and can add TSC to logs.
- a utility to recover the log from a coredump of a failed process that used the
	logger (Yay!).
- fast log library which logs everything in preallocated RAM and possibly
	a user space tool to peek into that RAM.
	It gives you an API to clear the log so that it doesn't become congested.
- maybe a kernel module to give you contiguous space to log to?

			Mark Veltzer, <mark.veltzer@gmail.com>, 2011
