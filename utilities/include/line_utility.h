/*
* Copyright (c) 2013, �캣��
* All rights reserved.
*
* �ļ����ƣ�line_utility.h
* �ļ����������ߵĹ���
*
* ��ǰ�汾��0.1
* ��    �ߣ��캣��
* ������ڣ�2013��1��27��
*/
#ifndef LINE_UTILITY_H
#define LINE_UTILITY_H
#include "common.h"
#include "etypes.h"
#include "float_base.h"
#include "renderer.h"
#include "ray.h"
typedef struct _line_def
{
    EFloat3 begin;
    EFloat4 begin_color;
    EFloat3 begin_normal;
    EFloat3 end;
    EFloat4 end_color;
    EFloat3 end_normal;
} line_def;

typedef struct _line_drawer
{
    line_def* lines;
    Renderable line_strip_rabl;
    Renderer* rdr;
} line_drawer;
typedef struct _line_drawer* LineDrawer;
API_EXPORT LineDrawer LineDrawer_new(Renderer* _rdr);
API_EXPORT void LineDrawer_delete(LineDrawer _self);
API_EXPORT void LineDrawer_update(LineDrawer _self);
API_EXPORT void LineDrawer_add_line(LineDrawer _self, line_def* def);
API_EXPORT void LineDrawer_draw_circle(LineDrawer _self, EFloat3* ori, EFloat3* dir, float radius);
API_EXPORT void LineDrawer_draw_cone(LineDrawer _self, EFloat3* ori, EFloat3* dir, EFloat4* color, float radius);
API_EXPORT void LineDrawer_draw_translate_manipulator(LineDrawer _self, EFloat3* ori, EFloat3* z_axis, float radius);

struct _i_line_drawer
{
    LineDrawer (*New)(Renderer* _rdr);
    void (*Delete)(LineDrawer _self);
    void (*update)(LineDrawer _self);
    void (*add_line)(LineDrawer _self, line_def* def);
};

static struct _i_line_drawer ILnDwr = {
    LineDrawer_new,
    LineDrawer_delete,
    LineDrawer_update,
    LineDrawer_add_line,
};
#endif // LINE_UTILITY_H
