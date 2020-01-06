'''
dependencies for this project
'''

def populate(d):
    d.packs=[
        'doxygen',
        'libcpufreq-dev',
    ]

def get_deps():
    return [
        __file__, # myself
    ]
