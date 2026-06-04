headers_ghostrider = [
	'sph_types.h',
	'sph_blake.h',
	'sph_bmw.h',
	'sph_cubehash.h',
	'sph_echo.h',
	'sph_fugue.h',
	'sph_groestl.h',
	'sph_hamsi.h',
	'sph_jh.h',
	'sph_keccak.h',
	'sph_luffa.h',
	'sph_sha2.h',
	'sph_shabal.h',
	'sph_shavite.h',
	'sph_simd.h',
	'sph_skein.h',
	'sph_whirlpool.h',
	'ghostrider.h'
]

sources_ghostrider_c = [
	'sph_blake.c',
	'sph_bmw.c',
	'sph_cubehash.c',
	'sph_echo.c',
	'sph_fugue.c',
	'sph_groestl.c',
	'sph_hamsi.c',
	'sph_jh.c',
	'sph_keccak.c',
	'sph_luffa.c',
	'sph_shabal.c',
	'sph_shavite.c',
	'sph_simd.c',
	'sph_sha2.c',
	'sph_skein.c',
	'sph_whirlpool.c'
]

g_def_args = []
foreach def : xmrig_defines
	g_def_args += '-D' + def
endforeach

if c.get_id() == 'msvc'
	if get_option('buildtype') == 'release'
		ghostrider_compile_args = g_def_args + ['/O1', '/Oi', '/Os']
	else
		ghostrider_compile_args = g_def_args
	endif
	ghostrider_int_lib = static_library('ghostrider_internal', headers_ghostrider + sources_ghostrider_c, c_args: g_def_args  + ghostrider_compile_args)
elif c.get_id() == 'gcc' or c.get_id() == 'clang'
	ghostrider_int_libs = []
	src_norm = []
	foreach s : sources_ghostrider_c
		if s == 'sph_jh.c'
			ghostrider_int_libs += static_library('ghostrider_internal_jh', headers_ghostrider + s, c_args: g_def_args + ['-Os', '-fno-tree-vrp'])
		elif s == 'sph_luffa.c'
			ghostrider_int_libs += static_library('ghostrider_internal_luffa', headers_ghostrider + s, c_args: g_def_args + ['-Os', '-Wno-unused-const-variable'])
		else
			src_norm += s
		endif
	endforeach
	ghostrider_int_lib = static_library('ghostrider_internal', headers_ghostrider + src_norm, link_with: ghostrider_int_libs, c_args: g_def_args + ['-Os'])
else
	ghostrider_int_lib = static_library('ghostrider_internal', headers_ghostrider + sources_ghostrider_c, c_args: g_def_args)
endif

ghostrider_lib = static_library('ghostrider',
	headers_ghostrider + 'ghostrider.cpp',
	link_with: ghostrider_int_lib,
	cpp_args: g_def_args,
	dependencies: [uv_dep, hwloc_dep]
)
