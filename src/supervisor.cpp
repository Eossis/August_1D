#include "supervisor.h"
#include "sampler_full.h"

void simulation_init_and_run(Simulation* simulation)
{
	simulation->init();
	simulation->run();
    simulation->save_to_file();
    simulation->deallocate();
}

Supervisor::Supervisor(const char* settings_filename, bool verbose)
	: verbose(verbose),
	file(settings_filename, H5F_ACC_RDWR)
{
	H5::Group settings_group = file.openGroup("Settings");

	f32 grid_X = readAttribute<f32>(settings_group, "X");
	f32 grid_dx = readAttribute<f32>(settings_group, "dx");
	f32 grid_T = readAttribute<f32>(settings_group, "T");
	u32 cour = readAttribute<u32>(settings_group, "cour");
	bool film_enabled = readAttribute<bool>(settings_group, "film_enabled");

	u32 n_sources = readAttribute<u32>(settings_group, "n_sources");
	u32 n_samplers = readAttribute<u32>(settings_group, "n_samplers");

	Grid grid(grid_X, grid_dx, grid_T, cour);
	Simulation* sim = new Simulation(grid);

	for (u64 i = 0; i < n_sources; i++)
	{
		std::string source_group_name = std::string("Source_") + std::to_string(i);
		H5::Group source_group = settings_group.openGroup(source_group_name.c_str());

		std::string source_name = readAttribute<std::string>(source_group, "name");
		std::string source_type = readAttribute<std::string>(source_group, "type");
		if (source_type == "gaussian")
		{
			f32 wl = readAttribute<f32>(source_group, "wl");
			f32 duration = readAttribute<f32>(source_group, "duration");
			f32 peak_field = readAttribute<f32>(source_group, "peak_field");
			f32 t0 = readAttribute<f32>(source_group, "t0");
			Source* source = new Source(source_name, 2.0f * pi * c / wl, duration, peak_field, t0);
			sim->add_source(source);
		}

		source_group.close();
	}

	for (u64 i = 0; i < n_samplers; i++)
	{
		std::string sampler_group_name = std::string("Sampler_") + std::to_string(i);
		H5::Group sampler_group = settings_group.openGroup(sampler_group_name.c_str());
		std::string sampler_target_name = readAttribute<std::string>(sampler_group, "target_name");
		std::string sampler_type = readAttribute<std::string>(sampler_group, "type");
		f32 dt = readAttribute<f32>(sampler_group, "dt");
		if (sampler_type == "point")
		{
			u32 position_index = readAttribute<u32>(sampler_group, "position_index");
			Sampler_Point* sampler = new Sampler_Point(sampler_target_name, grid, dt, position_index);
			sim->add_sampler((Sampler_Interface*)sampler);
		}
		if (sampler_type == "full")
		{
			Sampler_Full* sampler = new Sampler_Full(sampler_target_name, grid, dt);
			sim->add_sampler((Sampler_Interface*)sampler);
		}

		sampler_group.close();
	}

	if (film_enabled)
	{
		H5::Group material_group = settings_group.openGroup("Material");
		std::string material_type = readAttribute<std::string>(material_group, "type");
		if (material_type == "material_oscillator")
		{
			f32 film_width = readAttribute<f32>(settings_group, "film_width");
			Film* film = new Film(grid, film_width);
			
			u32 n_oscillators = readAttribute<u32>(material_group, "n_oscillators");
			f32 perm_bg = readAttribute<f32>(material_group, "perm_bg");

			Material_Interface* material = new Material_Oscillators(grid, film->N, perm_bg);
			for (u32 i = 0; i < n_oscillators; i++)
			{
				std::string oscillator_group_name = std::string("Oscillator_") + std::to_string(i);
				H5::Group oscillator_group = material_group.openGroup(oscillator_group_name.c_str());
				std::string oscillator_type = readAttribute<std::string>(oscillator_group, "type");
				std::string oscillator_name = readAttribute<std::string>(oscillator_group, "name");
				if (oscillator_type == "drude")
				{
					f32 omega_P = readAttribute<f32>(oscillator_group, "omega_P");
					f32 gamma = readAttribute<f32>(oscillator_group, "gamma");
					((Material_Oscillators*)material)->add_oscillator(new Drude(grid, film->N, omega_P, gamma, oscillator_name));
				}
				else if (oscillator_type == "saturable_lorentzian")
				{
					f32 omega_P = readAttribute<f32>(oscillator_group, "omega_P");
					f32 omega_0 = readAttribute<f32>(oscillator_group, "omega_0");
					f32 gamma = readAttribute<f32>(oscillator_group, "gamma");
					f32 P_sat = readAttribute<f32>(oscillator_group, "P_sat");
					((Material_Oscillators*)material)->add_oscillator(new Saturable_Lorentzian(grid, film->N, omega_P, omega_0, gamma, P_sat));
				}
				else if (oscillator_type == "lorentzian")
				{
					f32 omega_P = readAttribute<f32>(oscillator_group, "omega_P");
					f32 omega_0 = readAttribute<f32>(oscillator_group, "omega_0");
					f32 gamma = readAttribute<f32>(oscillator_group, "gamma");
					((Material_Oscillators*)material)->add_oscillator(new Lorentzian(grid, film->N, omega_P, omega_0, gamma, oscillator_name));
				}
				else if (oscillator_type == "hot_electron_drude")
				{
					f32 gamma = readAttribute<f32>(oscillator_group, "gamma");
					f32 N_e = readAttribute<f32>(oscillator_group, "N_e");
					f32 m_e_0 = readAttribute<f32>(oscillator_group, "m_e_0");
					f32 delta = readAttribute<f32>(oscillator_group, "delta");
					f32 relaxation = readAttribute<f32>(oscillator_group, "relaxation");
					Hot_Electron_Drude* hot_electron_drude = new Hot_Electron_Drude(grid, film->N, gamma, N_e, m_e_0, delta, relaxation);
					((Material_Oscillators*)material)->add_oscillator(hot_electron_drude);
				}
				else if (oscillator_type == "two_level_hot_electron")
				{
					f32 gamma_d = readAttribute<f32>(oscillator_group, "gamma_d");
					f32 N_e_d = readAttribute<f32>(oscillator_group, "N_e_d");
					f32 m_e_0_d = readAttribute<f32>(oscillator_group, "m_e_0_d");
					f32 delta_d = readAttribute<f32>(oscillator_group, "delta_d");
					f32 relaxation_d = readAttribute<f32>(oscillator_group, "relaxation_d");
					f32 gamma_l = readAttribute<f32>(oscillator_group, "gamma_l");
					f32 N_e_l = readAttribute<f32>(oscillator_group, "N_e_l");
					f32 m_e_0_l = readAttribute<f32>(oscillator_group, "m_e_0_l");
					f32 omega_0_l = readAttribute<f32>(oscillator_group, "omega_0_l");
					f32 elevation_coupling = readAttribute<f32>(oscillator_group, "elevation_coupling");
					f32 elevation_relaxation = readAttribute<f32>(oscillator_group, "elevation_relaxation");
					f32 saturation_field = readAttribute<f32>(oscillator_group, "saturation_field");
					Two_Level_Hot_Electron* two_level_hot_electron = new Two_Level_Hot_Electron(grid, film->N,
						gamma_d, N_e_d, m_e_0_d, delta_d, relaxation_d,
						gamma_l, N_e_l, m_e_0_l, omega_0_l, elevation_coupling, elevation_relaxation, saturation_field, oscillator_name);
					((Material_Oscillators*)material)->add_oscillator(two_level_hot_electron);
				}
				oscillator_group.close();
			}
			film->set_material(material);
			sim->set_film(film);
		}
		material_group.close();
	}
	else
	{
		Film* film = new Film();
		sim->set_film(film);
	}

	bool delay_scan_active = settings_group.nameExists("Delay_Scan");
	if (delay_scan_active)
	{
		std::string delay_scan_group_name = std::string("Delay_Scan");
		H5::Group delay_scan_group = settings_group.openGroup(delay_scan_group_name.c_str());
		u32 n_steps = readAttribute<u32>(delay_scan_group, "n_steps");
		f32 start = readAttribute<f32>(delay_scan_group, "start");
		f32 stop = readAttribute<f32>(delay_scan_group, "stop");
		std::string target_name = readAttribute<std::string>(delay_scan_group, "target_name");

		for (u32 i = 0; i < n_steps; i++)
		{
			simulations.push_back(new Simulation(*sim));
			simulations[i]->get_source_with_name(target_name)->set_t0(start + i * (stop - start) / ((f32)n_steps - 1));
		}

		delay_scan_group.close();
	}
	else
	{
		simulations.push_back(sim);
	}


	settings_group.close();

	for (u64 i = 0; i < simulations.size();  i++)
	{
        simulations[i]->build_datastructure(file, i);
	}

}

Supervisor::~Supervisor()
{
	file.close();
}

void Supervisor::run()
{
	vector<std::thread> threads;

	for (u64 i = 0; i < simulations.size(); i++)
	{
		threads.push_back(std::thread(simulation_init_and_run, simulations[i]));
	}
    
	for (u64 i = 0; i < simulations.size(); i++)
	{
        threads[i].join();
	}

    for (u64 i = 0; i < simulations.size(); i++)
    {
    }
}
