subdir('cpu')
if get_option('opencl')
	subdir('opencl')
else
	headers_backend_opencl = []
	sources_backend_opencl = []
endif
if get_option('cuda')
	subdir('cuda')
else
	headers_backend_cuda = []
	sources_backend_cuda = []
endif
subdir('common')

headers_backend = headers_backend_common + headers_backend_cpu + headers_backend_opencl + headers_backend_cuda
sources_backend = sources_backend_common + sources_backend_cpu + sources_backend_opencl + sources_backend_cuda
