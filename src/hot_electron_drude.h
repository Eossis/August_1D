#ifndef HOT_ELECTRON_DRUDE_H
#define HOT_ELECTRON_DRUDE_H

#include "common.h"
#include "oscillator.h"
#include "grid.h"
#include "field.h"

class Hot_Electron_Drude : public Oscillator
{
private:
	Field P;
	const Grid grid;

	const f32 dt;
	const f32 dt2;

	const f32 scattering_rate;
	const f32 delta;
	f32 omega_P_0;
	f32 omega_P;
	const f32 gamma;
	const u64 N;
	const f32 N_e;
	const f32 gamma_wl;

	f32 a0;
	f32 b0;
	const f32 m_e_0;

	f32 f2;
	f32 f1;
	f32 f0;
	f32 d2;
	f32 d1;
	f32 d0;

public:
	Field m_e_d;

	Field* get_P() { return &P; }

	Hot_Electron_Drude(Grid grid, u64 N, f32 gamma_wl, f32 N_e, f32 m_e_0, f32 delta, f32 scattering_rate) :
		Oscillator(Type::HOT_ELECTRON_DRUDE),
		grid(grid),
		omega_P_0(sqrt(N_e * C_electron * C_electron / (eps0 * m_e_0))),
		gamma(2.0f * pi * c / gamma_wl),
		m_e_0(m_e_0),
		N(N),
		gamma_wl(gamma_wl),
		N_e(N_e),
		P(Field(grid, N, "P_hot_electron")),
		m_e_d(Field(grid, N, "m_e_d_hot_electron")),
		a0(omega_P_0 * omega_P_0),
		b0(gamma),
		dt(grid.dt),
		dt2(grid.dt2),
		f2((4.0f + 2.0f * b0 * dt)),
		f1(2.0f * (4.0f) / f2),
		f0((2.0f * b0 * dt - 4.0f) / f2),
		delta(delta),
		scattering_rate(scattering_rate)
	{
		
	}

	~Hot_Electron_Drude()
	{

	}
	
	inline void post_field_update(Field& E, u64 film_index, u64 i) {}

	Hot_Electron_Drude* clone()
	{
		return new Hot_Electron_Drude(grid, N, gamma_wl, N_e, m_e_0, delta, scattering_rate);
	}

	c32 get_permittivity_from_wavelength(f32 wl)
	{
		f32 omega = 2.0f * pi * c / wl;
		return omega_P * omega_P / (-omega * omega- Im * omega * gamma);
	}

	inline void step(Field& E, u64 film_index, u64 i)
	{
		m_e_d(film_index, 2) = ((1.0f - dt * scattering_rate / 2.0f) * m_e_d(film_index, 1) 
			+ delta * (P(film_index, 1) - P(film_index, 0)) * (E(i, 1) + E(i, 0)) / 2.0f
			+ dt * scattering_rate) / (1.0f + dt * scattering_rate / 2.0f);

		a0 = omega_P_0 * omega_P_0 / m_e_d(film_index, 2);
		b0 = gamma + (1.0f / m_e_d(film_index, 2)) * (m_e_d(film_index, 2) - m_e_d(film_index, 0)) / (2.0f * dt);

		f2 = ((4.0f + 2.0f * b0 * dt));
		f1 = (2.0f * (4.0f) / f2);
		f0 = ((2.0f * b0 * dt - 4.0f) / f2);

		d2 = ((dt2 * a0) / f2);
		d1 = ((2.0f * dt2 * a0) / f2);
		d0 = ((dt2 * a0) / f2);

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
		m_e_d.init(indexable_map);
		for (u64 i = 0; i < N; i++)
		{
			m_e_d(i, 0) = 1;
			m_e_d(i, 1) = 1;
			m_e_d(i, 2) = 1;
		}
	}

	void rotate_ns()
	{
		P.rotate_ns();
		m_e_d.rotate_ns();
	}
};

#endif