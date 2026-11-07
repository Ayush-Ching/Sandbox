#include <stddef.h>
#include "constants.h"

int GetGridIndex(int x, int y);
unsigned GetNeighbourParticleType(int x, int y, int pos, unsigned* particlesGrid);
unsigned GetNeighbourParticleTypeXY(int x, int y, int dx, int dy, unsigned* particlesGrid);
