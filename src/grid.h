#ifndef GRID_H
#define GRID_H

#include "common.h"

class Grid
{
public:
	const f32 dx;
	const f32 dt;
	const f32 cour;
	const f32 M;
	
	const f32 X;
	const f32 T;

	const u64 Nx;
	const u64 Nt;

	const f32 dt2;

	Grid(f32 X, f32 dx, f32 T, u64 cour);
	Grid(const Grid& other);
	~Grid();

	void print_info();
};

#endif