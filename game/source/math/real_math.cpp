#include "math/real_math.hpp"

#include "cseries/cseries.hpp"

#include <math.h>

REFERENCE_DECLARE(0x0189CED8, real_point2d const* const, global_origin2d);
REFERENCE_DECLARE(0x0189CEDC, real_point2d const* const, global_x_axis2d);
REFERENCE_DECLARE(0x0189CEE0, real_point2d const* const, global_y_axis2d);
REFERENCE_DECLARE(0x0189CEE4, real_point2d const* const, global_negative_x_axis2d);
REFERENCE_DECLARE(0x0189CEE8, real_point2d const* const, global_negative_y_axis2d);
REFERENCE_DECLARE(0x0189CEEC, real_vector2d const* const, global_zero_vector2d);
REFERENCE_DECLARE(0x0189CEF0, real_vector2d const* const, global_forward2d);
REFERENCE_DECLARE(0x0189CEF4, real_vector2d const* const, global_left2d);
REFERENCE_DECLARE(0x0189CEF8, real_vector2d const* const, global_backward2d);
REFERENCE_DECLARE(0x0189CEFC, real_vector2d const* const, global_right2d);
REFERENCE_DECLARE(0x0189CF00, real_point3d const* const, global_origin3d);
REFERENCE_DECLARE(0x0189CF04, real_vector3d const* const, global_x_axis3d);
REFERENCE_DECLARE(0x0189CF08, real_vector3d const* const, global_y_axis3d);
REFERENCE_DECLARE(0x0189CF0C, real_vector3d const* const, global_z_axis3d);
REFERENCE_DECLARE(0x0189CF10, real_vector3d const* const, global_negative_x_axis3d);
REFERENCE_DECLARE(0x0189CF14, real_vector3d const* const, global_negative_y_axis3d);
REFERENCE_DECLARE(0x0189CF18, real_vector3d const* const, global_negative_z_axis3d);
REFERENCE_DECLARE(0x0189CF1C, real_vector3d const* const, global_zero_vector3d);
REFERENCE_DECLARE(0x0189CF20, real_vector3d const* const, global_forward3d);
REFERENCE_DECLARE(0x0189CF24, real_vector3d const* const, global_left3d);
REFERENCE_DECLARE(0x0189CF28, real_vector3d const* const, global_up3d);
REFERENCE_DECLARE(0x0189CF2C, real_vector3d const* const, global_backward3d);
REFERENCE_DECLARE(0x0189CF30, real_vector3d const* const, global_right3d);
REFERENCE_DECLARE(0x0189CF34, real_vector3d const* const, global_down3d);
REFERENCE_DECLARE(0x0189CF38, real_euler_angles2d const* const, global_zero_angles2d);
REFERENCE_DECLARE(0x0189CF3C, real_euler_angles3d const* const, global_zero_angles3d);
REFERENCE_DECLARE(0x0189CF40, real_vector4d const* const, global_zero_vector4d);
REFERENCE_DECLARE(0x0189CF44, real_quaternion const* const, global_identity_quaternion);
REFERENCE_DECLARE(0x0189CF48, real_matrix3x3 const* const, global_identity3x3);
REFERENCE_DECLARE(0x0189CF4C, real_matrix4x3 const* const, global_identity4x3);
REFERENCE_DECLARE(0x0189CF50, real_matrix4x3 const* const, global_negative_identity4x3);
REFERENCE_DECLARE(0x0189CF54, real_orientation const* const, global_identity_orientation);
REFERENCE_DECLARE(0x0189CF58, real_rectangle2d const* const, global_null_rectangle2d);
REFERENCE_DECLARE(0x0189CF5C, real_rectangle3d const* const, global_null_rectangle3d);
REFERENCE_DECLARE(0x0189CF60, real_rectangle2d const* const, global_zero_rectangle2d);
REFERENCE_DECLARE(0x0189CF64, real_rectangle3d const* const, global_zero_rectangle3d);

// 0164F660
short const global_projection3d_mappings[3][2][3] =
{
	{
		{ _z, _y, _x },
		{ _y, _z, _x }
	},
	{
		{ _x, _z, _y },
		{ _z, _x, _y }
	},
	{
		{ _y, _x, _z },
		{ _x, _y, _z }
	}
};

// 0164F684
short const global_projection3d_inverse_mappings[3][2][3] =
{
	{
		{ _z, _y, _x },
		{ _z, _x, _y }
	},
	{
		{ _x, _z, _y },
		{ _y, _z, _x }
	},
	{
		{ _y, _x, _z },
		{ _x, _y, _z }
	}
};

bool __cdecl valid_real_vector2d(real_vector2d const* v)
{
	return valid_real(v->i)
		&& valid_real(v->j);
}

bool __cdecl valid_real_vector4d(real_vector4d const* v)
{
	return valid_real(v->i)
		&& valid_real(v->j)
		&& valid_real(v->k)
		&& valid_real(v->l);
}

bool __cdecl valid_real_sine_cosine(real sine, real cosine)
{
	return valid_realcmp(((sine * sine) + (cosine * cosine)), 1.0f);
}

bool __cdecl valid_real_quaternion(real_quaternion const* quaternion)
{
	return valid_realcmp((magnitude_squared3d(&quaternion->v) + (quaternion->w * quaternion->w)), 1.0f);
}

bool __cdecl valid_real_normal2d(real_vector2d const* normal)
{
	return valid_realcmp(magnitude_squared2d(normal), 1.0f);
}

bool __cdecl valid_real_plane2d(real_plane2d const* plane)
{
	return valid_real_normal2d(&plane->n)
		&& valid_real(plane->d);
}

bool __cdecl valid_real_plane3d(real_plane3d const* plane)
{
	return valid_real_normal3d(&plane->n)
		&& valid_real(plane->d);
}

bool __cdecl valid_real_vector3d_right_handed_axes3(real_vector3d const* forward, real_vector3d const* left, real_vector3d const* up)
{
	return valid_real_vector3d_axes3(forward, left, up)
		&& triple_product3d(forward, left, up) > 0.0f;
}

bool __cdecl valid_real_matrix4x3(real_matrix4x3 const* matrix)
{
	return valid_real(matrix->scale)
		&& valid_real_vector3d_axes3(&matrix->forward, &matrix->left, &matrix->up)
		&& valid_real_point3d(&matrix->position);
}

bool __cdecl valid_real_euler_angles3d(real_euler_angles3d const* angles)
{
	return valid_real(angles->yaw)
		&& valid_real(angles->pitch)
		&& valid_real(angles->roll)
		&& IN_RANGE_INCLUSIVE(angles->yaw, 0.0f, k_full_circle)
		&& IN_RANGE_INCLUSIVE(angles->pitch, -k_quarter_circle, k_quarter_circle)
		&& IN_RANGE_INCLUSIVE(angles->roll, -k_half_circle, k_half_circle);
}

// networking/network_configuration.cpp
real __cdecl interpolate_linear(real start_value, real end_value, real interpolation_factor)
{
	return start_value + ((end_value - start_value) * interpolation_factor);
}

bool __cdecl valid_real(real const& value)
{
	//return INVOKE(0x004AB2A0, valid_real, value);

	return ((dword)value & 0x7F800000) != 0x7F800000;
}

bool __cdecl valid_real_euler_angles2d(real_euler_angles2d const* angles)
{
	//return INVOKE(0x004AB2C0, valid_real_euler_angles2d, angles);

	return valid_real(angles->yaw)
		&& valid_real(angles->pitch)
		&& IN_RANGE_INCLUSIVE(angles->yaw, 0.0f, k_full_circle)
		&& IN_RANGE_INCLUSIVE(angles->pitch, -k_quarter_circle, k_quarter_circle);
}

bool __cdecl valid_real_normal3d(real_vector3d const* normal)
{
	//return INVOKE(0x004AB330, valid_real_normal3d, normal);

	return valid_realcmp(magnitude_squared3d(normal), 1.0f);
}

bool __cdecl valid_real_point3d(real_point3d const* point)
{
	//return INVOKE(0x004AB3A0, valid_real_point3d, point);

	return valid_real(point->x)
		&& valid_real(point->y)
		&& valid_real(point->z);
}

bool __cdecl valid_real_point2d(real_point2d const* point)
{
	//return INVOKE(0x004AB3E0, valid_real_point2d, point);

	return valid_real(point->x)
		&& valid_real(point->y);
}

bool __cdecl valid_real_vector3d(real_vector3d const* vector)
{
	//return INVOKE(0x004AB410, valid_real_vector3d, vector);

	return valid_real(vector->i)
		&& valid_real(vector->j)
		&& valid_real(vector->k);
}

bool __cdecl valid_real_vector3d_axes2(real_vector3d const* forward, real_vector3d const* up)
{
	//return INVOKE(0x004AB450, valid_real_vector3d_axes2, forward, up);

	return valid_real_normal3d(forward)
		&& valid_real_normal3d(up)
		&& valid_realcmp(dot_product3d(forward, up), 0.0f);
}

bool __cdecl valid_realcmp(real a, real b)
{
	//return INVOKE(0x004AB570, valid_realcmp, a, b);

	real diff = a - b;
	return valid_real(diff)
		&& fabs(diff) < k_test_real_epsilon;
}

real __cdecl angle_between_vectors3d(real_vector3d const* a, real_vector3d const* b)
{
	return INVOKE(0x004EEC40, angle_between_vectors3d, a, b);
}

real_euler_angles2d* __cdecl euler_angles2d_from_vector3d(real_euler_angles2d* facing, real_vector3d const* forward)
{
	return INVOKE(0x004F13E0, euler_angles2d_from_vector3d, facing, forward);
}

real_vector3d* __cdecl generate_up_vector3d(real_vector3d const* forward, real_vector3d* up)
{
	return INVOKE(0x004F2310, generate_up_vector3d, forward, up);
}

real_point3d* __cdecl project_point2d(real_point2d const* p2d, real_plane3d const* plane, short projection, bool sign, real_point3d* p3d)
{
	//return INVOKE(0x004F9830, project_point2d, p2d, plane, projection, sign, p3d);

	short v5 = global_projection3d_mappings[projection][sign][_x];
	short v6 = global_projection3d_mappings[projection][sign][_y];

	real v7 = 0.0f;
	if (fabsf((plane->n.n[projection] - 0.0f)) >= k_real_epsilon)
		v7 = ((plane->d - (plane->n.n[v5] * p2d->n[0])) - (plane->n.n[v6] * p2d->n[1])) / plane->n.n[projection];

	ASSERT(projection >= _x && projection <= _z);
	p3d->n[v5] = p2d->n[0];
	p3d->n[v6] = p2d->n[1];
	p3d->n[projection] = v7;

	return p3d;
}

//.text:004F98F0 ; real_point2d* __cdecl project_point3d(real_point3d const*, short, bool, real_point2d*)
//.text:004F9930 ; void __cdecl project_polygon2d(long, real_point2d const* const, real_plane3d const*, long, bool, real_point3d* const)
//.text:004F9A20 ; void __cdecl project_polygon3d(long, real_point3d const* const, long, bool, real_point2d* const)
//.text:004F9A80 ; real_vector2d* __cdecl project_vector3d(real_vector3d const *, short, bool, real_vector2d*)

void __cdecl quaternion_transform_point(real_quaternion const* q, real_point3d const* p, real_point3d* result)
{
	INVOKE(0x004FAAF0, quaternion_transform_point, q, p, result);
}

void __cdecl real_math_dispose()
{
	INVOKE(0x004FB7D0, real_math_dispose);

	//periodic_functions_dispose();
}

//.text:004FB7E0 ; void __cdecl real_math_enable_floating_point_exceptions(bool)

void __cdecl real_math_initialize()
{
	INVOKE(0x004FB7F0, real_math_initialize);

	//real_math_reset_precision();
	//periodic_functions_initialize();
}

//.text:004FB820 ; void __cdecl real_math_recover_fpu_exceptions()

void __cdecl real_math_reset_precision()
{
	INVOKE(0x004FB850, real_math_reset_precision);
}

//.text:004FB880
//.text:004FB890
//.text:004FB920
//.text:004FB960 ; real __cdecl real_rectangle2d_area(real_rectangle2d const*)

void __cdecl real_rectangle2d_clamp_bounds(real_rectangle2d* clampee, real_rectangle2d const* clamper)
{
	INVOKE(0x004FB990, real_rectangle2d_clamp_bounds, clampee, clamper);
}

bool __cdecl real_rectangle2d_compute_intersection(real_rectangle2d const* a, real_rectangle2d const* b, real_rectangle2d* result)
{
	return INVOKE(0x004FB9E0, real_rectangle2d_compute_intersection, a, b, result);
}

//.text:004FBAA0 ; real_rectangle2d* __cdecl real_rectangle2d_enclose_point(real_rectangle2d*, real_point2d const*)
//.text:004FBB30 ; real_rectangle2d* __cdecl real_rectangle2d_enclose_points(real_rectangle2d*, long, real_point2d const* const)
//.text:004FBBE0 ; real_rectangle2d* __cdecl real_rectangle2d_enclose_rectangle(real_rectangle2d*, real_rectangle2d const*)
//.text:004FBC70 ; bool __cdecl real_rectangle3d_compute_intersection(real_rectangle3d const*, real_rectangle3d const*, real_rectangle3d*)

real_rectangle3d* __cdecl real_rectangle3d_enclose_point(real_rectangle3d* bounds, real_point3d const* point)
{
	//return INVOKE(0x004FBD80, real_rectangle3d_enclose_point, bounds, point);

	if (bounds->x0 > point->x)
		bounds->x0 = point->x;

	if (bounds->x1 < point->x)
		bounds->x1 = point->x;

	if (bounds->y0 > point->y)
		bounds->y0 = point->y;

	if (bounds->y1 < point->y)
		bounds->y1 = point->y;

	if (bounds->z0 > point->z)
		bounds->z0 = point->z;

	if (bounds->z1 < point->z)
		bounds->z1 = point->z;

	return bounds;
}

real_rectangle3d* __cdecl real_rectangle3d_enclose_points(real_rectangle3d* bounds, long point_count, real_point3d const* points)
{
	//return INVOKE(0x004FBE40, real_rectangle3d_enclose_points, bounds, point_count, points);

	//ASSERT(valid_polygon3d(point_count, points));

	for (long i = 0; i < point_count; i++)
		real_rectangle3d_enclose_point(bounds, &points[i]);

	return bounds;
}

real_rectangle3d* __cdecl real_rectangle3d_enclose_rectangle(real_rectangle3d* bounds, real_rectangle3d const* rectangle)
{
	//return INVOKE(0x004FBF30, real_rectangle3d_enclose_rectangle, bounds, rectangle);

	real x_lower = bounds->x0;
	if (x_lower > rectangle->x0)
		x_lower = rectangle->x0;
	bounds->x0 = x_lower;

	real x_upper = rectangle->x1;
	if (bounds->x1 > x_upper)
		x_upper = bounds->x1;
	bounds->x1 = x_upper;

	real y_lower = rectangle->y0;
	if (bounds->y0 <= y_lower)
		y_lower = bounds->y0;
	bounds->y0 = y_lower;

	real y_upper = rectangle->y1;
	if (bounds->y1 > y_upper)
		y_upper = bounds->y1;
	bounds->y1 = y_upper;

	real z_lower = rectangle->z0;
	if (bounds->z0 <= z_lower)
		z_lower = bounds->z0;
	bounds->z0 = z_lower;

	real z_upper = rectangle->z1;
	if (bounds->z1 > z_upper)
		z_upper = bounds->z1;
	bounds->z1 = z_upper;

	return bounds;
}

//.text:004FBFF0 ; real __cdecl real_rectangle3d_volume(real_rectangle3d const*)
//.text:004FC030 ; void __cdecl real_vector3d_build_axes_from_forward(real_vector3d const*, real_vector3d*, real_vector3d*)
//.text:004FC060 ; void __cdecl real_vector3d_build_axes_from_forward_and_left(real_vector3d const*, real_vector3d const*, real_vector3d*, real_vector3d*, real_vector3d*)
//.text:004FC610 ; void __cdecl real_vector3d_build_axes_from_vectors(real_vector3d const*, real_vector3d const*, real_vector3d*, real_vector3d*, real_vector3d*)
//.text:004FCA60 ; void __cdecl real_vector3d_build_axes_from_vectors_using_up(real_vector3d const*, real_vector3d const*, real_vector3d*, real_vector3d*, real_vector3d*)
//.text:004FCEB0 ; 

long __cdecl rectangle3d_build_edges(real_rectangle3d const* bounds, long maximum_edge_count, real_point3d(* const edges)[2])
{
	//return INVOKE(0x004FCF10, rectangle3d_build_edges, bounds, maximum_edge_count, edges);

	ASSERT(bounds);
	ASSERT(maximum_edge_count >= k_edges_per_cube_count);
	ASSERT(edges);

	long line_vertex_indices[k_edges_per_cube_count][2]
	{
		{ 0, 2 },
		{ 2, 3 },
		{ 3, 1 },
		{ 1, 0 },
		{ 0, 4 },
		{ 1, 5 },
		{ 2, 6 },
		{ 3, 7 },
		{ 4, 5 },
		{ 5, 7 },
		{ 7, 6 },
		{ 6, 4 },
	};

	real_point3d vertices[k_vertices_per_cube_count]{};
	rectangle3d_build_vertices(bounds, k_vertices_per_cube_count, vertices);

	for (long edge_index = 0; edge_index < k_edges_per_cube_count; edge_index++)
	{
		ASSERT((line_vertex_indices[edge_index][0] >= 0) && (line_vertex_indices[edge_index][0] < k_vertices_per_cube_count));
		ASSERT((line_vertex_indices[edge_index][1] >= 0) && (line_vertex_indices[edge_index][1] < k_vertices_per_cube_count));

		edges[edge_index][0] = vertices[line_vertex_indices[edge_index][0]];
		edges[edge_index][1] = vertices[line_vertex_indices[edge_index][1]];
	}

	return k_edges_per_cube_count;
}

long __cdecl rectangle3d_build_faces(real_rectangle3d const* bounds, long maximum_face_count, real_point3d(* const faces)[4])
{
	//return INVOKE(0x004FD030, rectangle3d_build_faces, bounds, maximum_face_count, faces);

	ASSERT(bounds);
	ASSERT(maximum_face_count >= k_faces_per_cube_count);
	ASSERT(faces);

	long face_vertex_indices[k_faces_per_cube_count][4]
	{
		{ 0, 2, 3, 1 },
		{ 0, 1, 5, 4 },
		{ 1, 3, 7, 5 },
		{ 2, 6, 7, 3 },
		{ 0, 4, 6, 2 },
		{ 4, 5, 7, 6 },
	};

	real_point3d vertices[k_vertices_per_cube_count]{};
	rectangle3d_build_vertices(bounds, k_vertices_per_cube_count, vertices);

	for (long face_index = 0; face_index < k_faces_per_cube_count; face_index++)
	{
		for (long vertex_index = 0; vertex_index < k_vertices_per_cube_count; vertex_index++)
		{
			ASSERT((face_vertex_indices[face_index][vertex_index] >= 0) && (face_vertex_indices[face_index][vertex_index] < k_vertices_per_cube_count));

			*faces[face_vertex_indices[face_index][vertex_index]] = vertices[face_vertex_indices[face_index][vertex_index]];
		}
	}

	return k_faces_per_cube_count;
}

long __cdecl rectangle3d_build_vertices(real_rectangle3d const* bounds, long maximum_vertex_count, real_point3d* const vertices)
{
	//return INVOKE(0x004FD1A0, rectangle3d_build_vertices, bounds, maximum_vertex_count, vertices);

	ASSERT(bounds);
	ASSERT(maximum_vertex_count >= k_vertices_per_cube_count);
	ASSERT(vertices);

	vertices[0].x = bounds->x0;
	vertices[0].y = bounds->y0;
	vertices[0].z = bounds->z0;
	vertices[1].x = bounds->x1;
	vertices[1].y = bounds->y0;
	vertices[1].z = bounds->z0;
	vertices[2].x = bounds->x0;
	vertices[2].y = bounds->y1;
	vertices[2].z = bounds->z0;
	vertices[3].x = bounds->x1;
	vertices[3].y = bounds->y1;
	vertices[3].z = bounds->z0;
	vertices[4].x = bounds->x0;
	vertices[4].y = bounds->y0;
	vertices[4].z = bounds->z1;
	vertices[5].x = bounds->x1;
	vertices[5].y = bounds->y0;
	vertices[5].z = bounds->z1;
	vertices[6].x = bounds->x0;
	vertices[6].y = bounds->y1;
	vertices[6].z = bounds->z1;
	vertices[7].x = bounds->x1;
	vertices[7].y = bounds->y1;
	vertices[7].z = bounds->z1;

	return k_vertices_per_cube_count;
}

//.text:004FD240 ; void __cdecl rectangle3d_compute_nearest_point(real_rectangle3d const*, real_point3d const*, real_point3d*)
//.text:004FD2D0 ; real_vector3d* __cdecl reflect_vector3d(real_vector3d const*, real_vector3d const*, real_vector3d*)
//.text:004FD350 ; 
//.text:004FD560 ; real_vector3d* __cdecl rotate_vector_about_axis(real_vector3d*, real_vector3d const*, real, real)
//.text:004FD660 ; 
//.text:004FD6F0 ; void __cdecl scalars_interpolate(real, real, real, real*)
//.text:004FD720 ; 
//.text:004FD780 ; 
//.text:004FD7B0 ; 
//.text:004FD7D0 ; 
//.text:004FD800 ; 
//.text:004FD830 ; 
//.text:004FD850 ; 
//.text:004FD880 ; real __cdecl signed_angle_between_normals3d(real_vector3d const*, real_vector3d const*, real_vector3d const*)
//.text:004FD9C0 ; 
//.text:004FDB20 ; real __cdecl signed_angle_between_vectors2d(real_vector2d const*, real_vector2d const*)
//.text:004FDBF0 ; 
//.text:004FDC10 ; 
//.text:004FDC30 ; bool __cdecl sphere_intersects_cone3d(real_point3d const*, real, real_point3d const*, real_vector3d const*, real, real, real)
//.text:004FDCF0 ; bool __cdecl sphere_intersects_rectangle3d(real_point3d const*, real, real_rectangle3d const*)
//.text:004FDDA0 ; bool __cdecl sphere_intersects_sector3d(real_point3d const*, real, real_point3d const*, real_vector3d const*, real, real, real)
//.text:004FDEE0 ; bool __cdecl sphere_intersects_triangle3d(real_point3d const*, real, real_point3d const*, real_point3d const*, real_point3d const*)
//.text:004FE4D0 ;
//.text:004FE710 ; 
//.text:004FE740 ; 

real_vector3d* __cdecl vector3d_from_angle(real_vector3d* vector, real angle)
{
	return INVOKE(0x004FF020, vector3d_from_angle, vector, angle);

	//vector->i = cosf(angle);
	//vector->j = sinf(angle);
	//vector->k = 0.0f;
	//return vector;
}

real_vector3d* __cdecl vector3d_from_euler_angles2d(real_vector3d* vector, real_euler_angles2d const* angles)
{
	return INVOKE(0x004FF060, vector3d_from_euler_angles2d, vector, angles);

	//vector->i = cosf(angles->yaw) * cosf(angles->pitch);
	//vector->j = sinf(angles->yaw) * cosf(angles->pitch);
	//vector->k = sinf(angles->pitch);
	//return vector;
}

//.text:004FF0D0 ; 
//.text:004FF100 ;
//.text:004FF170 ; bool __cdecl vector_intersects_pill2d(real_point2d const*, real_vector2d const*, real_point2d const*, real_vector2d const*, real)
//.text:004FF5B0 ; bool __cdecl vector_intersects_pill3d(real_point3d const*, real_vector3d const*, real_point3d const*, real_vector3d const*, real)
//.text:004FFD80 ; bool __cdecl vector_intersects_rectangle2d(real_point2d const*, real_vector2d const*, real_rectangle2d const*)
//.text:004FFEF0 ; 
//.text:00500120 ; 
//.text:00500160 ; 
//.text:005003A0 ; 
//.text:005005F0 ; real __cdecl vector_to_line_distance_squared3d(real_point3d const*, real_vector3d const*, real_point3d const*, real_vector3d const*)
//.text:00500FF0 ; void __cdecl vectors3d_from_euler_angles2d(real_vector3d*, real_vector3d*, real_euler_angles2d const*)
//.text:00501040 ; void __cdecl vectors3d_from_euler_angles3d(real_vector3d*, real_vector3d*, real_euler_angles3d const*)
//.text:00501090 ; 
//.text:00501100 ; void __cdecl yaw_vectors(real_vector3d*, real_vector3d const*, real, real)
//.text:005011A0 ; 
//.text:005011C0 ; 
//.text:005011E0 ; 
//.text:00501230 ; 
//.text:00501250 ; 
//.text:00501270 ; 
//.text:005012B0 ; 
//.text:00501300 ; 
//.text:00501340 ; 

//.text:00523B20 ; 
//.text:00523B70 ; 

bool __cdecl valid_real_vector3d_axes3(real_vector3d const* forward, real_vector3d const* left, real_vector3d const* up)
{
	//return INVOKE(0x005B3D30, valid_real_vector3d_axes3, forward, left, up);

	return valid_real_normal3d(forward)
		&& valid_real_normal3d(left)
		&& valid_real_normal3d(up)
		&& valid_realcmp(dot_product3d(forward, left), 0.0f)
		&& valid_realcmp(dot_product3d(left, up), 0.0f)
		&& valid_realcmp(dot_product3d(up, forward), 0.0f);
}

bool __cdecl valid_polygon2d(long point_count, real_point2d const* const points)
{
	//return INVOKE(0x0078AF90, valid_polygon2d, point_count, points);

	ASSERT(point_count >= 0);
	ASSERT(point_count == 0 || points);

	for (long i = 0; i < point_count; i++)
	{
		if (!valid_real_point2d(&points[i]))
			return false;
	}

	return true;
}

//.text:0078B440 ; 
//.text:0078BF90 ; 
//.text:00C35C30 ; 

real_vector3d* __cdecl add_vectors3d(real_vector3d const* a, real_vector3d const* b, real_vector3d* result)
{
	result->i = a->i + b->i;
	result->j = a->j + b->j;
	result->k = a->k + b->k;

	return result;
}

real __cdecl arctangent(real y, real x)
{
	return atan2f(y, x); // atan2
}

real_vector3d* __cdecl cross_product3d(real_vector3d const* a, real_vector3d const* b, real_vector3d* result)
{
	result->i = (a->j * b->k) - (a->k * b->j);
	result->j = (a->k * b->i) - (a->i * b->k);
	result->k = (a->i * b->j) - (a->j * b->i);

	return result;
}

real __cdecl distance_squared3d(real_point3d const* a, real_point3d const* b)
{
	real_vector3d temp{};
	return magnitude_squared3d(vector_from_points3d(a, b, &temp));
}

real __cdecl distance3d(real_point3d const* a, real_point3d const* b)
{
	return square_root(distance_squared3d(a, b));
}

real __cdecl dot_product3d(real_vector3d const* a, real_vector3d const* b)
{
	return ((a->i * b->i) + (a->j * b->j)) + (a->k * b->k);
}

real __cdecl magnitude_squared2d(real_vector2d const* v)
{
	return (v->i * v->i) + (v->j * v->j);
}

real __cdecl magnitude_squared3d(real_vector3d const* v)
{
	return (v->i * v->i) + (v->j * v->j) + (v->k * v->k);
}

real __cdecl magnitude2d(real_vector2d const* v)
{
	return square_root(magnitude_squared2d(v));
}

real __cdecl magnitude3d(real_vector3d const* v)
{
	return square_root(magnitude_squared3d(v));
}

real_vector3d* __cdecl negate_vector3d(real_vector3d const* a, real_vector3d* result)
{
	result->i = -a->i;
	result->j = -a->j;
	result->k = -a->k;

	return result;
}

real __cdecl normalize3d(real_vector3d* v)
{
	real magnitude = magnitude3d(v);
	if (fabsf(magnitude - 0.0f) < k_real_epsilon)
		magnitude = 0.0f;
	else
		scale_vector3d(v, 1.0f / magnitude, v);

	return magnitude;
}

real_vector3d* __cdecl perpendicular3d(real_vector3d const* a, real_vector3d* result)
{
	real i = fabsf(a->i);
	real j = fabsf(a->j);
	real k = fabsf(a->k);
	if (i > j || i > k)
	{
		if (j > k)
		{
			result->i = a->j;
			result->j = -a->i;
			result->k = 0.0f;
		}
		else
		{
			result->i = -a->k;
			result->j = 0.0f;
			result->k = a->i;
		}
	}
	else
	{
		result->i = 0.0f;
		result->j = a->k;
		result->k = -a->j;
	}

	return result;
}

real __cdecl plane3d_distance_to_point(real_plane3d const* plane, real_point3d const* point)
{
	return ((((point->x * plane->n.i) + (point->y * plane->n.j)) + (point->z * plane->n.k)) - plane->d);
}

real_point3d* __cdecl point_from_line3d(real_point3d const* p, real_vector3d const* v, real t, real_point3d* result)
{
	result->x = p->x + (v->i * t);
	result->y = p->y + (v->j * t);
	result->z = p->z + (v->k * t);

	return result;
}

real_point2d* __cdecl point_from_line2d(real_point2d const* p, real_vector2d const* v, real t, real_point2d* result)
{
	result->x = p->x * (v->i * t);
	result->y = p->y * (v->j * t);

	return result;
}

bool __cdecl point_in_sphere(real_point3d const* point, real_point3d const* center, real radius)
{
	return radius * radius > distance_squared3d(point, center);
}

bool __cdecl point_intersects_rectangle2d(real_point2d const* p, real_rectangle2d const* bounds)
{
	return p->x >= bounds->x0 && bounds->x1 >= p->x && p->y >= bounds->y0 && bounds->y1 >= p->y;
}

real_vector2d* __cdecl rotate_vector2d(real_vector2d const* v, real sine, real cosine, real_vector2d* result)
{
	result->i = (cosine * v->i) - (sine * v->j);
	result->j = (sine * v->i) + (cosine * v->j);

	return result;
}

real_vector3d* __cdecl scale_vector3d(real_vector3d const* a, real c, real_vector3d* result)
{
	result->i = c * a->i;
	result->j = c * a->j;
	result->k = c * a->k;

	return result;
}

real_point2d* __cdecl set_real_point2d(real_point2d* p, real x, real y)
{
	p->x = x;
	p->y = y;

	return p;
}

real_point3d* __cdecl set_real_point3d(real_point3d* p, real x, real y, real z)
{
	p->x = x;
	p->y = y;
	p->z = z;

	return p;
}

real_rectangle2d* __cdecl set_real_rectangle2d(real_rectangle2d* bounds, real x0, real x1, real y0, real y1)
{
	bounds->x0 = x0;
	bounds->x1 = x1;
	bounds->y0 = y0;
	bounds->y1 = y1;

	return bounds;
}

real_rectangle3d* __cdecl set_real_rectangle3d(real_rectangle3d* bounds, real x0, real x1, real y0, real y1, real z0, real z1)
{
	bounds->x0 = x0;
	bounds->x1 = x1;
	bounds->y0 = y0;
	bounds->y1 = y1;
	bounds->z0 = z0;
	bounds->z1 = z1;

	return bounds;
}

real_vector2d* __cdecl set_real_vector2d(real_vector2d* vector, real i, real j)
{
	vector->i = i;
	vector->j = j;

	return vector;
}

real_vector3d* __cdecl set_real_vector3d(real_vector3d* vector, real i, real j, real k)
{
	vector->i = i;
	vector->j = j;
	vector->k = k;

	return vector;
}

real_vector4d* __cdecl set_real_vector4d(real_vector4d* result, real i, real j, real k, real l)
{
	result->i = i;
	result->j = j;
	result->k = k;
	result->l = l;

	return result;
}

real __cdecl square_root(real x)
{
	return sqrtf(x); // sqrt
}

real __cdecl triple_product3d(real_vector3d const* a, real_vector3d const* b, real_vector3d const* n)
{
	real_vector3d product3d;
	return dot_product3d(cross_product3d(a, b, &product3d), n);
}

real_vector2d* __cdecl vector_from_points2d(real_point2d const* a, real_point2d const* b, real_vector2d* result)
{
	result->i = b->x - a->x;
	result->j = b->y - a->y;

	return result;
}

real_vector3d* __cdecl vector_from_points3d(real_point3d const* a, real_point3d const* b, real_vector3d* result)
{
	result->i = b->x - a->x;
	result->j = b->y - a->y;
	result->k = b->z - a->z;

	return result;
}

