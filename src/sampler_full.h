#ifndef SAMPLER_FULL_H
#define SAMPLER_FULL_H

#include "common.h"
#include "grid.h"
#include "field.h"
#include "sampler_interface.h"
#include "H5Cpp.h"

class Sampler_Full : Sampler_Interface
{
private:
	f32* core;
	Field* sampler_target;
	H5::Group datagroup;
	H5::DataSet t_dataset;
	H5::DataSet x_dataset;
	H5::DataSet core_dataset;

	vector<f32> t;
	vector<f32> x;

	std::string postfix_str = "";

public:
	const Grid grid;
	const string target_name;
	const u64 stride;
	const f32 dt;
	const u64 Nt;
	std::string filename_prefix;
	const u64 start;

	f32* get_core() { return core; }
	f32* get_core(u64 nt) { return &(core[nt]); }
	vector<f32>* get_t() { return &t; }
	f32 get_dt() { return dt; }
	void set_postfix(u64 i) { postfix_str = std::to_string(i); }

	Sampler_Full(string target_name, Grid grid, f32 dt);
	~Sampler_Full();

	inline f32& operator()(u64 nt, u64 nx);

	void check_and_write_field(u64 nt);
	void write_field(u64 nt_target);
	void save_to_file();
	void init(std::unordered_map<string, Field*> &indexable_map);
	void create_datastructure(H5::H5File& file, H5::Group& group_in);
    void deallocate();
};

#endif