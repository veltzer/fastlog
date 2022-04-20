import datetime
import subprocess
import os
import socket

general_current_folder = os.path.basename(os.getcwd())
general_current_year = datetime.date.today().year
general_homedir = os.path.expanduser('~')
general_hostname = socket.gethostname()
general_domain_name = subprocess.check_output(['hostname', '--domain']).decode().rstrip()
