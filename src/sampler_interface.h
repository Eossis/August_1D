#ifndef SAMPLER_INTERFACE_H
#define SAMPLER_INTERFACE_H

#include "common.h"

class Sampler_Interface
{
public:
	enum class Species
	{
		SAMPLER_POINT,
	} const species;

	Sampler_Interface(Species species_in) : species(species_in) {}
	virtual ~Sampler_Interface() {}

	virtual void check_and_write_field(u64 nt) = 0;
	virtual void save_to_file() = 0;
	virtual void set_postfix(u64 i) = 0;
	virtual void init(std::unordered_map<string, Field*> &indexable_map) = 0;
	virtual void create_datastructure(H5::H5File& file, H5::Group& group_in) = 0;
	virtual f32* get_core() = 0;
    virtual void deallocate() = 0;

	virtual f32 get_dt() = 0;
};

#endif