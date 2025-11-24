#ifndef INDEXABLE_H
#define INDEXABLE_H

#include "common.h"

class Indexable
{
public:
	Indexable() {}
	virtual ~Indexable() {}

	inline virtual f32& operator()(u64 i) = 0;
};

#endif