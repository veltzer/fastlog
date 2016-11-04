'''
dependencies for this project
'''

def populate(d):
    d.packs=[
        'doxygen',
        'libcpufreq-dev',
    ]

def getdeps():
    return [
        __file__, # myself
    ]
