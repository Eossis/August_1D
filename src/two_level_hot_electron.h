#ifndef TWO_LEVEL_HOT_ELECTRON_H
#define TWO_LEVEL_HOT_ELECTRON_H

#include "common.h"
#include "oscillator.h"
#include "grid.h"
#include "field.h"

class Two_Level_Hot_Electron : public Oscillator
{
private:
	Field P_l;
	Field P_d;
	Field P;
	Field Abs;

public:
	const string name;
	const Grid grid;

	const f32 dt;
	const f32 dt2;

	const f32 N_d_0;
	const f32 m_e_d_0;
	const f32 gamma_d;
	const f32 omega_P_d;
	const f32 gamma_wl_d;
	const f32 gamma_wl_l;
	const f32 omega_0_l_in;

	const f32 delta_hot_electrons;
	const f32 relaxation_hot_electrons;

	const f32 N_l_0;
	const f32 m_e_l_0;
	const f32 omega_0_l;
	const f32 gamma_l;
	const f32 omega_P_l;

	const f32 saturation_field;
	const f32 elevation_coupling;
	const f32 elevation_relaxation;

private:
	f32 a0_d;
	f32 b0_d;

	f32 f2_d;
	f32 f1_d;
	f32 f0_d;
	f32 d2_d;
	f32 d1_d;
	f32 d0_d;

	f32 f2_l;
	f32 f1_l;
	f32 f0_l;
	f32 d2_l;
	f32 d1_l;
	f32 d0_l;

	f32 a0_l;
	f32 b0_l;
	f32 b1_l;

	const u64 N;

public:
	Field m_e_d;
	Field N_d;
	Field N_l;

	Field* get_P() { return &P; }

	Two_Level_Hot_Electron(Grid grid, u64 N,
		f32 gamma_wl_d, f32 N_e_d, f32 m_e_d_0, f32 delta_hot_electrons, f32 relaxation_hot_electrons,
		f32 gamma_wl_l, f32 N_e_l, f32 m_e_l_0, f32 omega_0_l_in,
		f32 elevation_coupling, f32 elevation_relaxation, f32 saturation_field, string name_in) :

		Oscillator(Type::TWO_LEVEL_HOT_ELECTRON),
		name(name_in),
		grid(grid),
		N(N),
		P_l(Field(grid, N, "P_l_two_level_hot_electron")),
		P_d(Field(grid, N, "P_d_two_level_hot_electron")),
		P(Field(grid, N, "P_two_level_hot_electron")),
		m_e_d(Field(grid, N, "m_e_d_two_level_hot_electron")),
		N_d(Field(grid, N, "N_d_two_level_hot_electron")),
		N_l(Field(grid, N, "N_l_two_level_hot_electron")),
		Abs(Field(grid, N, "Abs")),
		dt(grid.dt),
		dt2(grid.dt2),

		//Drude Setup
		gamma_d(2.0f * pi * c / gamma_wl_d),
		N_d_0(N_e_d),
		m_e_d_0(m_e_d_0),
		omega_P_d(sqrt(N_d_0* pow(C_electron, 2.0f) / (eps0 * m_e_d_0))),

		a0_d(omega_P_d* omega_P_d),
		b0_d(gamma_d),

		//Lorentz Setup
		gamma_l(2.0f * pi * c / gamma_wl_l),
		N_l_0(N_e_l),
		m_e_l_0(m_e_l_0),
		omega_0_l(2.0f * pi * c / omega_0_l_in),
		omega_P_l(sqrt(N_l_0* pow(C_electron, 2.0f) / (eps0 * m_e_l_0))),

		a0_l(omega_P_l* omega_P_l),
		b0_l(gamma_l),
		b1_l(omega_0_l * omega_0_l),

		delta_hot_electrons(delta_hot_electrons),
		relaxation_hot_electrons(relaxation_hot_electrons),
		saturation_field(saturation_field),
		elevation_coupling(elevation_coupling),
		elevation_relaxation(elevation_relaxation),

		gamma_wl_d(gamma_wl_d),
		gamma_wl_l(gamma_wl_l),
		omega_0_l_in(omega_0_l_in)
	{

	}

	~Two_Level_Hot_Electron()
	{

	}

	Two_Level_Hot_Electron* clone()
	{
		return new Two_Level_Hot_Electron(grid, N, 
			gamma_wl_d, N_d_0, m_e_d_0, delta_hot_electrons, relaxation_hot_electrons,
			gamma_wl_l, N_l_0, m_e_l_0, omega_0_l_in, elevation_coupling, elevation_relaxation, saturation_field, name);
	}

	c32 get_permittivity_from_wavelength(f32 wl)
	{
        UNUSED(wl);
		return 0;
	}

	inline void step(Field& E, u64 film_index, u64 i)
	{
		//omega_P**2 = n * e^2 / (m * eps0)

		//drude step
		a0_d = omega_P_d * omega_P_d * (N_d(film_index, 1)) / m_e_d(film_index, 1);
		b0_d = gamma_d
			+ (1.0f / m_e_d(film_index, 1)) * (m_e_d(film_index, 1) - m_e_d(film_index, 0)) / (dt)
			- (1.0f / N_d(film_index, 1)) * (N_d(film_index, 1) - N_d(film_index, 0)) / (dt);

		f2_d = ((4.0f + 2.0f * b0_d * dt));
		f1_d = (2.0f * (4.0f) / f2_d);
		f0_d = ((2.0f * b0_d * dt - 4.0f) / f2_d);

		d2_d = ((dt2 * a0_d) / f2_d);
		d1_d = ((2.0f * dt2 * a0_d) / f2_d);
		d0_d = ((dt2 * a0_d) / f2_d);

		P_d(film_index, 2) = f1_d * P_d(film_index, 1) + f0_d * P_d(film_index, 0)
			+ d1_d * E(i, 1) + d0_d * E(i, 0);

		//lorentz step
		f32 E_sat2 = pow(saturation_field, 2.0f);
		a0_l = (omega_P_l * omega_P_l) * (N_l(film_index, 1) + N_l(film_index, 1)) *(1.0f / (1.0f + pow((abs(E(i, 1) + E(i, 0)) / 2.0f), 2.0f) / E_sat2)) / 2.0f;
		b0_l = gamma_l 
			- (2.0f / (N_l(film_index, 1) + N_l(film_index, 0))) * (N_l(film_index, 1) - N_l(film_index, 0)) / (dt);


		f2_l = ((4.0f + 2.0f * b0_l * dt + dt2 * b1_l));
		f1_l = (2.0f * (4.0f - dt2 * b1_l) / f2_l);
		f0_l = ((2.0f * b0_l * dt - dt2 * b1_l - 4.0f) / f2_l);
		d2_l = ((dt2 * a0_l) / f2_l);
		d1_l = ((2.0f * dt2 * a0_l) / f2_l);
		d0_l = ((dt2 * a0_l) / f2_l);

		P_l(film_index, 2) = f1_l * P_l(film_index, 1) + f0_l * P_l(film_index, 0)
			+ d1_l * E(i, 1) + d0_l * E(i, 0);

		//combining
		P(film_index, 0) = P_d(film_index, 0) + P_l(film_index, 0);
		P(film_index, 1) = P_d(film_index, 1) + P_l(film_index, 1);
		P(film_index, 2) = P_d(film_index, 2) + P_l(film_index, 2);
	}

	inline void post_field_update(Field& E, u64 film_index, u64 i)
	{
		P_d(film_index, 2) += d2_d * E(i, 2);
		P_l(film_index, 2) += d2_l * E(i, 2);

		//electron mass update
		m_e_d(film_index, 2) = ((1.0f - dt * relaxation_hot_electrons / 2.0f) * m_e_d(film_index, 1)
			+ delta_hot_electrons * ((P_d(film_index, 2) - P_d(film_index, 0)) * (E(i, 1)))
			+ dt * relaxation_hot_electrons) / (1.0f + dt * relaxation_hot_electrons / 2.0f);


		//electron n update
		f32 temp = ((P_l(film_index, 2) - P_l(film_index, 0)) * (E(i, 1)) / 2.0f);
		N_d(film_index, 2) = ((1.0f - dt * elevation_relaxation / 2.0f) * N_d(film_index, 1)
			+ elevation_coupling * temp
			+ dt * elevation_relaxation) / (1.0f + dt * elevation_relaxation / 2.0f);

		N_l(film_index, 2) = (N_l_0 + N_d_0 - N_d(film_index, 2) * N_d_0) / N_l_0;
	}

	inline f32 get_coupling_constant()
	{
		return d2_d + d2_l;
	}

	void init(std::unordered_map<string, Field*> &indexable_map)
	{
		P_d.init(indexable_map);
		P_l.init(indexable_map);
		m_e_d.init(indexable_map);
		P.init(indexable_map);
		N_d.init(indexable_map);
		N_l.init(indexable_map);
		Abs.init(indexable_map);
		for (u64 i = 0; i < N; i++)
		{
			m_e_d(i, 0) = 1;
			m_e_d(i, 1) = 1;
			m_e_d(i, 2) = 1;

			N_d(i, 0) = 1;
			N_d(i, 1) = 1;
			N_d(i, 2) = 1;

			N_l(i, 0) = 1;
			N_l(i, 1) = 1;
			N_l(i, 2) = 1;
		}
	}

	void rotate_ns()
	{
		P_d.rotate_ns();
		P_l.rotate_ns();
		m_e_d.rotate_ns();
		P.rotate_ns();
		N_d.rotate_ns();
		N_l.rotate_ns();
		Abs.rotate_ns();
	}
};

#endif