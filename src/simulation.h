#ifndef SIMULATION_H
#define SIMULATION_H

#include "sampler_point.h"
#include "sampler_interface.h"
#include "source.h"
#include "grid.h"
#include "material_single_index.h"
#include "material_oscillators.h"
#include "field.h"

#include "lorentzian.h"
#include "drude.h"
#include "saturable_lorentzian.h"
#include "hot_electron_drude.h"
#include "two_level_hot_electron.h"
#include "film.h"

class Simulation
{
private:
	bool initialized = false;
	std::unordered_map<string, Field*> indexable_map;
	
	vector<Sampler_Interface*> samplers;
	vector<Source_Interface*> sources;
	Film* film = NULL;

    H5::Group group;
    
public:
	const Grid grid;
	Field E;

	Film* get_film() { return film; }
	Field* get_E() { return &E; }

	void set_film(Film* film_in) { film = film_in; }
	void add_source(Source_Interface* source) { sources.push_back(source); }
	void add_sampler(Sampler_Interface* sampler) { samplers.push_back(sampler); }
    void build_datastructure(H5::H5File& file, u64 i);
    void deallocate();
	Source_Interface* get_source_with_name(string name);

	Simulation(Grid	grid);
	~Simulation();
	Simulation(const Simulation& original);
	void run();
	void save_to_file();
	void init();
	void rotate_ns();
	void cleanup();
};

#endif