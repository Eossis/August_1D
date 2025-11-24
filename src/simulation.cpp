#include "simulation.h"

Simulation::Simulation(const Simulation& original) :
	grid(original.grid),
	E(original.grid, "E")
{
	if (initialized)
	{
		throw;
	}

	for (auto source : original.sources)
	{
		if (source->species == Source_Interface::Species::SIMPLE_SOURCE)
		{
			Source* new_source = new Source(*((Source*)source));
			sources.push_back((Source_Interface*)new_source);
		}
	}
	for (auto sampler : original.samplers)
	{
		if (sampler->species == Sampler_Interface::Species::SAMPLER_POINT)
		{
			Sampler_Point* new_sampler = new Sampler_Point(*((Sampler_Point*)sampler));
			samplers.push_back((Sampler_Interface*)new_sampler);
		}
	}

	film = new Film(*original.film);
}

Source_Interface* Simulation::get_source_with_name(string name)
{
	for (u32 i = 0; i < sources.size(); i++)
	{
		if (sources[i]->get_name() == name)
		{
			return sources[i];
		}
	}
	throw;
}

void Simulation::init()
{
	if (initialized)
	{
		throw;
	}
	else
	{
		initialized = true;
	}
	E.init(indexable_map);
	film->init(indexable_map);

	for (u64 i = 0; i < samplers.size(); i++)
	{
		samplers[i]->init(indexable_map);
	}
}

void Simulation::deallocate()
{
    E.deallocate();
    delete film;
    for (u32 i = 0; i < samplers.size(); ++i)
	{
        samplers[i]->deallocate();
	}
}

void Simulation::rotate_ns()
{
	E.rotate_ns();
	film->rotate_ns();
}

Simulation::Simulation(Grid grid) :
	grid(grid),
	E(grid, "E")
{

}

void Simulation::build_datastructure(H5::H5File& file, u64 i)
{
    string groupname = std::string("/Simulation_" + std::to_string(i));
    group = file.createGroup(groupname.c_str());    

    for (u64 i = 0; i < samplers.size(); i++)
    {
        samplers[i]->create_datastructure(file, group);
    }
}

Simulation::~Simulation()
{
	for (u64 i = 0; i < sources.size(); i++)
	{
		delete sources[i];
	}
	for (u64 i = 0; i < samplers.size(); i++)
	{
		delete samplers[i];
	}
	delete film;

	if (writing.exchange(true))
	{

	}
	group.close();
	writing = false;
}

void Simulation::run()
{
	for (u64 nt = 1; nt < grid.Nt - 1; nt++)
	{
		if (film->enabled)
		{
			E.step_constrained(film->film_start, film->film_end);
			film->step(&E);
		}
		else
		{
			E.step();
		}
		

		for (u64 i = 0; i < sources.size(); i++)
		{
			E.write_source(*sources[i], nt, 3);
		}

		E.boundaries();
		for (u32 i = 0; i < samplers.size(); ++i)
		{
			samplers[i]->check_and_write_field(nt);
		}

		rotate_ns();
	}
}

void Simulation::cleanup()
{
    for (u32 i = 0; i < samplers.size(); ++i)
	{
        delete samplers[i];
	}
    delete film;
}

void Simulation::save_to_file()
{
	for (u32 i = 0; i < samplers.size(); ++i)
	{
		samplers[i]->save_to_file();
	}
}
