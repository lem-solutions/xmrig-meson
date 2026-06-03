// we're effectivly aliasing <OpenCL/cl.h> to <CL/cl.h>.
// This is useful because we don't need to handle them having a different path
// on Apple platforms than on other platforms.
#include <CL/cl.h>
