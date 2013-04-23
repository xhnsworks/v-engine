#include "shadow_emitter.h"
#include "sketch.h"
#include "matrix4x4.h"
#include "camera.h"
#include "emem.h"
#include "estring.h"
#include "pass_console.h"

ShaderNode create_shadow2d_test_node()
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "Shadow2DTest");
    ShaderNode_add_input_param(ret, Texture2D_Obj, "ShadowMap", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "PixelWorldPosition", 1);
    ShaderNode_add_input_param(ret, Matrix4x4_Obj, "LightWorldMatrix", 1);
    ShaderNode_add_input_param(ret, Matrix4x4_Obj, "LightProjectionMatrix", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "LightPosition", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "LightDirection", 1);
    ///ShaderNode_set_return_type(ret, Float3_Obj, 1);
	ShaderNode_add_output_param(ret, Float3_Obj, "LightValue", 1);

    const char* func =
    "{\n"
    "    vec4 world_pos = vec4(PixelWorldPosition, 1.0);\n"
    "    world_pos = LightWorldMatrix * world_pos;\n"
    "    world_pos = LightProjectionMatrix * world_pos;\n"
    "    world_pos /= world_pos.w;\n"
    "    vec2 uv = world_pos.xy;\n"
    "    uv = uv * 0.5 + vec2(0.5, 0.5);\n"
    "    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0)\n"
	"    {\n"
    "        LightValue = vec3(0.0f, 0.0f, 0.0f);\n"
	"    }\n"
	"    else\n"
	"    {\n"
    "        float depth = world_pos.z * 0.5 + 0.5;\n"
    "        vec2 moments = texture(ShadowMap, uv).rg;\n"
    "        if (depth < moments.x)\n"
	"        {\n"
    "            LightValue = vec3(1.0f, 1.0f, 1.0f);\n"
	"        }\n"
    "        else\n"
	"        {\n"
    "            float variance = moments.y - (moments.x * moments.x);\n"
    "            variance = max(variance,0.000001);\n"
    "            float d = depth - moments.x;\n"
    "            float p_max = variance / (variance + d*d);\n"
    "            LightValue = vec3(p_max, p_max, p_max);\n"
	"        }\n"
	"    }\n"
    "}\n";

    ShaderNode_set_function(ret, func);
    return ret;
}

ShaderNode create_shadowcube_test_node()
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "ShadowCubeTest");
    /**
    ShaderNode_add_input_link(shadow_test_node, shadow_cube_map, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(shadow_test_node, tgt_pos, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(shadow_test_node, light_pos, INVALID_ARRAY_INDEX);
            ShaderNode_set_result_link(shadow_test_node, light_weight, INVALID_ARRAY_INDEX);
    **/
    ShaderNode_add_input_param(ret, TextureCube_Obj, "ShadowMap", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "PixelWorldPosition", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "LightPosition", 1);
    ShaderNode_add_input_param(ret, Float_Obj, "LightInfluence", 1);
    ///ShaderNode_add_input_param(ret, Float3_Obj, "LightDirection", 1);
    ///ShaderNode_set_return_type(ret, Float3_Obj, 1);
	ShaderNode_add_output_param(ret, Float3_Obj, "Result", 1);

    const char* func =
    "{\n"
    "    vec3 ray_dir = PixelWorldPosition - LightPosition;\n"
    "    float depth = length(ray_dir);\n"
    "    depth /= LightInfluence;\n"
    "    depth = clamp(depth, 0.0, 1.0);\n"
    "    ray_dir = normalize(ray_dir);\n"
    "    vec2 moments = texture(ShadowMap, ray_dir).rg;\n"
    "    if (depth < moments.x)\n"
	"    {\n"
    "        Result = vec3(1.0f, 1.0f, 1.0f);\n"
	"    }\n"    
	"    else\n"
	"    {\n"
	"        float variance = moments.y - (moments.x * moments.x);\n"
    "        variance = max(variance,0.002);\n"
    "        float d = depth - moments.x;\n"
    "        float p_max = variance / (variance + d*d);\n"
    "        Result = vec3(p_max, p_max, p_max);\n"
	"    }\n"
    "}\n";

    ShaderNode_set_function(ret, func);
    return ret;
}

ShaderNode create_white_screen_node()
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "WhiteScreen");
    ShaderNode_add_output_param(ret, Float3_Obj, "Result", 1);

    const char* func =
    "{\n"
    "    Result = vec3(1.0f, 1.0f, 1.0f);\n"
    "}\n";

    ShaderNode_set_function(ret, func);
    return ret;
}

