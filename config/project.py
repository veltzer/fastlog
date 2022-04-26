"""
project definitions for fastlog
"""
import datetime
import config.general

project_github_username="veltzer"
project_name="fastlog"
project_website=f"https://{project_github_username}.github.io/{project_name}"
project_website_source=f"https://github.com/{project_github_username}/{project_name}"
project_website_git=f"git://github.com/{project_github_username}/{project_name}.git"
project_paypal_donate_button_id="ZA2DTK4LL9LEC"
project_google_analytics_tracking_id="UA-80897396-1"
project_google_analytics_snipplet = f"""<script type="text/javascript">
(function(i,s,o,g,r,a,m){{i["GoogleAnalyticsObject"]=r;i[r]=i[r]||function(){{
(i[r].q=i[r].q||[]).push(arguments)}},i[r].l=1*new Date();a=s.createElement(o),
m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)
}})(window,document,"script","https://www.google-analytics.com/analytics.js","ga");

ga("create", "{project_google_analytics_tracking_id}", "auto");
ga("send", "pageview");

</script>"""
project_long_description="Fastlog is a C/C++ library for fast/realtime logging"
# keywords to put on html pages or for search, dont put the name of the project or my details
# as they will be added automatically...
project_keywords=[
"realtime",
"rt",
"logging",
"syslog",
"syslog-ng",
"rsyslog",
"fastlog",
]
project_license="GPLV3"
project_year_started=2010
project_description=f"""
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
It gives you an API to clear the log so that it doesn"t become congested.
* maybe a kernel module to give you contiguous space to log to?
"""

# deb
deb_package=False

project_paypal_donate_button_id = "0"
project_paypal_donate_button_snipplet = f"""<form action="https://www.paypal.com/cgi-bin/webscr"
    method="post" target="_top">
<input type="hidden" name="cmd" value="_s-xclick">
<input type="hidden" name="hosted_button_id" value="{project_google_analytics_tracking_id}">
<input type="image" src="https://www.paypalobjects.com/en_US/IL/i/btn/btn_donateCC_LG.gif" name="submit"
alt="PayPal - The safer, easier way to pay online!">
<img alt="" border="0" src="https://www.paypalobjects.com/en_US/i/scr/pixel.gif" width="1" height="1">
</form>"""

import datetime
project_year_now=str(datetime.datetime.now().year)
project_copyright_years=", ".join(
    map(str, range(int(project_year_started), int(project_year_now)+1)))
project_copyright_years_short = f"{project_year_started} - {project_year_now}"
