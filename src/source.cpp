#include "source.h"


Source::Source(string name, f32 omega_in, f32 duration_in, f32 field_peak_in, f32 t0_in) :
	Source_Interface(Species::SIMPLE_SOURCE),
	name(name),
	omega(omega_in),
	duration(duration_in),
	wavelength(2.0f * pi * c / omega_in),
	field_peak(field_peak_in),
	t0(t0_in)
{
	
}

Source::~Source() 
{

}

inline f32 Source::operator()(f32 t)
{
	return field_peak * cos(omega * (t - t0)) * sqrt(exp(-4.0f * log(2.0f) * pow(t - t0, 2) / pow(duration, 2)));
}