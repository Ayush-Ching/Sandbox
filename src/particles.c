#include <math.h>
#include "particles.h"
#include "constants.h"
#include "utils.h"
#include "colors.h"

Particle ParticlesList[] = {
	{ transparent },
	{ sandColor }
};

extern unsigned *particlesGrid;
extern float    *velocityGrid;
extern Color    *pixelColors;

void updateParticles(const float gravity, const float maxVelocity) {
	// 1 2 3
	// 4 5 6
	// 7 8 9

	for(int y = GRID_HEIGHT - 1; y >= 0 ; y--) {
		for(int x = 0; x < GRID_WIDTH; x++) {
			if(particlesGrid[GetGridIndex(x, y)] <= 0) continue;
			
			int newX = x;
			int newY = y;

			velocityGrid[GetGridIndex(newX, newY)] += gravity;
			if (velocityGrid[GetGridIndex(newX, newY)] > maxVelocity) {
				velocityGrid[GetGridIndex(newX, newY)] = maxVelocity;
			}

			int repeat = (int)ceil(velocityGrid[GetGridIndex(newX, newY)]);
			while(repeat--) {
				int p8 = GetNeighbourParticleType(newX, newY, 8, particlesGrid);
				if(p8 <= 0) {
					swapParticles(GetGridIndex(newX, newY + 1), GetGridIndex(newX, newY));
					newY = newY + 1;
					continue;
				}

				int p7 = GetNeighbourParticleType(newX, newY, 7, particlesGrid);
				if(p7 <= 0) {
					swapParticles(GetGridIndex(newX - 1, newY + 1), GetGridIndex(newX, newY));
					newX = newX - 1;
					newY = newY + 1;
					continue;
				}

				int p9 = GetNeighbourParticleType(newX, newY, 9, particlesGrid);
				int p6 = GetNeighbourParticleType(newX, newY, 6, particlesGrid);
				if(p9 <= 0 && p6 <= 0) {
					swapParticles(GetGridIndex(newX + 1, newY + 1), GetGridIndex(newX, newY));
					newX = newX + 1;
					newY = newY + 1;
					continue;
				}

				velocityGrid[GetGridIndex(newX, newY)] = 0.0f;

			}
		}
	}
}


void swapParticles(int i1, int i2) {
	unsigned tempP = particlesGrid[i1];
	float    tempV = velocityGrid [i1];
	Color    tempC = pixelColors  [i1];

	particlesGrid[i1] = particlesGrid[i2];
	velocityGrid [i1] = velocityGrid [i2];
	pixelColors  [i1] = pixelColors  [i2];

	particlesGrid[i2] = tempP;
	velocityGrid [i2] = tempV;
	pixelColors  [i2] = tempC;
}
