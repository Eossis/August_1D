#ifndef DRUDE_H
#define DRUDE_H

#include "common.h"
#include "oscillator.h"
#include "grid.h"
#include "field.h"

class Drude : public Oscillator
{
private:
	Field P;

public:
	const Grid grid;

	const f32 dt;
	const f32 dt2;

	const f32 omega_P_wl;
	const f32 gamma_wl;
	const f32 omega_P;
	const f32 gamma;
	const f32 omega_P2;
	const u64 N;

	const f32 a0;
	const f32 b0;
	const f32 b1;

	const f32 f2;
	const f32 f1;
	const f32 f0;
	const f32 d2;
	const f32 d1;
	const f32 d0;
	const std::string name;

	Field* get_P() { return &P; }

	Drude(Grid grid, u64 N, f32 omega_P_wl, f32 gamma_wl, std::string name) :
		Oscillator(Type::DRUDE),
		grid(grid),
		omega_P_wl(omega_P_wl),
		gamma_wl(gamma_wl),
		omega_P(2.0f * pi * c / omega_P_wl),
		gamma(2.0f * pi * c / gamma_wl),
		omega_P2(omega_P* omega_P),
		N(N),
		P(Field(grid, N, (name + "_P"))),
		a0(omega_P2),
		b0(gamma),
		b1(0.0f),
		dt(grid.dt),
		dt2(grid.dt2),
		f2((4.0f + 2.0f * b0 * dt + dt2 * b1)),
		f1(2.0f * (4.0f - dt2 * b1) / f2),
		f0((2.0f * b0 * dt - dt2 * b1 - 4.0f) / f2),
		d2((dt2* a0) / f2),
		d1((2.0f * dt2 * a0) / f2),
		d0((dt2* a0) / f2),
		name(name)
	{

	}

	Drude* clone()
	{
		return new Drude(grid, N, omega_P_wl, gamma_wl, name);
	}

	inline void post_field_update(Field& E, u64 film_index, u64 i) {}

	~Drude()
	{

	}

	c32 get_permittivity_from_wavelength(f32 wl)
	{
		f32 omega = 2.0f * pi * c / wl;
		return omega_P2 / (- omega*omega - Im * omega * gamma);
	}

	inline void step(Field& E, u64 film_index, u64 i)
	{
		P(film_index, 1) += d2 * E(i, 1);
		P(film_index, 2) = f1 * P(film_index, 1) + f0 * P(film_index, 0)
			+ d1 * E(i, 1) + d0 * E(i, 0);

	}

	inline f32 get_coupling_constant()
	{
		return d2;
	}

	void init(std::unordered_map<string, Field*> &indexable_map)
	{
		P.init(indexable_map);
	}

	void rotate_ns()
	{
		P.rotate_ns();
	}
};

#endif