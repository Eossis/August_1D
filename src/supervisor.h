#ifndef SUPERVISOR_H
#define SUPERVISOR_H

#include "simulation.h"

class Supervisor
{
private:
	vector<Simulation*> simulations;
	bool verbose;

	H5::H5File file;

public:
	Supervisor(const char* settings_filename, bool verbose = true);
	~Supervisor();
	void run();
};

#endif