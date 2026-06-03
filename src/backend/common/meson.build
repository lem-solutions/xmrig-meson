headers_backend_common = [
	'src/backend/common/Hashrate.h',
	'src/backend/common/Tags.h',
	'src/backend/common/interfaces/IBackend.h',
	'src/backend/common/interfaces/IRxListener.h',
	'src/backend/common/interfaces/IRxStorage.h',
	'src/backend/common/interfaces/IWorker.h',
	'src/backend/common/misc/PciTopology.h',
	'src/backend/common/Thread.h',
	'src/backend/common/Threads.h',
	'src/backend/common/Worker.h',
	'src/backend/common/WorkerJob.h',
	'src/backend/common/Workers.h'
]

sources_backend_common = [
	'src/backend/common/Hashrate.cpp',
	'src/backend/common/Threads.cpp',
	'src/backend/common/Worker.cpp',
	'src/backend/common/Workers.cpp'
]

if get_option('randomx') and get_option('benchmark')
	headers_backend_common += [
		'src/backend/common/benchmark/Benchmark.h',
		'src/backend/common/benchmark/BenchState_test.h',
		'src/backend/common/benchmark/BenchState.h',
		'src/backend/common/interfaces/IBenchListener.h'
	]
	
	sources_backend_common += [
		'src/backend/common/benchmark/Benchmark.cpp',
		'src/backend/common/benchmark/BenchState.cpp'
	]
endif

if get_option('opencl') or get_option('cuda')
	headers_backend_common += [
		'src/backend/common/HashrateInterpolator.h',
		'src/backend/common/GpuWorker.h'
	]
	sources_backend_common += [
		'src/backend/common/HashrateInterpolator.cpp',
		'src/backend/common/GpuWorker.cpp'
	]
endif
