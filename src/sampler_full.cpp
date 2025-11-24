#include "sampler_full.h"

Sampler_Full::Sampler_Full(string target_name, Grid grid, f32 dt_in) :
	Sampler_Interface(Species::SAMPLER_POINT),
	target_name(target_name),
	grid(grid),
	stride((u64)floor(dt_in / grid.dt)),
	dt(grid.dt* (f32)stride),
	Nt(ceil(grid.Nt / floor(dt / grid.dt))),
	start(0)
{

}

void Sampler_Full::create_datastructure(H5::H5File& file, H5::Group& group_in)
{
	// datagroup = datagroup_in->createGroup((string("data_") + target_name).c_str());

	// H5::DataSpace dataspace_t(1, &Nt);
	// t_dataset = datagroup.createDataSet("t", H5::PredType::NATIVE_FLOAT, dataspace_t);

	// H5::DataSpace dataspace_x(1, &grid.Nx);
	// x_dataset = datagroup.createDataSet("x", H5::PredType::NATIVE_FLOAT, dataspace_x);

	// hsize_t dims[2] = { grid.Nx, Nt };
	// H5::DataSpace dataspace_core(2, dims);
	// core_dataset = datagroup.createDataSet(target_name.c_str(), H5::PredType::NATIVE_FLOAT, dataspace_core);
}

void Sampler_Full::deallocate()
{
    
}

void Sampler_Full::init(std::unordered_map<string, Field*> &indexable_map)
{
	core = (f32*)calloc(Nt * grid.Nx, sizeof(f32));
	t.resize(Nt);
	linspace(&t, Nt, dt);
	x.resize(grid.Nx);
	linspace(&x, grid.Nx, grid.dx);

	if (indexable_map.contains(target_name) == false)
	{
		std::cerr << "Error: Sampler_Full target field '" << target_name << "' not found in indexable_map.\n";
		throw;
	}
	sampler_target = indexable_map[target_name];
}

Sampler_Full::~Sampler_Full()
{
	if (writing.exchange(true))
	{

	}
	core_dataset.close();
	t_dataset.close();
	x_dataset.close();
	datagroup.close();
	free(core);
	writing = false;
}

void Sampler_Full::check_and_write_field(u64 nt)
{
	if (nt % stride == 0)
	{
		write_field(nt / stride);
	}
}

void Sampler_Full::write_field(u64 nt_target)
{
	for (u32 i = 0; i < grid.Nx; i++)
	{
		operator()(nt_target, i) = (*sampler_target)(i);
	}
}

inline f32& Sampler_Full::operator()(u64 nt, u64 nx)
{
	return core[nt + nx * Nt];
}

void Sampler_Full::save_to_file()
{
	while (writing.exchange(true))
	{
		std::this_thread::yield();
	}

	core_dataset.write(core, H5::PredType::NATIVE_FLOAT);
	t_dataset.write(t.data(), H5::PredType::NATIVE_FLOAT);
	x_dataset.write(x.data(), H5::PredType::NATIVE_FLOAT);
	writing = false;
}