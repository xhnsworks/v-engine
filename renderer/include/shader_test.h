#ifndef SHADER_TEST_H
#define SHADER_TEST_H
#include "common.h"
#include "etypes.h"
#include "shader_buffer.h"
#include "vertex_shader_buffer.h"
#include "pixel_shader_buffer.h"
#include "shader_object.h"
#include "eassert.h"
_INLINE_ void _mat4_obj_test(ShaderBuffer _sb, const char* _name)
{
    ShaderObject mat4 = ShaderBuffer_find_object(_sb, _name);
    EAssert(to_ptr(mat4), "cant find shader object %s!", _name);
    uint array_size, array_index;
    EAssert(ShaderObject_get_type(mat4, &array_size, &array_index) == Matrix4x4_Obj, "shader object %s is not Matrix4x4_Obj!", _name);
    EAssert(array_size == 1 || array_index != INVALID_ARRAY_INDEX, "shader object %s is array!", _name);
}

_INLINE_ void _vec3_obj_test(ShaderBuffer _sb, const char* _name)
{
    ShaderObject vec3 = ShaderBuffer_find_object(_sb, _name);
    EAssert(to_ptr(vec3), "cant find shader object %s!", _name);
    uint array_size, array_index;
    EAssert(ShaderObject_get_type(vec3, &array_size, &array_index) == Float3_Obj, "shader object %s is not Float3_Obj!", _name);
    EAssert(array_size == 1 || array_index != INVALID_ARRAY_INDEX, "shader object %s is array!", _name);
}

_INLINE_ void _vec2_obj_test(ShaderBuffer _sb, const char* _name)
{
    ShaderObject vec2 = ShaderBuffer_find_object(_sb, _name);
    EAssert(to_ptr(vec2), "cant find shader object %s!", _name);
    uint array_size, array_index;
    EAssert(ShaderObject_get_type(vec2, &array_size, &array_index) == Float2_Obj, "shader object %s is not Float2_Obj!", _name);
    EAssert(array_size == 1 || array_index != INVALID_ARRAY_INDEX, "shader object %s is array!", _name);
}

_INLINE_ void _tex_2d_obj_test(ShaderBuffer _sb, const char* _name)
{
    ShaderObject tex2 = ShaderBuffer_find_object(_sb, _name);
    EAssert(to_ptr(tex2), "cant find shader object %s!", _name);
    uint array_size, array_index;
    EAssert(ShaderObject_get_type(tex2, &array_size, &array_index) == Texture2D_Obj, "shader object %s is not Texture2D_Obj!", _name);
    EAssert(array_size == 1 || array_index != INVALID_ARRAY_INDEX, "shader object %s is array!", _name);
}
#endif
