project('argon2', 'c', default_options: ['c_std=gnu99,c99'])

cc = meson.get_compiler('c')

sources = [
	'lib/argon2.c',
	'lib/core.c',
	'lib/encoding.c',
	'lib/genkat.c',
	'lib/impl-select.c',
	'lib/blake2/blake2.c'
]

aarch64 = host_machine.cpu_family() == 'aarch64'
riscv64 = host_machine.cpu_family() == 'riscv64'
argon2_x86_64_enabled = true
libs = []
if cc.get_id() == 'msvc'
	features = [
		['sse2', []],
		['ssse3', '/arch:SSSE3'],
		['xop', []],
		['avx2', '/arch:AVX2'],
		['avx512f', '/arch:AVX512F']
	]
	foreach x : features
		name = x[0]
		flag = x[1]
		libs += static_library('argon2-'+name, 'arch/x86_64/lib/argon2-'+name+'.c', include_directories: 'lib', pic: true, c_args: ['-DHAVE_'+name.to_upper()] + flag)
	endforeach
elif (not aarch64) and (not riscv64) and cc.sizeof('void*') == 8
	features = [
		['sse2', '-msse2'],
		['ssse3', '-mssse3'],
		['xop', '-mxop'],
		['avx2', '-mavx2'],
		['avx512f', '-mavx512f']
	]
	foreach x : features
		name = x[0]
		flag = x[1]
		
		has_feature = cc.compiles('arch/x86_64/src/test-feature-'+name+'.c')
		used_flag = []
		if not has_feature
			has_feature = cc.compiles('arch/x86_64/src/test-feature-'+name+'.c', args: flag)
			used_flag = flag
		endif
		
		libs += static_library('argon2-'+name, 'arch/x86_64/lib/argon2-'+name+'.c', include_directories: 'lib', pic: true, c_args: ['-DHAVE_'+name.to_upper()] + used_flag)
	endforeach
else
	sources += 'arch/generic/lib/argon2-arch.c'
	argon2_x86_64_enabled = false
endif

if argon2_x86_64_enabled
	sources += 'arch/x86_64/lib/argon2-arch.c'
endif

argon2_lib = static_library('argon2', sources, link_with: libs, include_directories: 'lib')
