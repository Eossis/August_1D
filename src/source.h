#ifndef SOURCE_H
#define SOURCE_H

#include "common.h"
#include "grid.h"
#include "indexable.h"
#include "source_interface.h"

class Source : public Source_Interface
{
private:
	f32 t0;
public:
	const string name;
	const f32 omega;
	const f32 duration;
	const f32 wavelength;
	const f32 field_peak;

	Source(string name, f32 omega_in, f32 duration_in, f32 peak_field_in, f32 t0_in);
	~Source();

	void set_t0(f32 new_t0) { t0 += new_t0; }
	string get_name() { return name; }
	inline f32 operator()(f32 t);
};

#endif