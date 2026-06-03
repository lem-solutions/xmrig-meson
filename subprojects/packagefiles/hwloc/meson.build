project('hwloc', 'c', version: '2.12.1')


mod = import('unstable-external_project')

p = mod.add_project('configure',
	configure_options : [
		'--disable-shared',
		'--enable-static',
		'--disable-io',
		'--disable-libudev',
		'--disable-libxml2'
	]
)

hwloc_dep = p.dependency('hwloc')
