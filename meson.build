project('xmrig', ['cpp', 'c'], meson_version: '>=1.2', default_options: {
	'warning_level': '1',
	'strip': get_option('buildtype') == 'release',
	'cpp_std': 'c++11',
	'c_std': 'c99'
})

xmrig_deps = []
xmrig_link_with = []
cmake = import('cmake')
cpp = meson.get_compiler('cpp')
c = meson.get_compiler('c')

xmrig_64_bit = cpp.sizeof('void*') == 8
os_win = host_machine.system() == 'windows' or host_machine.system() == 'cygwin'
arm = host_machine.cpu_family() == 'aarch64' or host_machine.cpu_family() == 'arm'
riscv =  host_machine.cpu_family() == 'risc64' or host_machine.cpu_family() == 'risc32'
disable_adl = not (os_win or (host_machine.system() == 'linux'))
adl = get_option('adl').disable_if(disable_adl).enable_auto_if(true).enabled()

dmi_oscheck = (not os_win) and (host_machine.system() != 'linux') and (host_machine.system() != 'freebsd')
dmi_maccheck = not (host_machine.system() == 'darwin' and host_machine.subsystem() == 'macos' and not arm)
dmi = get_option('dmi').disable_if(dmi_oscheck and dmi_maccheck).enable_auto_if(true).enabled()
msr = get_option('msr').disable_if(arm or riscv or (not xmrig_64_bit) or not (os_win or host_machine.system() == 'linux')).enable_auto_if(true).enabled()

# We always use our subprojects for theses libraries, some of them are modified.
if get_option('http')
	sub = subproject('llhttp')
	xmrig_deps += sub.get_variable('llhttp_dep')
endif
if get_option('hwloc')
	sub = subproject('hwloc')
	hwloc_dep = sub.get_variable('hwloc_dep')
	xmrig_deps += hwloc_dep
endif
if  get_option('argon2')
	sub = subproject('argon2')
	argon2_dep = sub.get_variable('argon2_dep')
	xmrig_deps += argon2_dep
endif
if get_option('kawpow')
	ethash_dep = declare_dependency(
		sources: [
			'src/3rdparty/libethash/data_sizes.h',
			'src/3rdparty/libethash/endian.h',
			'src/3rdparty/libethash/ethash.h',
			'src/3rdparty/libethash/ethash_internal.h',
			'src/3rdparty/libethash/fnv.h',
			'src/3rdparty/libethash/ethash_internal.c',
			'src/3rdparty/libethash/keccakf800.c',
		],
		include_directories: include_directories('src/3rdparty/libethash')
	)
	xmrig_deps += ethash_dep
endif
if host_machine.system() != 'darwin'
	sub = subproject('opencl')
	xmrig_deps += sub.get_variable('opencl_dep')
endif

# For these dependencies, we try to find one the normal way and fall back to our
# subprojects if they are not available.
openssl_dep = dependency('openssl', version: '>=3.0.8', required: get_option('tls'), default_options:['default_library=static'])
if openssl_dep.found()
	xmrig_deps += openssl_dep
endif

add_project_dependencies(declare_dependency(include_directories: include_directories('src')),language: ['c', 'cpp'])

xmrig_defines = [
	'RAPIDJSON_WRITE_DEFAULT_FLAGS=6', # rapidjson::kWriteNanAndInfFlag | rapidjson::kWriteNanAndInfNullFlag
	
	'XMRIG_MINER_PROJECT',
	'XMRIG_JSON_SINGLE_LINE_ARRAY',
	'__STDC_FORMAT_MACROS',
	'UNICODE',
	'FILE_OFFSET_BITS=64'
]
arm8_cpp_flags = []
compile_flags_for_files = {}

# cmake/cpu.cmake

if xmrig_64_bit
	xmrig_defines += 'XMRIG_64_BIT'
endif

if riscv
	xmrig_defines += 'XMRIG_RISCV'
	rvarch = 'rv64gc' # seems wrong for risc32...
	
	if (not meson.is_cross_build()) or meson.has_exe_wrapper()
		add_languages('linearasm', native: true)
		asm = get_compiler('linearasm', native: true)
		rv_v_res = asm.run('src/crypto/randomx/tests/riscv64_vector.s', args: ['-march=rv64gcv'])
		rv_zicbop_res = asm.run('src/crypto/randomx/tests/riscv64_zicbop.s', args: ['-march=rv64gc_zicbop'])
		rv_zba_res = asm.run('src/crypto/randomx/tests/riscv64_zba.s', args: ['-march=rv64gc_zba'])
		rv_zbb_res = asm.run('src/crypto/randomx/tests/riscv64_zbb.s', args: ['-march=rv64gc_zbb'])
		rv_zvkb_res = asm.run('src/crypto/randomx/tests/riscv64_zvkb.s', args: ['-march=rv64gc_zvkb'])
		rv_zvkned_res = asm.run('src/crypto/randomx/tests/riscv64_zvkned.s', args: ['-march=rv64gc_zvkned'])
		rv_v = rv_v_res.compiled() and rv_v_res.returncode() == 0
		rv_zicbop = rv_zicbop_res.compiled() and rv_zicbop_res.returncode() == 0
		rv_zba = rv_zba_res.compiled() and rv_zba_res.returncode() == 0
		rv_zbb = rv_zbb_res.compiled() and rv_zbb_res.returncode() == 0
		rv_zvkb = rv_zvkb_res.compiled() and rv_zvkb_res.returncode() == 0
		rv_zvkned = rv_zvkned_res.compiled() and rv_zvkned_res.returncode() == 0
		
		if rv_v
			rvarch += 'v'
		endif
		if rv_zicbop
			rvarch += '_zicbop'
		endif
		if rv_zba
			rvarch += '_zba'
		endif
		if rv_zbb
			rvarch += '_zbb'
		endif
		if rv_zvkb
			rvarch += '_zvkb'
		endif
		if rv_zvkned
			rvarch += '_zvkned'
		endif
	endif
	
	massage('Using -march=' + rvarch)
endif

if xmrig_64_bit and host_machine.cpu() == 'x86_64'
	xmrig_defines += 'RAPIDJSON_SSE2'
endif

# Seems wired to me, the the original cmake build script does that...
vaes = get_option('vaes').disable_if(host_machine.cpu() != 'x86_64' or (not cpp.has_argument('-mavx2')) or (not cpp.has_argument('-mvaes'))).enabled()
avx2 = get_option('avx2').disable_if(host_machine.cpu() != 'x86_64').enabled()
sse41 = get_option('sse41').disable_if(host_machine.cpu() != 'x86_64').enabled()

if host_machine.cpu_family() == 'aarch64'
	xmrig_defines += 'XMRIG_ARM=8'
	
	if(cpp.get_id() == 'clang' or cpp.get_id() == 'gcc')
		if cpp.has_argument('-march=armv8-a+crypto')
			xmrig_defines += 'XMRIG_ARM_CRYPTO'
			arm8_cpp_flags += '-march=armv8-a+crypto'
		else
			arm8_cpp_flags += '-march=armv8-a'
		endif
	endif
elif host_machine.cpu_family() == 'arm'
	xmrig_defines += 'XMRIG_ARM=7'
endif

if sse41
	xmrig_defines += 'XMRIG_FEATURE_SSE4_1'
endif
if avx2
	xmrig_defines += 'XMRIG_FEATURE_AVX2'
endif

# cmake/os.cmake

if os_win
	xmrig_defines += 'WIN32'
	xmrig_defines += 'XMRIG_OS_WIN'
elif host_machine.system() == 'darwin'
	xmrig_defines += 'XMRIG_OS_APPLE'
	if host_machine.subsystem() == 'macos'
		xmrig_defines += 'XMRIG_OS_MACOS'
	elif host_machine.subsystem() == 'iso' or host_machine.subsystem() == 'iso-simulator'
		xmrig_defines += 'XMRIG_OS_IOS'
	endif
else
	xmrig_defines += 'XMRIG_OS_UNIX'
	if host_machine.system() == 'android'
		xmrig_defines += 'XMRIG_OS_ANDROID'
	elif  host_machine.system() == 'linux'
		xmrig_defines += 'XMRIG_OS_LINUX'
	elif  host_machine.system() == 'freebsd'
		xmrig_defines += 'XMRIG_OS_FREEBSD'
	elif  host_machine.system() == 'openbsd'
		xmrig_defines += 'XMRIG_OS_OPENBSD'
	elif  host_machine.system() == 'haiku'
		xmrig_defines += 'XMRIG_OS_HAIKU'
	endif
endif

if get_option('secure-jit').enable_if(host_machine.system() == 'darwin' and arm).disable_auto_if(true).enabled()
	xmrig_defines += XMRIG_SECURE_JIT
endif

subdir('src/base')
subdir('src/backend')

headers = [
	'src/App.h',
	'src/core/config/Config_default.h',
	'src/core/config/Config_platform.h',
	'src/core/config/Config.h',
	'src/core/config/ConfigTransform.h',
	'src/core/config/usage.h',
	'src/core/Controller.h',
	'src/core/Miner.h',
	'src/core/Taskbar.h',
	'src/net/interfaces/IJobResultListener.h',
	'src/net/JobResult.h',
	'src/net/JobResults.h',
	'src/net/Network.h',
	'src/net/strategies/DonateStrategy.h',
	'src/Summary.h',
	'src/version.h',
]

headers_crypto = [
	'src/backend/common/interfaces/IMemoryPool.h',
	'src/crypto/cn/asm/CryptonightR_template.h',
	'src/crypto/cn/c_blake256.h',
	'src/crypto/cn/c_groestl.h',
	'src/crypto/cn/c_jh.h',
	'src/crypto/cn/c_skein.h',
	'src/crypto/cn/CnAlgo.h',
	'src/crypto/cn/CnCtx.h',
	'src/crypto/cn/CnHash.h',
	'src/crypto/cn/CryptoNight_monero.h',
	'src/crypto/cn/CryptoNight_test.h',
	'src/crypto/cn/CryptoNight.h',
	'src/crypto/cn/groestl_tables.h',
	'src/crypto/cn/hash.h',
	'src/crypto/cn/skein_port.h',
	'src/crypto/cn/soft_aes.h',
	'src/crypto/common/HugePagesInfo.h',
	'src/crypto/common/MemoryPool.h',
	'src/crypto/common/Nonce.h',
	'src/crypto/common/portable/mm_malloc.h',
	'src/crypto/common/VirtualMemory.h'
]

if arm or riscv
	headers_crypto += 'src/crypto/cn/CryptoNight_arm.h'
else
	headers_crypto += 'src/crypto/cn/CryptoNight_x86.h'
endif

sources = sources_base + sources_base_http + sources_backend
sources += [
	'src/App.cpp',
	'src/core/config/Config.cpp',
	'src/core/config/ConfigTransform.cpp',
	'src/core/Controller.cpp',
	'src/core/Miner.cpp',
	'src/core/Taskbar.cpp',
	'src/net/JobResults.cpp',
	'src/net/Network.cpp',
	'src/net/strategies/DonateStrategy.cpp',
	'src/Summary.cpp',
	'src/xmrig.cpp'
]

sources_crypto = [
	'src/crypto/cn/c_blake256.c',
	'src/crypto/cn/c_groestl.c',
	'src/crypto/cn/c_jh.c',
	'src/crypto/cn/c_skein.c',
	'src/crypto/cn/CnCtx.cpp',
	'src/crypto/cn/CnHash.cpp',
	'src/crypto/common/HugePagesInfo.cpp',
	'src/crypto/common/MemoryPool.cpp',
	'src/crypto/common/Nonce.cpp',
	'src/crypto/common/VirtualMemory.cpp'
]

extra_libs = []

if c.get_id() == 'gcc'
	compile_flags_for_files += {'src/crypto/cn/CnHash.cpp': ['-Ofast', '-fno-tree-vectorize']}
endif

if vaes
	xmrig_defines += 'DXMRIG_VAES'
	headers_crypto += 'src/crypto/cn/CryptoNight_x86_vaes.h'
	sources_crypto += 'src/crypto/cn/CryptoNight_x86_vaes.cpp'
	compile_flags_for_files += {'src/crypto/cn/CryptoNight_x86_vaes.cpp': ['-Ofast', '-fno-tree-vectorize', '-mavx2', '-mvaes']}
endif

if get_option('hwloc')
	headers_crypto += 'src/crypto/common/NUMAMemoryPool.h'
	sources_crypto += 'src/crypto/common/NUMAMemoryPool.cpp'
	sources_crypto += 'src/crypto/common/VirtualMemory_hwloc.cpp'
endif

if os_win
	sources_os += 'res/app.rc'
	sources_os += 'src/App_win.cpp'
	sources_os += 'src/crypto/common/VirtualMemory_win.cpp'
	extra_libs += [
		dependency('ws2_32'),
		dependency('psapi'),
		dependency('iphlpapi'),
		dependency('userenv'),
		dependency('dbghelp')
	]
elif host_machine.system() == 'darwin'
	sources_os += 'src/App_unix.cpp'
	sources_os += 'src/crypto/common/VirtualMemory_unix.cpp'
	extra_libs += dependency('IOKit')
	extra_libs += dependency('CoreServices')
else
	sources_os += 'src/App_unix.cpp'
	sources_os += 'src/crypto/common/VirtualMemory_unix.cpp'
	
	if host_machine.system() == 'android'
		extra_libs += dependency('threads')
		extra_libs += c.find_library('dl')
		extra_libs += c.find_library('log')
	elif host_machine.system() == 'linux'
		sources += 'src/crypto/common/LinuxMemory.h'
		sources += 'src/crypto/common/LinuxMemory.cpp'
		extra_libs += dependency('threads')
		extra_libs += dependency('dl')
	elif host_machine.system() == 'freebsd'
		extra_libs += dependency('kvm')
		extra_libs += dependency('threads')
	endif
endif

uv_dep = dependency('libuv', static: true, default_options: ['default_library=static'])
xmrig_deps += uv_dep

# cmake/flags.cmake

if get_option('buildtype') == 'release'
	xmrig_defines += 'NDEBUG'
endif

cflags = []
cflags_rel = []
cflags_relwithdbg = []
cppflags = []
cppflags_rel = []
cppflags_relwithdbg = []
exe_linker_flags = []

if cpp.get_id() == 'gcc' or cpp.get_id() == 'clang'
	if host_machine.cpu_family() == 'aarch64'
		# the variable name arm8_cpp_flags IS AL LIE!!!
		cflags += arm8_cpp_flags
		cppflags += arm8_cpp_flags + ['-flax-vector-conversions']
	elif host_machine.cpu_family() == 'arm'
		cflags += ['-march=armv7-a', '-mfpu=neon', '-flax-vector-conversions']
		cppflags += ['-march=armv7-a', '-mfpu=neon', '-flax-vector-conversions']
	elif riscv
		cflags += ['-march=' + rvarch]
		cppflags += ['-march=' + rvarch]
		xmrig_defines += 'HAVE_ROTR'
	else
		cflags += '-maes'
		cppflags += '-maes'
		
		if cpp.get_id() == 'gcc' or cpp.has_header_symbol('x86intrin.h', '_rotr')
			xmrig_defines += 'HAVE_ROTR'
		endif
	endif
endif

if cpp.get_id() == 'gcc'
	# desisions for the C compiler bases on the C++ compiler id... This is probably
	# wrong.
	cflags += ['-Wno-strict-aliasing']
	cflags_rel += '-Ofast'
	
	cppflags += ['-fexceptions', '-fno-rtti', '-Wno-strict-aliasing', '-Wno-class-memaccess']
	cppflags_rel += ['-Ofast', '-s']
	
	if os_win
		exe_linker_flags += '-static'
		if not xmrig_64_bit
			exe_linker_flags += '-Wl,--large-address-aware'
		endif
	elif host_machine.system() == 'haiku'
		exe_linker_flags += '-static-libgcc'
	else
		exe_linker_flags += ['-static-libgcc', '-static-libstdc++']
	endif
	
	xmrig_defines += ['_GNU_SOURCE',  'HAVE_BUILTIN_CLEAR_CACHE']
elif cpp.get_id() == 'msvc'
	cflags_rel += ['/MP', '/MT', '/O2',  '/Oi', '/DNDEBUG', '/GL']
	cppflags_rel += ['/MP', '/MT', '/O2',  '/Oi', '/DNDEBUG', '/GL']
	
	cflags_relwithdbg += ['/MP', '/Ob1', '/Zi', '/DRELWITHDEBINFO']
	cppflags_relwithdbg += ['/MP', '/Ob1', '/Zi', '/DRELWITHDEBINFO']
	
	xmrig_defines += ['_CRT_SECURE_NO_WARNINGS', '_CRT_NONSTDC_NO_WARNINGS', 'NOMINMAX', 'HAVE_ROTR']
elif cpp.get_id() == 'clang'
	cflags_rel += ['-funroll-loops', '-fmerge-all-constants']
	
	cppflags += ['-fexceptions', '-fno-rtti']
	cppflags_rel += ['-funroll-loops', '-fmerge-all-constants']
	
	if os_win and arm
		exe_linker_flags += '-static'
	endif
endif

add_project_dependencies(declare_dependency(compile_args: cflags),language: 'c')
add_project_dependencies(declare_dependency(compile_args: cppflags),language: 'cpp')
if get_option('buildtype') == 'release'
	add_project_dependencies(declare_dependency(compile_args: cflags_rel),language: 'c')
	add_project_dependencies(declare_dependency(compile_args: cppflags_rel),language: 'cpp')
elif get_option('buildtype') == 'debugoptimized'
	add_project_dependencies(declare_dependency(compile_args: cflags_relwithdbg),language: 'c')
	add_project_dependencies(declare_dependency(compile_args: cppflags_relwithdbg),language: 'cpp')
endif

if (not os_win) and cpp.has_header_symbol('stdlib.h', '__builtin___clear_cache')
	xmrig_defines += 'HAVE_BUILTIN_CLEAR_CACHE'
endif

# cmake/randomx.cmake

if get_option('randomx')
	if os_win
		if cpp.has_header_symbol('stdlib.h', '_aligned_malloc')
			xmrig_defines += 'HAVE_ALIGNED_MALLOC'
		endif
	else
		if cpp.has_header_symbol('stdlib.h', 'posix_memalign')
			xmrig_defines += 'HAVE_POSIX_MEMALIGN'
		endif
	endif
	
	xmrig_defines += 'XMRIG_ALGO_RANDOMX'
	if not get_option('argon2')
		error('randomx requires argon2')
	endif
	
	headers_crypto += [
		'src/crypto/rx/Rx.h',
		'src/crypto/rx/RxAlgo.h',
		'src/crypto/rx/RxBasicStorage.h',
		'src/crypto/rx/RxCache.h',
		'src/crypto/rx/RxConfig.h',
		'src/crypto/rx/RxDataset.h',
		'src/crypto/rx/RxQueue.h',
		'src/crypto/rx/RxSeed.h',
		'src/crypto/rx/RxVm.h'
	]
	
	sources_crypto += [
		'src/crypto/randomx/aes_hash.cpp',
		'src/crypto/randomx/allocator.cpp',
		'src/crypto/randomx/blake2_generator.cpp',
		'src/crypto/randomx/blake2/blake2b.c',
		'src/crypto/randomx/bytecode_machine.cpp',
		'src/crypto/randomx/dataset.cpp',
		'src/crypto/randomx/instructions_portable.cpp',
		'src/crypto/randomx/randomx.cpp',
		'src/crypto/randomx/reciprocal.c',
		'src/crypto/randomx/soft_aes.cpp',
		'src/crypto/randomx/superscalar.cpp',
		'src/crypto/randomx/virtual_machine.cpp',
		'src/crypto/randomx/virtual_memory.cpp',
		'src/crypto/randomx/vm_compiled_light.cpp',
		'src/crypto/randomx/vm_compiled.cpp',
		'src/crypto/randomx/vm_interpreted_light.cpp',
		'src/crypto/randomx/vm_interpreted.cpp',
		'src/crypto/rx/Rx.cpp',
		'src/crypto/rx/RxAlgo.cpp',
		'src/crypto/rx/RxBasicStorage.cpp',
		'src/crypto/rx/RxCache.cpp',
		'src/crypto/rx/RxConfig.cpp',
		'src/crypto/rx/RxDataset.cpp',
		'src/crypto/rx/RxQueue.cpp',
		'src/crypto/rx/RxVm.cpp'
	]
	
	if get_option('asm') and c.get_id() == 'msvc'
		add_languages('masm', native: false)
		sources_crypto += 'src/crypto/randomx/jit_compiler_x86_static.asm'
		sources_crypto += 'src/crypto/randomx/jit_compiler_x86.cpp'
	elif get_option('asm') and (not arm) and (not riscv) and xmrig_64_bit
		sources_crypto += 'src/crypto/randomx/jit_compiler_x86_static.S'
		sources_crypto += 'src/crypto/randomx/jit_compiler_x86.cpp'
	elif host_machine.cpu_family() == 'aarch64'
		sources_crypto += 'src/crypto/randomx/jit_compiler_a64_static.S'
		sources_crypto += 'src/crypto/randomx/jit_compiler_a64.cpp'
	elif host_machine.cpu_family() == 'riscv64'
		sources_crypto += [
			'src/crypto/randomx/jit_compiler_rv64_static.S',
			'src/crypto/randomx/jit_compiler_rv64_vector_static.S',
			'src/crypto/randomx/jit_compiler_rv64.cpp',
			'src/crypto/randomx/jit_compiler_rv64_vector.cpp',
			'src/crypto/randomx/aes_hash_rv64_vector.cpp',
			'src/crypto/randomx/aes_hash_rv64_zvkned.cpp'
		]
		
		rv64_vector_file_arch = 'rv64gcv'
		if (not meson.is_cross_build()) or meson.has_exe_wrapper()
			if rv_zicbop
				rv64_vector_file_arch += '_zicbop'
			endif
			if rv_zba
				rv64_vector_file_arch += '_zba'
			endif
			if rv_zbb
				rv64_vector_file_arch += '_zbb'
			endif
			if rv_zvkb
				rv64_vector_file_arch += '_zvkb'
			endif
		endif
		
		compile_flags_for_files += {'src/crypto/randomx/jit_compiler_rv64_vector_static.S': ['-march='+rv64_vector_file_arch+'_zvkned']}
		compile_flags_for_files += {'src/crypto/randomx/aes_hash_rv64_vector.cpp': ['-march='+rv64_vector_file_arch]}
		compile_flags_for_files += {'src/crypto/randomx/aes_hash_rv64_zvkned.cpp': ['-march='+rv64_vector_file_arch+'_zvkned']}
	else
		sources_crypto += 'src/crypto/randomx/jit_compiler_fallback.cpp'
	endif
	if sse41
		sources_crypto += 'src/crypto/randomx/blake2/blake2b_sse41.c'
		if c.get_id() == 'gcc' or c.get_id() == 'clang'
			compile_flags_for_files += {'src/crypto/randomx/blake2/blake2b_sse41.c': ['-Ofast', '-msse4.1']}
		endif
	endif
	if avx2
		sources_crypto += 'src/crypto/randomx/blake2/avx2/blake2b_avx2.c'
		if c.get_id() == 'gcc' or c.get_id() == 'clang'
			compile_flags_for_files += {'src/crypto/randomx/blake2/avx2/blake2b_avx2.c': ['-Ofast', '-mavx2']}
		endif
	endif
	if cpp.get_id() == 'clang'
		compile_flags_for_files += {'src/crypto/randomx/jit_compiler_x86.cpp': ['-Wno-unused-const-variable']}
	endif
	
	if get_option('hwloc')
		headers_crypto += 'src/crypto/rx/RxNUMAStorage.h'
		sources_crypto += 'src/crypto/rx/RxNUMAStorage.cpp'
	endif
	
	
	if msr
		xmrig_defines += 'XMRIG_FEATURE_MSR'
		xmrig_defines += 'XMRIG_FIX_RYZEN'
		
		if os_win
			sources_crypto += 'src/crypto/rx/RxFix_win.cpp'
			sources_crypto += 'src/hw/msr/Msr_win.cpp'
		elif host_machine.system() == 'linux'
			sources_crypto += 'src/crypto/rx/RxFix_linux.cpp'
			sources_crypto += 'src/hw/msr/Msr_linux.cpp'
		endif
		
		headers_crypto += [
			'src/crypto/rx/RxFix.h',
			'src/crypto/rx/RxMsr.h',
			'src/hw/msr/Msr.h',
			'src/hw/msr/MsrItem.h'
		]
		
		sources_crypto += [
			'src/crypto/rx/RxMsr.cpp',
			'src/hw/msr/Msr.cpp',
			'src/hw/msr/MsrItem.cpp'
		]
	endif
	
	if get_option('profiling')
		xmrig_defines += 'XMRIG_FEATURE_PROFILING'
		headers_crypto += 'src/crypto/rx/Profiler.h'
		sources_crypto += 'src/crypto/rx/Profiler.cpp'
	endif
	
	if vaes
		sources_crypto += 'src/crypto/randomx/aes_hash_vaes512.cpp'
		# checking the c compiler and seting flags for C++ again
		if c.get_id() == 'msvc'
			compile_flags_for_files += {'src/crypto/randomx/aes_hash_vaes512.cpp': ['/arch:AVX512']}
		elif c.get_id() == 'gcc' or c.get_id() == 'clang'
			compile_flags_for_files += {'src/crypto/randomx/aes_hash_vaes512.cpp': ['-mavx512f', '-mvaes']}
		endif
	endif
endif

# cmake/argon2.cmake

if get_option('argon2')
	xmrig_defines += 'XMRIG_ALGO_ARGON2'
	headers_crypto += 'src/crypto/argon2/Hash.h'
	headers_crypto += 'src/crypto/argon2/Impl.h'
	sources_crypto += 'src/crypto/argon2/Impl.cpp'
endif

# cmake/kawpow.cmake

if get_option('kawpow')
	xmrig_defines += 'XMRIG_ALGO_KAWPOW'
	headers_crypto += 'src/crypto/kawpow/KPCache.h'
	headers_crypto += 'src/crypto/kawpow/KPHash.h'
	sources_crypto += 'src/crypto/kawpow/KPCache.cpp'
	sources_crypto += 'src/crypto/kawpow/KPHash.cpp'
endif

# cmake/ghostrider.cmake

if get_option('ghostrider')
	xmrig_defines += 'XMRIG_ALGO_GHOSTRIDER'
	subdir('src/crypto/ghostrider')
	xmrig_link_with += ghostrider_lib
endif

# cmake/openssl.cmake


exe_name = 'xmrig'
if openssl_dep.found()
	if os_win
		extra_libs += 'crypt32'
	endif
	
	tls_sources = [
		'src/base/net/stratum/Tls.cpp',
		'src/base/net/stratum/Tls.h',
		'src/base/net/tls/ServerTls.cpp',
		'src/base/net/tls/ServerTls.h',
		'src/base/net/tls/TlsConfig.cpp',
		'src/base/net/tls/TlsConfig.h',
		'src/base/net/tls/TlsContext.cpp',
		'src/base/net/tls/TlsContext.h',
		'src/base/net/tls/TlsGen.cpp',
		'src/base/net/tls/TlsGen.h'
	]
	
	if get_option('http')
		tls_sources += [
			'src/base/net/https/HttpsClient.cpp',
			'src/base/net/https/HttpsClient.h',
			'src/base/net/https/HttpsContext.cpp',
			'src/base/net/https/HttpsContext.h',
			'src/base/net/https/HttpsServer.cpp',
			'src/base/net/https/HttpsServer.h'
		]
	endif
	xmrig_defines += 'XMRIG_FEATURE_TLS'
else
	if llhttp_dep.found()
		tls_sources += 'src/base/net/http/HttpServer.cpp'
		tls_sources += 'src/base/net/http/HttpServer.h'
	endif
	
	exe_name += '-notls'
endif

if get_option('cn-lite')
	xmrig_defines += 'XMRIG_ALGO_CN_LITE'
endif
if get_option('cn-heavy')
	xmrig_defines += 'XMRIG_ALGO_CN_HEAVY'
endif
if get_option('cn-pico')
	xmrig_defines += 'XMRIG_ALGO_CN_PICO'
endif
if get_option('cn-femto')
	xmrig_defines += 'XMRIG_ALGO_CN_FEMTO'
endif

if get_option('embedded-config')
	xmrig_defines += 'XMRIG_FEATURE_EMBEDDED_CONFIG'
endif

subdir('src/hw/api')
subdir('src/hw/dmi')

if get_option('debug-log')
	xmrig_defines += 'APP_DEBUG'
endif


xmrig_def_args = []
foreach def : xmrig_defines
	xmrig_def_args += '-D' + def
endforeach

# cmake/asm.cmake

if get_option('asm') and (not arm) and (not riscv) and xmrig_64_bit
	if c.get_id() == 'msvc'
		add_languages('masm')
		# MSVC_TOOLSET_VERSION GREATER_EQUAL 141
		# https://cmake.org/cmake/help/latest/variable/MSVC_TOOLSET_VERSION.html
		if c.version().version_compare('>=15.0')
			asm_files = [
				'src/crypto/cn/asm/cn_main_loop.asm',
				'src/crypto/cn/asm/CryptonightR_template.asm'
			]
		else
			asm_files = [
				'src/crypto/cn/asm/cn_main_loop.S',
				'src/crypto/cn/asm/CryptonightR_template.S'
			]
		endif
	else
		if os_win and c.get_id() == 'gcc'
			asm_files = [
				'src/crypto/cn/asm/win64/cn_main_loop.S',
				'src/crypto/cn/asm/CryptonightR_template.S'
			]
		else
			asm_files = [
				'src/crypto/cn/asm/cn_main_loop.S',
				'src/crypto/cn/asm/CryptonightR_template.S'
			]
		endif
	endif
	
	xmrig_link_with += static_library('xmrig-asm', asm_files, c_args: xmrig_def_args, cpp_args: xmrig_def_args, install:false)
	xmrig_asm_sources = [
		'src/crypto/common/Assembly.h',
		'src/crypto/common/Assembly.cpp',
		'src/crypto/cn/r/CryptonightR_gen.cpp'
	]
	xmrig_defines += 'XMRIG_FEATURE_ASM'
else
	xmrig_asm_sources = []
endif

inputs = headers + sources + sources_os + headers_crypto + sources_crypto + sources_syslog + tls_sources + xmrig_asm_sources
inputs_filtered = []

foreach s : inputs
	if s in compile_flags_for_files
		extra_args = compile_flags_for_files[s]
		xmrig_link_with += static_library('boguslib-for-' + s.underscorify(), s,
			c_args: xmrig_def_args + extra_args, cpp_args: xmrig_def_args + extra_args,
			dependencies: argon2_dep,
			#dependencies: xmrig_deps + extra_libs,
			install: false,
		)
	else
		inputs_filtered += s
	endif
endforeach

executable(exe_name,
	inputs_filtered,
	link_with: xmrig_link_with,
	link_args: exe_linker_flags,
	c_args: xmrig_def_args,
	cpp_args: xmrig_def_args,
	dependencies: xmrig_deps + extra_libs,
	install: true,
)

if os_win
	if not arm
		install_data('bin/WinRing0/WinRing0x64.sys')
	endif
	install_data('scripts/benchmark_1M.cmd')
	install_data('scripts/benchmark_10M.cmd')
	install_data('scripts/pool_mine_example.cmd')
	install_data('scripts/solo_mine_example.cmd')
	install_data('scripts/rtm_ghostrider_example.cmd')
endif

# TODO 3rdparty durchgehen und änderungen patchen
