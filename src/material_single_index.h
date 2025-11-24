#ifndef MATERIAL_SINGLE_INDEX_H
#define MATERIAL_SINGLE_INDEX_H

#include "material.h"
#include "grid.h"
#include "common.h"

class Material_Single_Index : public Material_Interface
{
public:
	const Grid grid;
	const u64 N;
	const f32 index;
	const f32 M;

	Material_Single_Index(u64 N_in, f32 index_in, Grid grid_in) :
		Material_Interface(Material_Interface::Type::SINGLE_INDEX),
		grid(grid_in),
		N(N_in), 
		index(index_in),
		M((c2 * grid.dt2 / (grid.dx * index * grid.dx * index)))
	{

	}

	Material_Single_Index *clone() 
	{
		return new Material_Single_Index(N, index, grid);
	}

	c32 get_index_from_wavelength(f32 wl)
	{
        UNUSED(wl);
		return (c32)index;
	}

	inline void step(Field& E, u64 film_index, u64 i)
	{
        UNUSED(film_index);
		E(i, 2) = 2.0f * (1.0f - M) * E(i, 1)
			- E(i, 0)
			+ M * (E(i + 1, 1) + E(i - 1, 1));
	}

	void init(std::unordered_map<string, Field*> &indexable_map)
	{
        UNUSED(indexable_map);
	}

	virtual void rotate_ns()
	{

	}
};

#endif