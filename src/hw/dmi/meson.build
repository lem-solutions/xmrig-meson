if dmi
	xmrig_defines += 'XMRIG_FEATURE_DMI'
	headers += [
		'src/hw/dmi/DmiBoard.h',
		'src/hw/dmi/DmiMemory.h',
		'src/hw/dmi/DmiReader.h',
		'src/hw/dmi/DmiTools.h'
	]
	sources += [
		'src/hw/dmi/DmiBoard.cpp',
		'src/hw/dmi/DmiMemory.cpp',
		'src/hw/dmi/DmiReader.cpp',
		'src/hw/dmi/DmiTools.cpp'
	]
	
	if os_win
		sources += 'src/hw/dmi/DmiReader_win.cpp'
	elif host_machine.system() == 'linux' or host_machine.system() == 'freebsd'
		sources += 'src/hw/dmi/DmiReader_unix.cpp'
	elif host_machine.system() == 'darwin' and host_machine.subsystem() == 'macos'
		sources += 'src/hw/dmi/DmiReader_mac.cpp'
	endif
endif
