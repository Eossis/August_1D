#ifndef MATERIAL_OSCILLATORS_H
#define MATERIAL_OSCILLATORS_H

#include "common.h"
#include "material.h"
#include "oscillator.h"
#include "grid.h"
#include "field.h"

class Material_Oscillators : public Material_Interface
{
private:
	Field P_total;
	vector<Oscillator*> oscillators;

public:
	const Grid grid;
	const u64 N_film;
	const f32 perm_bg;
    
	Material_Oscillators(Grid grid, u64 N_in, f32 perm_bg);
    ~Material_Oscillators();
    Material_Oscillators* clone();
	inline void step(Field& E, u64 film_index, u64 i);
    void init(std::unordered_map<string, Field*> &indexable_map);
    void rotate_ns();
    c32 get_index_from_wavelength(f32 wl);
    
    vector<Oscillator*> get_oscillators() { return oscillators; }
    u64 get_n_oscillators() { return oscillators.size(); }
	void add_oscillator(Oscillator* oscillator) {oscillators.push_back(oscillator);}

};

#endif