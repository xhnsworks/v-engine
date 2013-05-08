#include "pch.h"
#include "material_instance.h"
#include "std_pass.h"
#include "lighting_pass.h"
#include "display_pass.h"

#include "array.h"
#include "estring.h"
#include "gl_utility.h"
#include "shader_buffer.h"
#include "renderer.h"
#include "pass_declaration.h"
#include "render_system.h"
#include "texture2d.h"

MaterialInstance::MaterialInstance(const char* matName, const char* colTexFile, const char* norTexFile, const char* resGrp)
{
	Texture2DPtr null_tex;
	if (colTexFile && strcmp(colTexFile, "") != 0)
		m_colTex = RenderSystem_register_texture2d(colTexFile, resGrp);
	else
		m_colTex = null_tex;
	if (norTexFile && strcmp(norTexFile, "") != 0)
		m_norTex = RenderSystem_register_texture2d(norTexFile, resGrp);
	else
		m_norTex = null_tex;

	if (matName)
		m_matName = matName;
	else
		m_matName = "default_material";
}

MaterialInstance* MaterialInstance::Clone()
{
    return ENEW MaterialInstance(*this);
}