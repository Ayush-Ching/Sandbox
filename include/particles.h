#ifndef PARTICLES_H
#define PARTICLES_H

#include "colors.h"

typedef struct Particle {
	Color    color;
} Particle;

const Particle ParticleTypes[] = {
	{ transparent },
	{ sandColor }
};

#endif
