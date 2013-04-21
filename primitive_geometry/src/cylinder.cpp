#include "cylinder.h"
#include "math_base.h"
#include "float2.h"
#include "float3.h"
#include "float4.h"
#include "matrix4x4.h"
#include "sfloat3.h"
#include "sfloat4.h"
#include "emem.h"

Mesh create_cylinder_mesh(float height, float radius, int num_segments)
{

    ///height = 3.0f;
    ///radius = 0.2f;
    ///num_segments = 16;

    Mesh ret = Mesh_new();

    float rad = 2.0f * E_PI / (float)num_segments;
    sfloat4 vec = SFloat4(0.0f, radius, 0.0f, 1.0f);
    matrix4x4* axis_rot_mat = Matrix4x4_new();
    Matrix4x4_from_axis_angle(axis_rot_mat, SFloat3(0.0f, 0.0f, 1.0f), rad);
    float half_height = height * 0.5f;

    sfloat3 pos_axis_disp = SFloat3(0.0f, 0.0f, half_height);
    sfloat3 neg_axis_disp = SFloat3(0.0f, 0.0f, -half_height);

	EFloat3* pos_buf = ENEW_ARRAY EFloat3[num_segments * 2];
	EFloat3* nor_buf = ENEW_ARRAY EFloat3[num_segments * 2];
	EFloat2* uv_buf = ENEW_ARRAY EFloat2[num_segments * 2];
	uint32* idx_buf = (uint32*)SMalloc(sizeof(uint32) * num_segments * 6);
    for (int i = 0; i < num_segments; i++)
    {
        sfloat4 tmp = Matrix4x4_mul_float4(axis_rot_mat, vec);
        vec = tmp;
        ///sfloat3 tmp_f3 = SFloat3_assign_from_float3(Float4_xyz(tmp));
        sfloat3 tmp_f3 = SFloat4_xyz(tmp);

        sfloat3 pos0 = SFloat3_add(tmp_f3, pos_axis_disp);
        sfloat3 pos1 = SFloat3_add(tmp_f3, neg_axis_disp);

        pos_buf[i] = SFloat3_convert(pos0);
        pos_buf[num_segments + i] = SFloat3_convert(pos1);

        tmp_f3 = SFloat3_normalize(tmp_f3);

        nor_buf[i] = SFloat3_convert(tmp_f3);
        nor_buf[num_segments + i] = SFloat3_convert(tmp_f3);

        uv_buf[i].x = (float)i / (float)(num_segments - 1);
        uv_buf[i].y = 0.0f;

        uv_buf[num_segments + i].x = (float)i / (float)(num_segments - 1);
        uv_buf[num_segments + i].y = 1.0f;

        if (i < num_segments - 1)
        {
            idx_buf[i * 6 + 0] = num_segments + i;
            idx_buf[i * 6 + 1] = i + 1;
            idx_buf[i * 6 + 2] = i;

            idx_buf[i * 6 + 3] = i + 1;
            idx_buf[i * 6 + 4] = num_segments + i;
            idx_buf[i * 6 + 5] = num_segments + i + 1;
        }
        else
        {
            idx_buf[i * 6 + 0] = num_segments + i;
            idx_buf[i * 6 + 1] = 0;
            idx_buf[i * 6 + 2] = i;

            idx_buf[i * 6 + 3] = 0;
            idx_buf[i * 6 + 4] = num_segments + i;
            idx_buf[i * 6 + 5] = num_segments;
        }
    }
    Mesh_build(ret,
               (float*)pos_buf, (float*)uv_buf, (float*)nor_buf, num_segments * 2,
               idx_buf, num_segments * 2,
               Triangular);
	EDELETE_ARRAY[] pos_buf;
	EDELETE_ARRAY[] uv_buf;
	EDELETE_ARRAY[] nor_buf;
	Mfree(idx_buf);
    return ret;
}
