#include <float2.h>
#include <float3.h>
#include <float4.h>
#include <mem.h>
void test()
{
    MInit();
    Float_Init();
    {
        float3 f0 = Float3(1.0, 2.0, 3.0);
        float3 f1 = Float3(2.0, 3.0, 4.0);
        float d = Float3_dot(f0, f1);
        if (d != 20.0)
        {
            printf("test error\n");
            getchar();
            exit(0);
        }
        f0 = Float3(1.0, 0.0, 0.0);
        f1 = Float3(0.0, 1.0, 0.0);
        f0 = Float3_cross(f0, f1);
        Float3_print(f0);
    }

    {
        float4 f0 = Float4(1.0, 2.0, 3.0, 4.0);
        float4 f1 = Float4(2.0, 3.0, 4.0, 5.0);
        float d = Float4_dot(f0, f1);
        if (d != 20.0)
        {
            printf("test error\n");
            getchar();
            exit(0);
        }
    }
}
