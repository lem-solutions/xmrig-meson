project('hwloc', 'c', version: '2.12.1')

# Upstream provides cmake as buildsystem for windows, but we cannot use it since CMakeFLists.txt is not in the project root.
# The configure script on the other hand does not support targets such as x86_64-pc-windows, only unixy systems and cygwin/mingw
if host_machine.system() == 'windows'
	error('Building hwloc as a meson subproject for native windows is currently not supported due to quirks in the upstream buildsystem. Try using a Cygwin/mingw environment or installing hwloc manually so meson doesn\'t need to use this subproject. Alternativly you can disable hwloc entirely.')
endif

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
