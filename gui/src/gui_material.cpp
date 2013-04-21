#include "gui_pch.h"
#include "gui_material.h"
#include "render_system.h"

Pass create_gui_pass(VertexDecl vdec)
{
	pass_decl pdec(RenderSystem_register_vertex_declaration(vdec), COLOR_MAP_MASK, Multiply);
	return RenderSystem_register_std_pass(&pdec);
}