#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"
#include "field.h"

class Material_Interface
{
public:
	enum class Type
	{
		SINGLE_INDEX,
		OSCILLATOR
	} const type;

	Material_Interface(Type type) : type(type) {}
	virtual ~Material_Interface() {}
	virtual c32 get_index_from_wavelength(f32 wl) = 0;
	inline virtual void step(Field& field, u64 film_index, u64 index) = 0;
	virtual void init(std::unordered_map<string, Field*> &indexable_map) = 0;
	virtual void rotate_ns() = 0;
	virtual Material_Interface* clone() = 0;
};

#endif