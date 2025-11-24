#include "grid.h"

Grid::Grid(f32 X_in, f32 dx_in, f32 T_in, u64 cour_in) 
	: X(X_in), 
	dx(dx_in), 
	T(T_in), 
	cour(cour_in),
	dt(dx_in / (c * (f32)cour_in)), 
	dt2(dt * dt),
	M(pow((c * (dx_in / (c * (f32)cour_in)) / dx_in), 2.0f)),
	Nt((u64)round(T_in / (dx_in / (c * (f32)cour_in)))),
	Nx((u64)round(X_in / dx_in))
{

}

Grid::Grid(const Grid& other)
	: X(other.X),
	dx(other.dx),
	T(other.T),
	cour(other.cour),
	dt(other.dt),
	dt2(other.dt2),
	M(other.M),
	Nt(other.Nt),
	Nx(other.Nx)
{

}

Grid::~Grid()
{
}

void Grid::print_info()
{
	printf("Grid parameters \n");
	printf("Nx:%u X:%2.2e dx:%2.2e \n", Nx, X, dx);
	printf("Nt:%u T:%2.2e dt:%2.2e \n\n", Nt, T, dt);
}