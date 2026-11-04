#ifndef PARTICLES_H
#define PARTICLES_H

#include "colors.h"

typedef struct particle {
	unsigned type;
	Color    color;
	bool     updated;
} particle;

particle Sand = {1, sandColor, false};

#endif
