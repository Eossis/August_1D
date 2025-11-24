#ifndef FILM_H
#define FILM_H

#include "grid.h"
#include "field.h"
#include "lorentzian.h"
#include "material.h"
#include "material_oscillators.h"
#include "material_single_index.h"

class Film
{
private:

public:
	const Grid grid;
	Material_Interface *material = NULL;
	const bool enabled;
	const f32 width;
	const u64 N;
	const u64 film_start;
	const u64 film_end;

	Film(Grid grid, f32 width);
	Film();
	Film(const Film& other);
	~Film();

	void set_material(Material_Interface* material_in) { material = material_in; }
	Material_Interface* get_material() { return material; }
	void step(Field *field);
	void init(std::unordered_map<string, Field*> &indexable_map);

	void rotate_ns();
};

#endif