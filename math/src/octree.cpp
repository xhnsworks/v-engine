#include "octree.h"
#include "emem.h"
#include "array.h"
#define MASK_X 0x00000001
#define MASK_Y 0x00000002
#define MASK_Z 0x00000004
static inline sfloat3 _get_center(octree_node* node)
{
    sfloat3 half = SFloat3_assign_from_float(0.5f);
    sfloat3 c = SFloat3_sub(node->bounding_box.max_xyz, node->bounding_box.min_xyz);
    c = SFloat3_mul(c, half);
    c = SFloat3_add(c, node->bounding_box.min_xyz);
    return c;
}
static inline float get_x(float* x_array, int index)
    {
        if (index & MASK_X)
            return x_array[1];
        else
            return x_array[0];
    }
static inline float get_y(float* y_array, int index)
    {
        if (index & MASK_Y)
            return y_array[1];
        else
            return y_array[0];
    }
static inline float get_z(float* z_array, int index)
    {
        if (index & MASK_Z)
            return z_array[1];
        else
            return z_array[0];
    }
void draw_axis_aligned_box(axis_aligned_box* box, LineDrawer drawer, float shrink, EFloat4* color)
{
    sfloat3 half = SFloat3_assign_from_float(0.5f);
    sfloat3 center = SFloat3_sub(box->max_xyz, box->min_xyz);
    center = SFloat3_mul(center, half);
    center = SFloat3_add(center, box->min_xyz);

    sfloat3 size = SFloat3_sub(box->max_xyz, box->min_xyz);

    sfloat3 half_size = SFloat3_mul(size, half);
    sfloat3 s = SFloat3_assign_from_float(shrink);
    sfloat3 tmp0 = SFloat3_sub(half_size, s);
    sfloat3 tmp1 = SFloat3_sub(center, tmp0);
    sfloat3 tmp2 = SFloat3_add(center, tmp0);

    EFloat3 min_xyz = SFloat3_convert(tmp1);
    EFloat3 max_xyz = SFloat3_convert(tmp2);

    EFloat3 normal(0.0f, 0.0f, 0.0f);

    float x_array[2] = { min_xyz.x, max_xyz.x };
    float y_array[2] = { min_xyz.y, max_xyz.y };
    float z_array[2] = { min_xyz.z, max_xyz.z };
	/**
    float get_x(int index)
    {
        if (index & MASK_X)
            return x_array[1];
        else
            return x_array[0];
    }
    float get_y(int index)
    {
        if (index & MASK_Y)
            return y_array[1];
        else
            return y_array[0];
    }
    float get_z(int index)
    {
        if (index & MASK_Z)
            return z_array[1];
        else
            return z_array[0];
    }
	**/
    for (euint i = 0; i < 8; i++)
    {
        for (euint j = 0; j < 8; j++)
        {
            euint diff_count = 0;
            euint diff = i ^ j;
            if (diff & MASK_X)
                diff_count++;
            if (diff & MASK_Y)
                diff_count++;
            if (diff & MASK_Z)
                diff_count++;
            /// only one axis is diffent then draw it
            if (diff_count != 1)
                continue;
			
            float x0 = get_x(x_array, j);
            float y0 = get_y(y_array, j);
            float z0 = get_z(z_array, j);

            float x1 = get_x(x_array, i);
            float y1 = get_y(y_array, i);
            float z1 = get_z(z_array, i);

            line_def line;
            line.begin.x = x0;
            line.begin.y = y0;
            line.begin.z = z0;
            line.begin_normal = normal;
            line.begin_color = *color;

            line.end.x = x1;
            line.end.y = y1;
            line.end.z = z1;
            line.end_normal = normal;
            line.end_color = *color;

            ILnDwr.add_line(drawer, &line);
        }
    }
}
typedef void (*DoIt)(struct _octree_node_proc*, octree_node*, int, int, int);
typedef struct _octree_node_proc
{
    DoIt do_it;
} octree_node_proc;
typedef struct _octree_node_proc* OctreeNodeProc;

static inline int _get_index(int x, int y, int z, int grid_size)
{
    int depth = grid_size * grid_size;
    int width = grid_size;
    return z * depth + y * width + x;
}

void _traversal_nodes(octree_node* nodes, int grid_size, OctreeNodeProc proc)
{
    for (int z = 0; z < grid_size; z++)
    {
        for (int y = 0; y < grid_size; y++)
        {
            for (int x = 0; x < grid_size; x++)
            {
                int index = _get_index(x, y, z, grid_size);
                octree_node* node = &nodes[index];
                proc->do_it(proc, node, x, y, z);
            }
        }
    }
}

typedef struct _octree_node_gen
{
    octree_node_proc base;
    float node_size;
} octree_node_gen;

void octree_node_Init(octree_node_gen* gen, octree_node* node, int x, int y, int z)
{
    sfloat3 node_size = SFloat3_assign_from_float(gen->node_size);
    sfloat3 min_xyz = SFloat3_assign_from_int3(x, y, z);
    min_xyz = SFloat3_mul(min_xyz, node_size);
    sfloat3 max_xyz = SFloat3_add(min_xyz, node_size);

    node->bounding_box.min_xyz = min_xyz;
    node->bounding_box.max_xyz = max_xyz;
    memset(&node->branch, 0, sizeof(node->branch));
    node->stem = NULL;
    node->color.x = 1.0f;
    node->color.y = 0.0f;
    node->color.z = 0.0f;
    node->color.w = 1.0f;
}

void octree_node_gen_Init(octree_node_gen* gen, float node_size)
{
    gen->node_size = node_size;
    gen->base.do_it = (DoIt)octree_node_Init;
}

typedef struct _octree_layer_info
{
    octree_node* nodes;
    float node_size;
    int grid_size;
} octree_layer_info;

typedef struct _octree_node_linker
{
    octree_node_proc base;
    octree_layer_info* prev_layer;
} octree_node_linker;

void octree_node_link(octree_node_linker* linker, octree_node* node, int x, int y, int z)
{
    int prev_x = 0;
    int prev_y = 0;
    int prev_z = 0;
	/**
    octree_node* _get_node()
    {
        int index = _get_index(prev_x, prev_y, prev_z, linker->prev_layer->grid_size);
        return &linker->prev_layer->nodes[index];
    }
	**/
    for (int i = 0; i < 8; i++)
    {
        int x_idx = i & MASK_X;
        int y_idx = i & MASK_Y;
        int z_idx = i & MASK_Z;
        x_idx = x_idx >> 0;
        y_idx = y_idx >> 1;
        z_idx = z_idx >> 2;
        prev_x = x * 2 + x_idx;
        prev_y = y * 2 + y_idx;
        prev_z = z * 2 + z_idx;
		int index = _get_index(prev_x, prev_y, prev_z, linker->prev_layer->grid_size);
		node->branch.type1[i] = &linker->prev_layer->nodes[index];
        ///node->branch.type1[i] = _get_node();
        ///_get_node()->stem = node;
		linker->prev_layer->nodes[index].stem = node;
    }
}
void octree_node_linker_Init(octree_node_linker* linker, octree_layer_info* prev_layer)
{
    linker->base.do_it = (DoIt)octree_node_link;
    linker->prev_layer = prev_layer;
}
octree_layer_info create_octree_layer(octree_layer_info* prev_layer)
{
    octree_layer_info curt_layer;
    curt_layer.grid_size = prev_layer->grid_size / 2;
    curt_layer.node_size = prev_layer->node_size * 2.0f;
    curt_layer.nodes = (octree_node*)SMalloc(sizeof(octree_node) * curt_layer.grid_size * curt_layer.grid_size * curt_layer.grid_size);

    octree_node_gen gen;
    octree_node_gen_Init(&gen, curt_layer.node_size);
	_traversal_nodes(curt_layer.nodes, curt_layer.grid_size, (OctreeNodeProc)&gen);

    octree_node_linker linker;
    octree_node_linker_Init(&linker, prev_layer);
	_traversal_nodes(curt_layer.nodes, curt_layer.grid_size, (OctreeNodeProc)&linker);

    return curt_layer;
}

static inline bool _is_in_x_plane(octree_node* node, sfloat3 pt)
{
    sfloat3_compare_result min_cmp_ret = SFloat3_greater(pt, node->bounding_box.min_xyz);
    if (min_cmp_ret.y_cmp_ret && min_cmp_ret.z_cmp_ret) {
        sfloat3_compare_result max_cmp_ret = SFloat3_less(pt, node->bounding_box.max_xyz);
        if (max_cmp_ret.y_cmp_ret && max_cmp_ret.z_cmp_ret)
            return true;
    }
    return false;
}

static inline bool _is_in_y_plane(octree_node* node, sfloat3 pt)
{
    sfloat3_compare_result min_cmp_ret = SFloat3_greater(pt, node->bounding_box.min_xyz);
    if (min_cmp_ret.x_cmp_ret && min_cmp_ret.z_cmp_ret) {
        sfloat3_compare_result max_cmp_ret = SFloat3_less(pt, node->bounding_box.max_xyz);
        if (max_cmp_ret.x_cmp_ret && max_cmp_ret.z_cmp_ret)
            return true;
    }
    return false;
}

static inline bool _is_in_z_plane(octree_node* node, sfloat3 pt)
{
    sfloat3_compare_result min_cmp_ret = SFloat3_greater(pt, node->bounding_box.min_xyz);
    if (min_cmp_ret.x_cmp_ret && min_cmp_ret.y_cmp_ret) {
        sfloat3_compare_result max_cmp_ret = SFloat3_less(pt, node->bounding_box.max_xyz);
        if (max_cmp_ret.x_cmp_ret && max_cmp_ret.y_cmp_ret)
            return true;
    }
    return false;
}

static inline int _outer_index(octree_node* node, sfloat3 pt)
{
    sfloat3 center = _get_center(node);
    sfloat3_compare_result cmp_ret = SFloat3_greater(pt, center);
    volatile int i = 0;
    if (cmp_ret.x_cmp_ret)
        i |= MASK_X;
    if (cmp_ret.y_cmp_ret)
        i |= MASK_Y;
    if (cmp_ret.z_cmp_ret)
        i |= MASK_Z;
    return i;
}

typedef struct _octree_node_index_pair
{
    int idx0;
    int idx1;
} octree_node_index_pair;
octree_node_index_pair _inner_index(octree_node* node, sfloat3 pt, euint mask)
{
    sfloat3 center = _get_center(node);
    sfloat3_compare_result cmp_ret = SFloat3_greater(pt, center);
    /// 这里存在一个GCC的bug
    volatile euint i = 0;
    if (cmp_ret.x_cmp_ret) {
        i |= MASK_X;
    }
    if (cmp_ret.y_cmp_ret) {
        i |= MASK_Y;
    }
    if (cmp_ret.z_cmp_ret) {
        i |= MASK_Z;
    }

    euint inv_mask = ~mask;
    euint index0 = i & inv_mask;
    euint index1 = i | mask;

    octree_node_index_pair ret;
    ret.idx0 = index0;
    ret.idx1 = index1;

    return ret;
}

///       x-,y+,z- ________   x+,y+,z-
///               /|       /|
///              / |      / |
///   x-,y+,z+  /--------/x+,y+,z+
///             |  |_____|__|
///       x-,y-,z-/      |  / x+,y-,z-
///             |/       | /
///    x-,y-,z+ -------- x+,y-,z+
///

void octree_node_make_outer_planes(octree_node* node, plane* pos_x, plane* neg_x, plane* pos_y, plane* neg_y, plane* pos_z, plane* neg_z)
{
    sfloat3 c = _get_center(node);

    EFloat3 min_xyz = SFloat3_convert(node->bounding_box.min_xyz);
    EFloat3 max_xyz = SFloat3_convert(node->bounding_box.max_xyz);
    EFloat3 center = SFloat3_convert(c);

#define MAKE_OUTER_PLANE(axis, normal_x, normal_y, normal_z) \
    { \
        pos_##axis->origin = center; \
        pos_##axis->origin.axis = max_xyz.axis; \
        pos_##axis->normal.x = normal_x; \
        pos_##axis->normal.y = normal_y; \
        pos_##axis->normal.z = normal_z; \
        neg_##axis->origin = center; \
        neg_##axis->origin.axis = min_xyz.axis; \
        neg_##axis->normal.x = normal_x; \
        neg_##axis->normal.y = normal_y; \
        neg_##axis->normal.z = normal_z; \
    }
    MAKE_OUTER_PLANE(x, 1.0f, 0.0f, 0.0f);
    MAKE_OUTER_PLANE(y, 0.0f, 1.0f, 0.0f);
    MAKE_OUTER_PLANE(z, 0.0f, 0.0f, 1.0f);
}
void octree_node_make_inner_planes(octree_node* node, plane* inner_x, plane* inner_y, plane* inner_z)
{
    sfloat3 c = _get_center(node);
    EFloat3 center = SFloat3_convert(c);
    inner_x->origin = center; inner_x->normal.x = 1.0f; inner_x->normal.y = 0.0f; inner_x->normal.z = 0.0f;
    inner_y->origin = center; inner_y->normal.x = 0.0f; inner_y->normal.y = 1.0f; inner_y->normal.z = 0.0f;
    inner_z->origin = center; inner_z->normal.x = 0.0f; inner_z->normal.y = 0.0f; inner_z->normal.z = 1.0f;
}
static inline int _cale_num_leaves(int octree_depth)
    {
        int ret = 8;
        for (int i = 0; i < octree_depth; i++)
            ret *= 8;
        return ret;
    }
static inline int _cale_grid_size(int octree_depth)
    {
        int ret = 2;
        for (int i = 0; i < octree_depth; i++)
            ret *= 2;
        return ret;
    }
Octree Octree_new(float min_size, int octree_depth)
{
    
    ///
    int num_nodes = _cale_num_leaves(octree_depth);
    int grid_size = _cale_grid_size(octree_depth);
    octree_node* nodes = (octree_node*)SMalloc(sizeof(octree_node) * num_nodes);

    octree_node_gen gen;
    octree_node_gen_Init(&gen, min_size);
	_traversal_nodes(nodes, grid_size, (OctreeNodeProc)&gen);

    printf("num_leaf_nodes %d\n", num_nodes);

    octree_node** octree_layers = array_new(octree_node*, 5, NULL);
    apush(octree_layers, nodes);

    octree_layer_info prev_layer;
    prev_layer.grid_size = grid_size;
    prev_layer.node_size = min_size;
    prev_layer.nodes = nodes;

    while (octree_depth)
    {
        octree_layer_info curt_layer = create_octree_layer(&prev_layer);
        apush(octree_layers, curt_layer.nodes);
        prev_layer = curt_layer;
        octree_depth--;
    }

    octree_layer_info curt_layer = create_octree_layer(&prev_layer);
    apush(octree_layers, curt_layer.nodes);

    Octree ret = (Octree)SMalloc(sizeof(octree));
    ret->octree_layers = octree_layers;
    ret->trunk = curt_layer.nodes;

    return ret;
}

void Octree_print(Octree _self)
{
    euint count = 0;
    octree_node* n = _self->trunk;

    do
    {
        elog("##layer %d\n", count);
        sfloat3 size = SFloat3_sub(n->bounding_box.max_xyz, n->bounding_box.min_xyz);
        SFloat3_log(size);
        n = n->branch.type1[0];
        count++;
    }
    while (n);
}

typedef enum _crossed_plane
{
    XPlane,
    YPlane,
    ZPlane,
} crossed_plane;
typedef struct _crossed_point_info
{
    EFloat3       point;
    crossed_plane plane;
} crossed_point_info;

static inline void _draw_pt(EFloat3 p)
{
}
static int s_space_count = 0;
void _touch(octree_node* node, Ray ray, LineDrawer drawer, crossed_point_info* point_array)
{
    ///void _draw_pt(EFloat3 p)
    {
        /**
        EFloat4 c = {0.0f, 0.0f, 1.0f, 1.0f};
        EFloat3 n = {0.0f, 0.0f, 0.0f};
        float r = 0.05f;
#define DRAW_LOC_LINE2(op, axis) \
        { \
            line_def line; \
            line.begin = p; \
            line.begin_color = c; \
            line.begin_normal = n; \
            line.end = p; \
            line.end.axis op##= r; \
            line.end_color = c; \
            line.end_normal = n; \
            ILnDwr.add_line(drawer, &line); \
        }
        DRAW_LOC_LINE2(+, x);
        DRAW_LOC_LINE2(-, x);
        DRAW_LOC_LINE2(+, y);
        DRAW_LOC_LINE2(-, y);
        DRAW_LOC_LINE2(+, z);
        DRAW_LOC_LINE2(-, z);
        **/
    }

    EFloat4 color(1.0f, 0.0f, 0.0f, 1.0f);

    plane inner_x;
    plane inner_y;
    plane inner_z;
    octree_node_make_inner_planes(node, &inner_x, &inner_y, &inner_z);

    bool touched[8];
    memset(touched, 0, sizeof(touched));

    euint n = array_n(point_array);
    crossed_point_info null_point = {EFloat3(fexce, fexce, fexce), XPlane};
    crossed_point_info* next_point_array = array_new(crossed_point_info, n, null_point);

    const char* x = "XAxis";
    const char* y = "YAxis";
    const char* z = "ZAxis";
#define PUSH_CROSS_POINT(axis) \
        if      (_is_in_##axis##_plane(node, SFloat3_assign_from_efloat3(&pt))) { \
            _draw_pt(pt); \
            node->color = color; \
            int idx = _outer_index(node, SFloat3_assign_from_efloat3(&pt)); \
            touched[idx] = true; \
            apush(next_point_array, point_array[i]); \
        }

    for (euint i = 0; i < n; i++)
    {
        EFloat3 pt = point_array[i].point;
        crossed_plane plane_type = point_array[i].plane;
        switch (plane_type)
        {
        case XPlane:
            PUSH_CROSS_POINT(x);
            break;
        case YPlane:
            PUSH_CROSS_POINT(y);
            break;
        case ZPlane:
            PUSH_CROSS_POINT(z);
            break;
        default:
            break;
        }
    }
#define RECURSION_INNER_TOUCH(plane, axis, mask, plane_type) \
    { \
        EFloat3 pt = Plane_ray_cross(&plane, ray); \
        if (pt.x != fexce && _is_in_##axis##_plane(node, SFloat3_assign_from_efloat3(&pt))) { \
            _draw_pt(pt); \
            node->color = color; \
            octree_node_index_pair idx_pair = _inner_index(node, SFloat3_assign_from_efloat3(&pt), mask); \
            touched[idx_pair.idx0] = true; \
            touched[idx_pair.idx1] = true; \
            crossed_point_info point = {pt, plane_type}; \
            apush(next_point_array, point); \
        } \
    }

    RECURSION_INNER_TOUCH(inner_x, x, MASK_X, XPlane);
    RECURSION_INNER_TOUCH(inner_y, y, MASK_Y, YPlane);
    RECURSION_INNER_TOUCH(inner_z, z, MASK_Z, ZPlane);

    char mbuf[256];
    s_space_count += 2;
    for (int i = 0; i < s_space_count; i++) {
        mbuf[i] = ' ';
    }
    mbuf[s_space_count] = 0x00;
#ifdef DEBUG
    snprintf(mbuf, 255, "%s##touched %d", mbuf, array_n(next_point_array));
    elog("%s", mbuf);
#endif

    for (int i = 0; i < 8; i++)
    {
        if (touched[i])
        {
            octree_node* branch = node->branch.type1[i];
            if (branch)
                _touch(branch, ray, drawer, next_point_array);
        }
    }
    s_space_count -= 2;
    array_delete(next_point_array);
}

void Octree_touch(Octree _self, Ray ray, LineDrawer drawer)
{
    ///Ray_log(ray)
    /**;
    EFloat3 ori = {-0.181452, -0.169355, 1.990000};
    EFloat3 dir = {-0.330346, -0.308323, 0.892081};
    ray->origin = ori;
    ray->direction = dir;
    **/

    plane pos_x;
    plane neg_x;
    plane pos_y;
    plane neg_y;
    plane pos_z;
    plane neg_z;
    octree_node_make_outer_planes(_self->trunk, &pos_x, &neg_x, &pos_y, &neg_y, &pos_z, &neg_z);
    crossed_point_info null_point = {EFloat3(fexce, fexce, fexce), XPlane};
    crossed_point_info* next_point_array = array_new(crossed_point_info, 5, null_point);
#define FIND_CROSS_POINT(plane, axis, plane_type) \
    { \
        EFloat3 pt = Plane_ray_cross(&plane, ray); \
        if (pt.x != fexce && _is_in_##axis##_plane(_self->trunk, SFloat3_assign_from_efloat3(&pt))) { \
                crossed_point_info point = {pt, plane_type}; \
                apush(next_point_array, point); \
        } \
    }
    FIND_CROSS_POINT(pos_x, x, XPlane);
    FIND_CROSS_POINT(neg_x, x, XPlane);
    FIND_CROSS_POINT(pos_y, y, YPlane);
    FIND_CROSS_POINT(neg_y, y, YPlane);
    FIND_CROSS_POINT(pos_z, z, ZPlane);
    FIND_CROSS_POINT(neg_z, z, ZPlane);
#ifdef DEBUG
    elog("%s", "#####touch begin#####");
    elog("touched %d", array_n(next_point_array));
#endif
    _touch(_self->trunk, ray, drawer, next_point_array);
    array_delete(next_point_array);
#ifdef DEBUG
    elog("%s", "#####touch end#####");
#endif
}

void _draw_octree_node(octree_node* node, LineDrawer drawer, float shrink)
{
    draw_axis_aligned_box(&node->bounding_box, drawer, shrink, &node->color);
    for (int i = 0; i < 8; i++)
    {
        if (node->branch.type1[i])
            _draw_octree_node(node->branch.type1[i], drawer, shrink + 0.0025f);
    }
}

void _set_octree_node_color(octree_node* node, EFloat4* color)
{
    node->color = *color;
    for (int i = 0; i < 8; i++)
    {
        if (node->branch.type1[i])
            _set_octree_node_color(node->branch.type1[i], color);
    }
}

void Octree_draw(Octree _self, LineDrawer drawer)
{
    _draw_octree_node(_self->trunk, drawer, 0.0f);
}

void Octree_set_color(Octree _self, EFloat4 color)
{
    _set_octree_node_color(_self->trunk, &color);
}
