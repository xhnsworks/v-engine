#include "pass_console.h"
#include "recycle_bin.h"
#include "shader_object.h"
#include "shader_node.h"
#include "shader_buffer.h"
#include "vertex_shader_buffer.h"
#include "pixel_shader_buffer.h"
#include "array.h"
#include "estring.h"
#include "std_material.h"
uint32 _file_length( FILE *fp )
{
    uint32 cur_pos;
    uint32 len;

///取得当前文件流的读取位置
    cur_pos = ftell( fp );
///将文件流的读取位置设为文件末尾
    fseek( fp, 0, SEEK_END );
///获取文件末尾的读取位置,即文件大小
    len = ftell( fp );
///将文件流的读取位置还原为原先的值
    fseek( fp, cur_pos, SEEK_SET );
    return len;
}
void Char_Dest(vptr p)
{
    printf("char dest\n");
}
char* _load_text_file(const char* _file_name)
{
    FILE* fp = fopen(_file_name, "r");
    uint32 len = _file_length( fp );
    char* buffer = Malloc(len);
    fread(buffer, len, 1, fp);
    fclose(fp);
    return buffer;
}

Shader create_shader_from_file(const char* _file_name, shader_type _type)
{
    char* buf = _load_text_file(_file_name);
    Shader ret = Shader_new();
    Shader_load_from_string(ret, (const char*)buf, _type);
    return ret;
}

Pass create_pass_from_shader(Shader _vtx_sdr, Shader _pxl_sdr)
{
    Pass ret = Pass_new();
    Pass_set_vertex_shader(ret, _vtx_sdr);
    Pass_set_pixel_shader(ret, _pxl_sdr);
    return ret;
}
///=========================ShaderNode============================///

ShaderNode create_map_sample3_node()
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "MapSample3");

    ShaderNode_set_function(ret,
                            "{\n"
                            "    Color = texture2D(Map, UV).rgb;\n"
                            "}\n");

    ShaderNode_add_input_param(ret, Texture2D_Obj, "Map", 1);
    ShaderNode_add_input_param(ret, Float2_Obj, "UV", 1);

    ShaderNode_add_output_param(ret, Float3_Obj, "Color", 1);
    return ret;
}

ShaderNode create_map_sample4_node()
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "MapSample4");

    ShaderNode_set_function(ret,
                            "{\n"
                            "    Color = texture2D(Map, UV);\n"
                            "}\n");

    ShaderNode_add_input_param(ret, Texture2D_Obj, "Map", 1);
    ShaderNode_add_input_param(ret, Float2_Obj, "UV", 1);

    ShaderNode_add_output_param(ret, Float4_Obj, "Color", 1);
    return ret;
}

///=========================create_lighting_shader_node============================///

ShaderNode create_clear_lighting_value_node()
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "ClearLightingValue");

    ShaderNode_set_function(ret,
                            "{\n"
                            "    return vec4(0.0, 0.0, 0.0, 1.0);\n"
                            "}\n");

    ShaderNode_set_return_type(ret, Float4_Obj, 1);
    return ret;
}




