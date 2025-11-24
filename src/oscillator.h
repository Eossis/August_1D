#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include "common.h"

class Oscillator
{
public:
	enum class Type
	{
		LORENTZIAN,
		DRUDE,
		SATURABLE_LORENTZIAN,
		HOT_ELECTRON_DRUDE,
		TWO_LEVEL_HOT_ELECTRON
	} const type;


	Oscillator(Type type) : type(type) {}
	virtual ~Oscillator() {}

	virtual Field* get_P() = 0;
	virtual c32 get_permittivity_from_wavelength(f32 wl) = 0;
	inline virtual void step(Field& E, u64 film_index, u64 i) = 0;
	inline virtual void post_field_update(Field& E, u64 film_index, u64 i) = 0;
	inline virtual f32 get_coupling_constant() = 0;
	virtual void init(std::unordered_map<string, Field*> &indexable_map) = 0;
	virtual void rotate_ns() = 0;
	virtual Oscillator* clone() = 0;
};

#endif
