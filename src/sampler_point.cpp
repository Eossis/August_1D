#include "sampler_point.h"

Sampler_Point::Sampler_Point(string target_name, Grid grid, f32 dt_in, u64 x_position) :
	Sampler_Interface(Species::SAMPLER_POINT),
	target_name(target_name),
	grid(grid),
	stride((u64)floor(dt_in / grid.dt)),
	dt(grid.dt* (f32)stride),
	Nt(ceil(grid.Nt / floor(dt / grid.dt))),
	x_position(x_position),
	start(0)
{
}

void Sampler_Point::create_datastructure(H5::H5File& file, H5::Group& group_in)
{
    string name = (string("data_") + target_name).c_str();
	datagroup = group_in.createGroup(name.c_str());

    H5::DataSpace dataspace_t(1, &Nt);
	t_dataset = datagroup.createDataSet("t", H5::PredType::NATIVE_FLOAT, dataspace_t);

	H5::DataSpace dataspace_core(1, &Nt);
	core_dataset = datagroup.createDataSet(target_name.c_str(), H5::PredType::NATIVE_FLOAT, dataspace_core);
}

void Sampler_Point::init(std::unordered_map<string, Field*> &indexable_map)
{
	core = (f32*)calloc(Nt, sizeof(f32));
	t.resize(Nt);
	linspace(&t, Nt, dt);

	if (indexable_map.contains(target_name) == false)
	{
		std::cerr << "Error: Sampler_Point target field '" << target_name << "' not found in indexable_map.\n";
		throw;
	}
	sampler_target = indexable_map[target_name];
}

void Sampler_Point::deallocate()
{
    free(core);
}

Sampler_Point::~Sampler_Point()
{
	if (writing.exchange(true))
	{
		
	}
	free(core);
	writing = false;
}

void Sampler_Point::check_and_write_field(u64 nt)
{
	if (nt % stride == 0)
	{
		write_field(nt / stride);
	}
}

void Sampler_Point::write_field(u64 nt_target)
{
	operator()(nt_target) = (*sampler_target)(x_position);
}

inline f32& Sampler_Point::operator()(u64 nt)
{
	return core[nt];
}

void Sampler_Point::save_to_file()
{
	while (writing.exchange(true))
	{

	}
	core_dataset.write(core, H5::PredType::NATIVE_FLOAT);
	t_dataset.write(t.data(), H5::PredType::NATIVE_FLOAT);

	writing = false;
}