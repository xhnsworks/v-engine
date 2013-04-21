#ifndef OCTREE_H
#define OCTREE_H
#include "common.h"
#include "etypes.h"
#include "sfloat3.h"
#include "sfloat4.h"
#include "line_utility.h"
#include "plane.h"
struct _axis_aligned_box;
struct _octree_node;
typedef struct _axis_aligned_box
{
    sfloat3 min_xyz;
    sfloat3 max_xyz;
} axis_aligned_box;
API_EXPORT void draw_axis_aligned_box(axis_aligned_box* box, LineDrawer drawer, float shrink, EFloat4* color);
typedef union _octree_branch
{
    struct _branch_type0
    {
        struct _octree_node* back_lower_left;
        struct _octree_node* back_lower_right;
        struct _octree_node* back_upper_left;
        struct _octree_node* back_upper_right;

        struct _octree_node* front_lower_left;
        struct _octree_node* front_lower_right;
        struct _octree_node* front_upper_left;
        struct _octree_node* front_upper_right;
    } type0;
    struct _octree_node* type1[8];
} octree_branch;
typedef struct _octree_node
{
    axis_aligned_box bounding_box;
    octree_branch branch;
    struct _octree_node* stem;
    EFloat4 color;
} octree_node;
API_EXPORT void octree_node_make_outer_planes(octree_node* node, plane* pos_x, plane* neg_x, plane* pos_y, plane* neg_y, plane* pos_z, plane* neg_z);
API_EXPORT void octree_node_make_inner_planes(octree_node* node, plane* inner_x, plane* inner_y, plane* inner_z);
typedef struct _octree
{
    octree_node** octree_layers;
    octree_node* trunk;
} octree;
typedef struct _octree* Octree;
API_EXPORT Octree Octree_new(float min_size, int octree_depth);
API_EXPORT void Octree_draw(Octree _self, LineDrawer drawer);
API_EXPORT void Octree_print(Octree _self);
API_EXPORT void Octree_touch(Octree _self, Ray ray, LineDrawer drawer);
API_EXPORT void Octree_set_color(Octree _self, EFloat4 color);
#endif // OCTREE_H
