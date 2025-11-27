#ifndef FIELD_H
#define FIELD_H

#include "common.h"
#include "grid.h"
#include "source.h"

class Field
{
private:
	f32* core = NULL;
	vector<size_t> ns;
	bool initialized;

public:
	const Grid grid;
	const u64 Nx;
	const u64 time_steps;
	const string name;

	void step();
	void step_constrained(u64 edge_1, u64 edge_2);
	void boundaries();
	void rotate_ns();
	void init(std::unordered_map<string, Field*> &indexable_map);
	void clear();
	void write_source(Source_Interface& source, u64 nt, u64 nx);
    void deallocate();

	Field(Grid grid, string name);
	Field(Grid grid, u64 size, string name);
	~Field();

	inline f32& operator()(u64 i, u64 nt) { return core[ns[nt] + i * time_steps]; }
	inline f32& operator()(u64 i) { return core[ns[2] + i * time_steps]; }
};

#endif