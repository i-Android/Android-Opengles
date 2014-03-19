#include "LWGLHelper.h"
#include "LWGLDefine.h"
#include <stdlib.h>

#include <sstream>
using namespace std;

void checkGlError(const char* op)
{
    for (GLint error = glGetError(); error; error = glGetError())
    {
        LOGI("checkGlError after %s() glError (0x%x)\n", op, error);
    }
}
