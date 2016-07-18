'''
project definitions for templar
'''

def populate(d):
	d.project_github_username='veltzer'
	d.project_name='fastlog'
	d.project_website='https://{project_github_username}.github.io/{project_name}'.format(**d)
	d.project_website_source='https://github.com/{project_github_username}/{project_name}'.format(**d)
	d.project_website_git='git://github.com/{project_github_username}/{project_name}.git'.format(**d)
	d.project_paypal_donate_button_id='ZA2DTK4LL9LEC'
	d.project_google_analytics_tracking_id='UA-80897396-1'
	d.project_long_description='Fastlog is a C/C++ library for fast/realtime logging'
	# keywords to put on html pages or for search, dont put the name of the project or my details
	# as they will be added automatically...
	d.project_keywords=[
		'realtime',
		'rt',
		'logging',
		'syslog',
		'syslog-ng',
		'rsyslog',
		'fastlog',
	]
	d.project_license='GPLV3'
	d.project_year_started='2010'
	d.project_description='''
Fast log is a fast system logger, faster than syslog or file based.
It is designed for Linux based, user space, high performance, real time
systems.

Have a look at doc/DESIGN.txt for more info.

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
	It gives you an API to clear the log so that it doesn't become congested.
* maybe a kernel module to give you contiguous space to log to?
'''.strip().format(**d)

	# deb
	d.deb_package=False

def getdeps():
	return [
		__file__, # myself
	]
