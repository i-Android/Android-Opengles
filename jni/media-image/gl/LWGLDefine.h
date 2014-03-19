#ifndef __LWGL_DEFINE_H__
#define __LWGL_DEFINE_H__

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "../jniDefine.h"


#define INVALID_LOCATION -1
#define INVALID_UNIFORM -1
#define INVALID_PROGRAM 0
#define INVALID_SHADER 0
#define INVALID_TEXTURE 0
#define INVALID_FRAMEBUFFER 0
#define INVALID_ATTRIBUTE_INDEX -1
#define INVALID_ID -1
#define INVALID_GROUP_ID -1
#define INVALID_EFFECT_ID -1
#define INVALID_FILTER_ID -1

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif


// clockwise
typedef enum _tagFilterRotationMode
{
    RotationMode_RotationMask = 0xF0,
    RotationMode_FlipMask = 0x0F,
    RotationMode_Unknown = -1,
    RotationMode_Rotate0 = 0x00,
    RotationMode_Rotate90 = 0x10,
    RotationMode_Rotate180 = 0x20,
    RotationMode_Rotate270 = 0x30,
    RotationMode_Rotate0_FlipV = 0x01,   // Flip vertical
    RotationMode_Rotate90_FlipV = 0x11,  // Rotate 90 then flip vertical, AKA transpose, flipped about top-left <--> bottom-right axis
    RotationMode_Rotate180FlipV = 0x21,  // Rotate 180 then flip vertical, AKA flip horizontal
    RotationMode_Rotate270FlipV = 0x31   // Rotate 270 then flip vertical, AKA transverse, flipped about top-right <--> bottom-left axis
} RotationMode;


typedef enum _tagInputFormat
{
    InputFormat_RGB,
    InputFormat_RGBA,
    InputFormat_YUV420P
} InputFormat;


typedef struct _tagRect
{
    int left, top, right, bottom;
} Rect;

typedef struct _tagRectF
{
    float left, top, right, bottom;
} RectF;



typedef enum _tagVarType
{
    VAR_UNKNOWN,
    VAR_ATTR_POSITION, 	// value will be NULL
    VAR_ATTR_TEX_COORD, // value will be NULL
    VAR_UNIFM_INPUT, 	// value will be NULL
    VAR_IMAGE_FILE,
    VAR_IMAGE_OBJ,
    VAR_INT,
    VAR_INT2,
    VAR_INT3,
    VAR_INT4,
    VAR_FLOAT,
    VAR_FLOAT2,
    VAR_FLOAT3,
    VAR_FLOAT4,
    VAR_GL_SAMPLER_2D, 	// type for filter.shader
    VAR_COUNT
} VarType;



#endif // __LWGL_DEFINE_H__
