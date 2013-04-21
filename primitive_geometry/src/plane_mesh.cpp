#include "plane_mesh.h"
#include "math_base.h"
#include "float2.h"
#include "float3.h"
#include "float4.h"
#include "matrix4x4.h"
#include "sfloat3.h"
#include "sfloat4.h"

_INLINE_ EFloat2 MakeEFloat2(float x, float y)
{
	EFloat2 ret(x, y);
	return ret;
}
struct MakeEFloat3
{
	inline EFloat3 operator() (float x, float y, float z) {
		return EFloat3(x, y, z);
	}
};
Mesh create_plane_mesh()
{
	return create_plane_mesh(-0.5f, -0.5f, 0.0f, 1.0f, 1.0f);
}

Mesh create_plane_mesh(float x, float y, float z, float width, float height)
{
	EColor c;
	c.red = 1.0f;
	c.green = 1.0f;
	c.blue = 1.0f;
	c.alpha = 1.0f;
	return create_plane_mesh(x, y, z, width, height, 0.0f, 1.0f, 0.0f, 1.0f, c, c, c, c);
}

Mesh create_plane_mesh(float x, float y, float z, float width, float height, float u0, float u1, float v0, float v1, 
					   const EColor& color_u0v0, const EColor& color_u1v0, const EColor& color_u1v1, const EColor& color_u0v1)
{
	Mesh ret = Mesh_new();

    EFloat3 pos[4];
    EFloat3 nor[4];
    EFloat2 uv[4];
	EColor col[4];

    uint32 idx[6];

	make_plane_points<EFloat3, MakeEFloat3>(x, y, z, width, height, pos);

    nor[0] = nor[1] = nor[2] = nor[3] = EFloat3(0.0f, 0.0f, 1.0f);///( { EFloat3 f3 = {0.0f, 0.0f, 1.0f}; f3; } );

	uv[0] = MakeEFloat2(u0, v0);
    uv[1] = MakeEFloat2(u1, v0);
    uv[2] = MakeEFloat2(u1, v1);
    uv[3] = MakeEFloat2(u0, v1);

	col[0] = color_u0v0;
    col[1] = color_u1v0;
	col[2] = color_u1v0;
	col[3] = color_u0v1;

    idx[0] = 3;
    idx[1] = 2;
    idx[2] = 0;

    idx[3] = 2;
    idx[4] = 1;
    idx[5] = 0;

    Mesh_build2(ret,
               (float*)pos, (float*)uv, (float*)nor, (float*)col, 4,
               idx, 2,
               Triangular);
    return ret;
}