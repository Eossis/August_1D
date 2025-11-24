#ifndef SOURCE_INTERFACE_H
#define SOURCE_INTERFACE_H

#include "common.h"

class Source_Interface
{
public:
	enum class Species
	{
		SIMPLE_SOURCE,
	} const species;

	Source_Interface(Species species_in) : species(species_in) {}
	virtual ~Source_Interface() {}
	
	virtual void set_t0(f32 t0) = 0;
	virtual string get_name() = 0;
	virtual inline f32 operator()(f32 t) = 0;
};

#endif