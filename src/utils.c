#include <stdio.h>
#include "utils.h"
#include "constants.h"

unsigned GetNeighbourParticleType(int x, int y, int pos, unsigned* particlesGrid) {

	if(x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) {
		fprintf(stderr, "GetNeighboutParticleType: the self particle is out of bounds of the grid\n");
	}

	// 1 2 3
	// 4 5 6
	// 7 8 9
	
	unsigned indestructibleType = 1;

	switch(pos) {
		case 1:
			if(x - 1 < 0 || y - 1 < 0) return indestructibleType;
			else return particlesGrid[GetGridIndex(x - 1, y - 1)];
			break;
		case 2:
			if(y - 1 < 0) return indestructibleType;
			else return particlesGrid[GetGridIndex(x, y - 1)];
			break;
		case 3:
			if(x + 1 >= GRID_WIDTH || y - 1 < 0) return indestructibleType;
			else return particlesGrid[GetGridIndex(x + 1, y - 1)];
			break;
		case 4:
			if(x - 1 < 0) return indestructibleType;
			else return particlesGrid[GetGridIndex(x - 1, y)];
			break;
		case 5:
			return particlesGrid[GetGridIndex(x, y)];
			break;
		case 6:
			if(x + 1 >= GRID_WIDTH) return indestructibleType;
			else return particlesGrid[GetGridIndex(x + 1, y)];
			break;
		case 7:
			if(x - 1 < 0 || y + 1 >= GRID_HEIGHT) return indestructibleType;
			else return particlesGrid[GetGridIndex(x - 1, y + 1)];
			break;
		case 8:
			if(y + 1 >= GRID_HEIGHT) return indestructibleType;
			else return particlesGrid[GetGridIndex(x, y + 1)];
			break;
		case 9:
			if(x + 1 >= GRID_WIDTH || y + 1 >= GRID_HEIGHT) return indestructibleType;
			else return particlesGrid[GetGridIndex(x + 1, y + 1)];
			break;
		default:
			fprintf(stderr, "GetNeighbourParticleType: pos must be from 1 to 9\n");
			return 0;
			break;
	}
}

int GetGridIndex(int x, int y) {
	return y * GRID_WIDTH + x;
}
