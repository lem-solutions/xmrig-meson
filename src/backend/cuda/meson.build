xmrig_defines += 'XMRIG_FEATURE_CUDA'
headers_backend_cuda = [
	'src/backend/cuda/CudaBackend.h',
	'src/backend/cuda/CudaConfig_gen.h',
	'src/backend/cuda/CudaConfig.h',
	'src/backend/cuda/CudaLaunchData.h',
	'src/backend/cuda/CudaThread.h',
	'src/backend/cuda/CudaThreads.h',
	'src/backend/cuda/CudaWorker.h',
	'src/backend/cuda/interfaces/ICudaRunner.h',
	'src/backend/cuda/runners/CudaBaseRunner.h',
	'src/backend/cuda/runners/CudaCnRunner.h',
	'src/backend/cuda/wrappers/CudaDevice.h',
	'src/backend/cuda/wrappers/CudaLib.h'
]

sources_backend_cuda = [
	'src/backend/cuda/CudaBackend.cpp',
	'src/backend/cuda/CudaConfig.cpp',
	'src/backend/cuda/CudaLaunchData.cpp',
	'src/backend/cuda/CudaThread.cpp',
	'src/backend/cuda/CudaThreads.cpp',
	'src/backend/cuda/CudaWorker.cpp',
	'src/backend/cuda/runners/CudaBaseRunner.cpp',
	'src/backend/cuda/runners/CudaCnRunner.cpp',
	'src/backend/cuda/wrappers/CudaDevice.cpp',
	'src/backend/cuda/wrappers/CudaLib.cpp'
]

if get_option('nvml').disable_if(host_machine.system() == 'darwin').enable_auto_if(true).enabled()
	xmrig_defines += 'XMRIG_FEATURE_NVML'
	headers_backend_cuda += [
		'src/backend/cuda/wrappers/nvml_lite.h',
		'src/backend/cuda/wrappers/NvmlHealth.h',
		'src/backend/cuda/wrappers/NvmlLib.h'
	]
	sources_backend_cuda += 'src/backend/cuda/wrappers/NvmlLib.cpp'
endif

if get_option('randomx')
	headers_backend_cuda += 'src/backend/cuda/runners/CudaRxRunner.h'
	sources_backend_cuda += 'src/backend/cuda/runners/CudaRxRunner.cpp'
endif

if get_option('kawpow')
	headers_backend_cuda += 'src/backend/cuda/runners/CudaKawPowRunner.h'
	sources_backend_cuda += 'src/backend/cuda/runners/CudaKawPowRunner.cpp'
endif
