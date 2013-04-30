#include "locator.h"
#include "sfloat3.h"

Mesh create_locator(EFloat3* pos, float radius)
{
    Mesh ret = Mesh_new();
    sfloat3 x_axis_disp = SFloat3(radius, 0.0f, 0.0f);
    sfloat3 y_axis_disp = SFloat3(0.0f, radius, 0.0f);
    sfloat3 z_axis_disp = SFloat3(0.0f, 0.0f, radius);
    sfloat3 center = SFloat3_assign_from_efloat3(pos);

    float pos_stream[7 * 3];
    float tex_stream[7 * 2];
    float nor_stream[7 * 3];
    memset(pos_stream, 0, sizeof(pos_stream));
    memset(tex_stream, 0, sizeof(tex_stream));
    memset(nor_stream, 0, sizeof(nor_stream));

    sfloat3 tmp;
    SFloat3_export(center, &pos_stream[0 * 3]);

    tmp = SFloat3_add(center, x_axis_disp);  SFloat3_export(tmp, &pos_stream[1 * 3]);
    tmp = SFloat3_sub(center, x_axis_disp);  SFloat3_export(tmp, &pos_stream[2 * 3]);
    tmp = SFloat3_add(center, y_axis_disp);  SFloat3_export(tmp, &pos_stream[3 * 3]);
    tmp = SFloat3_sub(center, y_axis_disp);  SFloat3_export(tmp, &pos_stream[4 * 3]);
    tmp = SFloat3_add(center, z_axis_disp);  SFloat3_export(tmp, &pos_stream[5 * 3]);
    tmp = SFloat3_sub(center, z_axis_disp);  SFloat3_export(tmp, &pos_stream[6 * 3]);

    euint32 idx_stream[6 * 2];

    idx_stream[0] = 0; idx_stream[1] = 1;
    idx_stream[2] = 0; idx_stream[3] = 2;
    idx_stream[4] = 0; idx_stream[5] = 3;
    idx_stream[6] = 0; idx_stream[7] = 4;
    idx_stream[8] = 0; idx_stream[9] = 5;
    idx_stream[10] = 0; idx_stream[11] = 6;

    Mesh_build(ret,
               pos_stream,
               tex_stream,
               nor_stream,
               6 * 2,
               idx_stream,
               6,
               Segment);

    return ret;
}

Mesh create_locator2(EFloat3* pos, float radius)
{
    Mesh ret = Mesh_new();
    sfloat3 x_axis_disp = SFloat3(radius, 0.0f, 0.0f);
    sfloat3 y_axis_disp = SFloat3(0.0f, radius, 0.0f);
    sfloat3 z_axis_disp = SFloat3(0.0f, 0.0f, radius);
    sfloat3 center = SFloat3_assign_from_efloat3(pos);

    float pos_stream[7 * 3];
    float tex_stream[7 * 2];
    float nor_stream[7 * 3];
    float col_stream[7 * 4];
    memset(pos_stream, 0, sizeof(pos_stream));
    memset(tex_stream, 0, sizeof(tex_stream));
    memset(nor_stream, 0, sizeof(nor_stream));
    memset(col_stream, 0, sizeof(col_stream));

    for (int i = 0; i < 7; i++)
    {
        col_stream[i * 4 + 0] = 1.0f;
        col_stream[i * 4 + 1] = 1.0f;
        col_stream[i * 4 + 2] = 0.0f;
        col_stream[i * 4 + 3] = 1.0f;
    }

    sfloat3 tmp;
    SFloat3_export(center, &pos_stream[0 * 3]);

    tmp = SFloat3_add(center, x_axis_disp);  SFloat3_export(tmp, &pos_stream[1 * 3]);
    tmp = SFloat3_sub(center, x_axis_disp);  SFloat3_export(tmp, &pos_stream[2 * 3]);
    tmp = SFloat3_add(center, y_axis_disp);  SFloat3_export(tmp, &pos_stream[3 * 3]);
    tmp = SFloat3_sub(center, y_axis_disp);  SFloat3_export(tmp, &pos_stream[4 * 3]);
    tmp = SFloat3_add(center, z_axis_disp);  SFloat3_export(tmp, &pos_stream[5 * 3]);
    tmp = SFloat3_sub(center, z_axis_disp);  SFloat3_export(tmp, &pos_stream[6 * 3]);

    euint32 idx_stream[6 * 2];

    idx_stream[0] = 0; idx_stream[1] = 1;
    idx_stream[2] = 0; idx_stream[3] = 2;
    idx_stream[4] = 0; idx_stream[5] = 3;
    idx_stream[6] = 0; idx_stream[7] = 4;
    idx_stream[8] = 0; idx_stream[9] = 5;
    idx_stream[10] = 0; idx_stream[11] = 6;

    Mesh_build2(ret,
               pos_stream,
               tex_stream,
               nor_stream,
               col_stream,
               6 * 2,
               idx_stream,
               6,
               Segment);

    return ret;
}
