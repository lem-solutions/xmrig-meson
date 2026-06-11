headers_backend_cpu = [
	'src/backend/cpu/Cpu.h',
	'src/backend/cpu/CpuBackend.h',
	'src/backend/cpu/CpuConfig_gen.h',
	'src/backend/cpu/CpuConfig.h',
	'src/backend/cpu/CpuLaunchData.h', # .cpp in cmake
	'src/backend/cpu/CpuThread.h',
	'src/backend/cpu/CpuThreads.h',
	'src/backend/cpu/CpuWorker.h',
	'src/backend/cpu/interfaces/ICpuInfo.h',
	'src/backend/cpu/platform/BasicCpuInfo.h'
]

sources_backend_cpu = [
	'src/backend/cpu/Cpu.cpp',
	'src/backend/cpu/CpuBackend.cpp',
	'src/backend/cpu/CpuConfig.cpp',
	'src/backend/cpu/CpuLaunchData.cpp', # .h in cmake
	'src/backend/cpu/CpuThread.cpp',
	'src/backend/cpu/CpuThreads.cpp',
	'src/backend/cpu/CpuWorker.cpp'
]

if hwloc_dep.found()
	headers_backend_cpu += 'src/backend/cpu/platform/HwlocCpuInfo.h'
	sources_backend_cpu += 'src/backend/cpu/platform/HwlocCpuInfo.cpp'
endif

if riscv
	sources_backend_cpu += 'src/backend/cpu/platform/lscpu_riscv.cpp'
	sources_backend_cpu += 'src/backend/cpu/platform/BasicCpuInfo_riscv.cpp'
elif arm
	sources_backend_cpu += 'src/backend/cpu/platform/BasicCpuInfo_arm.cpp'
	if os_win
		sources_backend_cpu += 'src/backend/cpu/platform/BasicCpuInfo_arm_win.cpp'
	elif host_machine.system() == 'darwin'
		sources_backend_cpu += 'src/backend/cpu/platform/BasicCpuInfo_arm_mac.cpp'
	else
		sources_backend_cpu += 'src/backend/cpu/platform/lscpu_arm.cpp'
		sources_backend_cpu += 'src/backend/cpu/platform/BasicCpuInfo_arm_unix.cpp'
	endif
else
	sources_backend_cpu += 'src/backend/cpu/platform/BasicCpuInfo.cpp'
endif
