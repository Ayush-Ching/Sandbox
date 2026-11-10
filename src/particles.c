#include <math.h>
#include "particles.h"
#include "constants.h"
#include "utils.h"
#include "colors.h"

Particle ParticlesList[] = {
	{ transparent },
	{ sandColor },
	{ stoneColor }
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
			
			velocityGrid[GetGridIndex(x, y)] += gravity;
			if (velocityGrid[GetGridIndex(x, y)] > maxVelocity) {
				velocityGrid[GetGridIndex(x, y)] = maxVelocity;
			}

			int repeat = (int)ceil(velocityGrid[GetGridIndex(x, y)]);

			switch(particlesGrid[GetGridIndex(x, y)]) {
				case 0:
					// Guess what happens when you update nothing...
					// Nothing happens!!
					break;
				case 1:
					updateSand(x, y, repeat);
					break;
				default:
					break;
			}
		}
	}
}

void updateSand(int x, int y, int repeat) {
	while(repeat--) {
		int p8 = GetNeighbourParticleType(x, y, 8, particlesGrid);
		if(p8 <= 0) {
			swapParticles(GetGridIndex(x, y + 1), GetGridIndex(x, y));
			y = y + 1;
			continue;
		}

		int p7 = GetNeighbourParticleType(x, y, 7, particlesGrid);
		if(p7 <= 0) {
			swapParticles(GetGridIndex(x - 1, y + 1), GetGridIndex(x, y));
			x = x - 1;
			y = y + 1;
			continue;
		}

		int p9 = GetNeighbourParticleType(x, y, 9, particlesGrid);
		int p6 = GetNeighbourParticleType(x, y, 6, particlesGrid);
		if(p9 <= 0 && p6 <= 0) {
			swapParticles(GetGridIndex(x + 1, y + 1), GetGridIndex(x, y));
			x = x + 1;
			y = y + 1;
			continue;
		}

		velocityGrid[GetGridIndex(x, y)] = 0.0f;

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
