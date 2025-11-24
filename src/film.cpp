#include "film.h"

Film::Film(Grid grid, f32 width) 
	: grid(grid),
	width(width), 
	N((u64)floor(width / grid.dx)),
	film_start((u64)floor(floor((f32)grid.Nx / 2.0f)) - (u64)floor((f32)N / 2.0f)),
	film_end((u64)floor(floor((f32)grid.Nx / 2.0f)) + (u64)ceil((f32)N / 2.0f) + 1),
	enabled(true)
{

}

Film::Film()
	: grid(Grid(0, 0, 0, 1)),
	width(0),
	N(0),
	film_start(0),
	film_end(0),
	enabled(false)
{

}

Film::Film(const Film& other)
	: grid(other.grid),
	width(other.width),
	N(other.N),
	film_start(other.film_start),
	film_end(other.film_end),
	enabled(other.enabled)
{
	if (enabled)
	{
		material = other.material->clone();
	}
}


void Film::step(Field *field)
{
	f32 dt = grid.dt;
	f32 dx = grid.dx;
	u64 j;
	for (u64 i = film_start; i < film_end - 1; ++i)
	{
		j = i - film_start;
		material->step(*field, j, i);
	}
}

Film::~Film()
{
	delete material;
}

void Film::init(std::unordered_map<string, Field*> &indexable_map)
{
	if (enabled)
	{
		material->init(indexable_map);
	}
}

void Film::rotate_ns()
{
	if (enabled)
	{
		material->rotate_ns();
	}
}