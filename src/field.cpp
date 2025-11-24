#include "field.h"

Field::Field(Grid grid, u64 size, string name)
	: grid(grid), Nx(size), time_steps(3), name(name), initialized(false)
{

}

Field::Field(Grid grid, string name) 
	: grid(grid), Nx(grid.Nx), time_steps(3), name(name), initialized(false)
{

}

void Field::deallocate()
{
    delete core;
}

void Field::init(std::unordered_map<string, Field*> &indexable_map)
{
	initialized = true;
	core = (f32*)calloc(Nx * time_steps, sizeof(f32));
	clear();
	ns.resize(time_steps);
	for (u64 i = 0; i < ns.size(); ++i)
	{
		ns[i] = i;
	}
	indexable_map[name] = this;
}

void Field::rotate_ns()
{
	for (u64 i = 0; i < ns.size(); ++i)
	{
		ns[i] = ns[i] + 1;
		if (ns[i] == ns.size())
		{
			ns[i] = 0;
		}
	}
}

Field::~Field()
{
	free(core);
}

void Field::write_source(Source_Interface& source, u64 nt, u64 nx)
{
	f32 cour = 1.0f / grid.cour;
	operator()(nx, 2) += cour * (source((nt + 1) * grid.dt) - source((nt - 1) * grid.dt));
}


void Field::step()
{
	Field &E = *this;
	for (u64 i = 1; i < grid.Nx - 1; i++)
	{
		E(i, 2) = 2.0f * (1.0f - grid.M) * E(i, 1) 
			- E(i, 0) 
			+ grid.M * (E(i + 1, 1) + E(i - 1, 1));
	}
}

void Field::step_constrained(u64 edge_1, u64 edge_2)
{
	Field& E = *this;
	for (u64 i = 1; i < edge_1; i++)
	{
		E(i, 2) = 2.0f * (1.0f - grid.M) * E(i, 1) 
			- E(i, 0) 
			+ grid.M * (E(i + 1, 1) + E(i - 1, 1));
	}
	for (u64 i = edge_2 - 1; i < grid.Nx - 1; i++)
	{
		E(i, 2) = 2.0f * (1.0f - grid.M) * E(i, 1) 
			- E(i, 0) 
			+ grid.M * (E(i + 1, 1) + E(i - 1, 1));
	}
}


void Field::boundaries()
{
	Field& E = *this;
	f32 cour = 1.0f / grid.cour;
	E(0, 2) = E(1, 1) + ((cour - 1.0f) / (cour + 1.0f)) * (E(1, 2) - E(0, 1));
	E(grid.Nx-1, 2) = E(grid.Nx - 2, 1) + ((cour - 1.0f) / (cour + 1.0f)) 
		* (E(grid.Nx - 2, 2) - E(grid.Nx - 1, 1));
}

void Field::clear()
{
	for (u64 i = 0; i < Nx * time_steps; ++i)
	{
		core[i] = 0;
	}
}