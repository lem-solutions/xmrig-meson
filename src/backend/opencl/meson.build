
xmrig_defines += 'XMRIG_FEATURE_OPENCL'
xmrig_defines += 'CL_USE_DEPRECATED_OPENCL_1_2_APIS'

headers_backend_opencl = [
	'src/backend/opencl/cl/OclSource.h',
	'src/backend/opencl/interfaces/IOclRunner.h',
	'src/backend/opencl/kernels/Cn0Kernel.h',
	'src/backend/opencl/kernels/Cn1Kernel.h',
	'src/backend/opencl/kernels/Cn2Kernel.h',
	'src/backend/opencl/kernels/CnBranchKernel.h',
	'src/backend/opencl/OclBackend.h',
	'src/backend/opencl/OclCache.h',
	'src/backend/opencl/OclConfig.h',
	'src/backend/opencl/OclConfig_gen.h',
	'src/backend/opencl/OclGenerator.h',
	'src/backend/opencl/OclLaunchData.h',
	'src/backend/opencl/OclThread.h',
	'src/backend/opencl/OclThreads.h',
	'src/backend/opencl/OclWorker.h',
	'src/backend/opencl/runners/OclBaseRunner.h',
	'src/backend/opencl/runners/OclCnRunner.h',
	'src/backend/opencl/runners/tools/OclCnR.h',
	'src/backend/opencl/runners/tools/OclSharedData.h',
	'src/backend/opencl/runners/tools/OclSharedState.h',
	'src/backend/opencl/wrappers/OclContext.h',
	'src/backend/opencl/wrappers/OclDevice.h',
	'src/backend/opencl/wrappers/OclError.h',
	'src/backend/opencl/wrappers/OclKernel.h',
	'src/backend/opencl/wrappers/OclLib.h',
	'src/backend/opencl/wrappers/OclPlatform.h',
	'src/backend/opencl/wrappers/OclVendor.h'
]

sources_backend_opencl = [
	'src/backend/opencl/cl/OclSource.cpp',
	'src/backend/opencl/generators/ocl_generic_cn_generator.cpp',
	'src/backend/opencl/generators/ocl_vega_cn_generator.cpp',
	'src/backend/opencl/kernels/Cn0Kernel.cpp',
	'src/backend/opencl/kernels/Cn1Kernel.cpp',
	'src/backend/opencl/kernels/Cn2Kernel.cpp',
	'src/backend/opencl/kernels/CnBranchKernel.cpp',
	'src/backend/opencl/OclBackend.cpp',
	'src/backend/opencl/OclCache.cpp',
	'src/backend/opencl/OclConfig.cpp',
	'src/backend/opencl/OclLaunchData.cpp',
	'src/backend/opencl/OclThread.cpp',
	'src/backend/opencl/OclThreads.cpp',
	'src/backend/opencl/OclWorker.cpp',
	'src/backend/opencl/runners/OclBaseRunner.cpp',
	'src/backend/opencl/runners/OclCnRunner.cpp',
	'src/backend/opencl/runners/tools/OclCnR.cpp',
	'src/backend/opencl/runners/tools/OclSharedData.cpp',
	'src/backend/opencl/runners/tools/OclSharedState.cpp',
	'src/backend/opencl/wrappers/OclContext.cpp',
	'src/backend/opencl/wrappers/OclDevice.cpp',
	'src/backend/opencl/wrappers/OclError.cpp',
	'src/backend/opencl/wrappers/OclKernel.cpp',
	'src/backend/opencl/wrappers/OclLib.cpp',
	'src/backend/opencl/wrappers/OclPlatform.cpp'
]

vl_ver = get_option('opencl-version')
if vl_ver == 'auto'
	if host_machine.system() == 'darwin'
		vl_ver = '120'
	else
		vl_ver = '200'
	endif
endif

xmrig_defines += 'CL_TARGET_OPENCL_VERSION='+vl_ver
if host_machine.system() == 'darwin'
	sources_backend_opencl += 'src/backend/opencl/wrappers/OclDevice_mac.cpp'
	if vl_ver != '120'
		error('for darwin only opnecl-version 120 is valid.')
	endif
endif

if os_win
	sources_backend_opencl += 'src/backend/opencl/OclCache_win.cpp'
else
	sources_backend_opencl += 'src/backend/opencl/OclCache_unix.cpp'
endif

if get_option('randomx')
	headers_backend_opencl += [
		'src/backend/opencl/kernels/rx/Blake2bHashRegistersKernel.h',
		'src/backend/opencl/kernels/rx/Blake2bInitialHashBigKernel.h',
		'src/backend/opencl/kernels/rx/Blake2bInitialHashDoubleKernel.h',
		'src/backend/opencl/kernels/rx/Blake2bInitialHashKernel.h',
		'src/backend/opencl/kernels/rx/ExecuteVmKernel.h',
		'src/backend/opencl/kernels/rx/FillAesKernel.h',
		'src/backend/opencl/kernels/rx/FindSharesKernel.h',
		'src/backend/opencl/kernels/rx/HashAesKernel.cpp',
		'src/backend/opencl/kernels/rx/InitVmKernel.h',
		'src/backend/opencl/kernels/rx/RxJitKernel.h',
		'src/backend/opencl/kernels/rx/RxRunKernel.h',
		'src/backend/opencl/runners/OclRxBaseRunner.h',
		'src/backend/opencl/runners/OclRxJitRunner.h',
		'src/backend/opencl/runners/OclRxVmRunner.h'
	]
		
	sources_backend_opencl += [
		'src/backend/opencl/generators/ocl_generic_rx_generator.cpp',
		'src/backend/opencl/kernels/rx/Blake2bHashRegistersKernel.cpp',
		'src/backend/opencl/kernels/rx/Blake2bInitialHashBigKernel.cpp',
		'src/backend/opencl/kernels/rx/Blake2bInitialHashDoubleKernel.cpp',
		'src/backend/opencl/kernels/rx/Blake2bInitialHashKernel.cpp',
		'src/backend/opencl/kernels/rx/ExecuteVmKernel.cpp',
		'src/backend/opencl/kernels/rx/FillAesKernel.cpp',
		'src/backend/opencl/kernels/rx/FindSharesKernel.cpp',
		'src/backend/opencl/kernels/rx/HashAesKernel.cpp',
		'src/backend/opencl/kernels/rx/InitVmKernel.cpp',
		'src/backend/opencl/kernels/rx/RxJitKernel.cpp',
		'src/backend/opencl/kernels/rx/RxRunKernel.cpp',
		'src/backend/opencl/runners/OclRxBaseRunner.cpp',
		'src/backend/opencl/runners/OclRxJitRunner.cpp',
		'src/backend/opencl/runners/OclRxVmRunner.cpp'
	]
endif

if get_option('kawpow')
	headers_backend_opencl += [
		'src/backend/opencl/kernels/kawpow/KawPow_CalculateDAGKernel.h',
		'src/backend/opencl/runners/OclKawPowRunner.h',
		'src/backend/opencl/runners/tools/OclKawPow.h'
	]
	
	sources_backend_opencl += [
		'src/backend/opencl/generators/ocl_generic_kawpow_generator.cpp',
		'src/backend/opencl/kernels/kawpow/KawPow_CalculateDAGKernel.cpp',
		'src/backend/opencl/runners/OclKawPowRunner.cpp',
		'src/backend/opencl/runners/tools/OclKawPow.cpp'
	]
endif

if get_option('strict-cache')
	xmrig_defines += 'XMRIG_STRICT_OPENCL_CACHE'
endif
if get_option('interleave-debug-log')
	xmrig_defines += 'XMRIG_INTERLEAVE_DEBUG'
endif
if adl
	xmrig_defines += 'XMRIG_FEATURE_ADL'
	headers_backend_opencl += 'src/backend/opencl/wrappers/AdlHealth.h'
	headers_backend_opencl += 'src/backend/opencl/wrappers/AdlLib.h'
	
	if os_win
		sources_backend_opencl += 'src/backend/opencl/wrappers/AdlLib.cpp'
	else
		sources_backend_opencl += 'src/backend/opencl/wrappers/AdlLib_linux.cpp'
	endif
endif
