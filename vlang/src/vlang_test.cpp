#include "vlang_test.h"
#include "vlang.h"
#include "common.h"
#include "etypes.h"
#include "elog.h"
/**
float dot(float x0, float y0, float z0, float x1, float y1, float z1)
{
	return x0 * x1 + y0 * y1 + z0 * z1;
}
void c_main()
{
	float x0 = 1.0f;
	float y0 = 0.5f;
	float z0 = 1.0f;
	float x1 = 0.3f;
	float y1 = 0.5f;
	float z1 = 0.7f;
	float d = dot(x0, y0, z0, x1, y1, z1);
	elog("c main: %f", d);
}
**/
float dot(float x0)
{
	if (x0 < 4.0f)
		return dot(x0 + 2.0f);
	else
		return x0;
}
void c_main()
{
	float x0 = 1.0f;
	float d = dot(x0);
	elog("c main: %f", d);
}
void vlang_test()
{
    const char* vlang = 
	/**
	"func dot(x0 float, y0 float, z0 float, x1 float, y1 float, z1 float) (d float)\n"
    "{\n"
    "d = x0 * x1 + y0 * y1 + z0 * z1;\n"
    "}\n"
	"func main (a float, b float) (c float)\n"
    "{\n"
    "x0 := 1.0;\n"
    "y0 := 0.5;\n"
    "z0 := 1.0;\n"
    "x1 := 0.3;\n"
    "y1 := 0.5;\n"
    "z1 := 0.7;\n"
    "var d float;\n"
    "d = dot(x0, y0, z0, x1, y1, z1);\n"
    "print d;\n"
	"}\n";
	**/
	"func dot(x0 float) (d float)\n"
	"{\n"
	"    if (x0 < 4.0)\n"
	"    {\n"
	"        var x1 float;\n"
	"        x1 = x0 + 2.0;\n"
	"        d = dot(x1);\n"
    "    }\n"
	"    else if(x0 >= 4.0)\n"
	"    {\n"
    "        d = x0;\n"
	"    }\n"
	"}\n"
	"func main (a float, b float) (c float)\n"
	"{\n"
	"x0 := 1.0;\n"
	"var d float;\n"
	"d = dot(x0);\n"
	"print d;\n"
	"var ret float;\n"
	"ret = c_test(d);\n"
    "print ret;\n"
	"}\n";
	vlang_run(vlang);
	c_main();
}