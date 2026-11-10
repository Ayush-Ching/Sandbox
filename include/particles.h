#ifndef PARTICLES_H
#define PARTICLES_H

#include <raylib.h>

typedef struct Particle {
	Color    color;
} Particle;

void updateParticles(const float gravity, const float maxVelocity);
void swapParticles(int i1, int i2);

#endif
