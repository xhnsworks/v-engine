#include "cylinder.h"
#include "math_base.h"
#include "float2.h"
#include "float3.h"
#include "float4.h"
#include "matrix4x4.h"
#include "sfloat3.h"
#include "sfloat4.h"
Mesh create_plane_mesh()
{
    Mesh ret = Mesh_new();

    EFloat3 pos[4];
    EFloat3 nor[4];
    EFloat2 uv[4];

    uint32 idx[6];

    float width = 2.0f;

    pos[0] = ( {EFloat3 f3 = {-width, -width, 0.0f}; f3; });
    pos[1] = ( {EFloat3 f3 = {width, -width, 0.0f}; f3; });
    pos[2] = ( {EFloat3 f3 = {width, width, 0.0f}; f3; });
    pos[3] = ( {EFloat3 f3 = {-width, width, 0.0f}; f3; });

    nor[0] = nor[1] = nor[2] = nor[3] = ( { EFloat3 f3 = {0.0f, 0.0f, 1.0f}; f3; } );

    uv[0] = ( { EFloat2 f2 = {0.0f, 0.0f}; f2; } );
    uv[1] = ( { EFloat2 f2 = {1.0f, 0.0f}; f2; } );
    uv[2] = ( { EFloat2 f2 = {1.0f, 1.0f}; f2; } );
    uv[3] = ( { EFloat2 f2 = {0.0f, 1.0f}; f2; } );

    idx[0] = 0;
    idx[1] = 2;
    idx[2] = 3;

    idx[3] = 0;
    idx[4] = 1;
    idx[5] = 2;

    Mesh_build(ret,
               pos, uv, nor, 4,
               idx, 2,
               Triangular);
    return ret;
}

Mesh create_cylinder_mesh(float height, float radius, int num_segments)
{

    ///height = 3.0f;
    ///radius = 0.2f;
    ///num_segments = 16;

    Mesh ret = Mesh_new();

    float rad = 2.0f * E_PI / (float)num_segments;
    sfloat4 vec = SFloat4(0.0f, radius, 0.0f, 1.0f);
    Matrix4x4 axis_rot_mat = Matrix4x4_new();
    Matrix4x4_from_axis_angle(axis_rot_mat, SFloat3(0.0f, 0.0f, 1.0f), rad);
    float half_height = height * 0.5f;

    sfloat3 pos_axis_disp = SFloat3(0.0f, 0.0f, half_height);
    sfloat3 neg_axis_disp = SFloat3(0.0f, 0.0f, -half_height);
    EFloat3 pos_buf[num_segments * 2];
    EFloat3 nor_buf[num_segments * 2];
    EFloat2 uv_buf[num_segments * 2];

    uint32 idx_buf[num_segments * 6];
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
               pos_buf, uv_buf, nor_buf, num_segments * 2,
               idx_buf, num_segments * 2,
               Triangular);

    return ret;
}
