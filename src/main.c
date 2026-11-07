#include <stdlib.h>
#include <string.h>
#include "particles.h"
#include "utils.h"
#include "constants.h"

void initGame();
void update();
void processInput();
void draw();
void clearParticleGrid();
void sandFall();

int      spawnRadius         = 5;
unsigned currentParticleType = 1;
Vector2  mousePosition;
unsigned *particlesGrid;
Texture2D particleTexture;
Color *pixels;

int main(void) {

	initGame();

	while(!WindowShouldClose()) {
		update();
		sandFall();
		draw();
	}

	UnloadTexture(particleTexture);
	free(pixels);
	free(particlesGrid);

	CloseWindow();
	
	return 0;
}

void initGame() {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "sandbox");
	SetTargetFPS(FPS);

	spawnRadius = 5;
	currentParticleType = 1;

	particlesGrid = calloc(GRID_WIDTH * GRID_HEIGHT, sizeof(unsigned));

	pixels = malloc(GRID_WIDTH * GRID_HEIGHT * sizeof(Color));

	Image img = GenImageColor(GRID_WIDTH, GRID_HEIGHT, bg);
	particleTexture = LoadTextureFromImage(img);
	UnloadImage(img);

	SetTextureFilter(particleTexture, TEXTURE_FILTER_POINT);
}

void update() {
	processInput();
	
	// 1 2 3
	// 4 5 6
	// 7 8 9

	for(int y = GRID_HEIGHT - 1; y >= 0 ; y--) {
		for(int x = 0; x < GRID_WIDTH; x++) {
			if(particlesGrid[GetGridIndex(x, y)] <= 0) continue;

			int p8 = GetNeighbourParticleType(x, y, 8, particlesGrid);
			if(p8 <= 0) {
				particlesGrid[GetGridIndex(x, y + 1)] = particlesGrid[GetGridIndex(x, y)];
				particlesGrid[GetGridIndex(x, y)] = 0;
				continue;
			}

			int p7 = GetNeighbourParticleType(x, y, 7, particlesGrid);
			if(p7 <= 0) {
				particlesGrid[GetGridIndex(x - 1, y + 1)] = particlesGrid[GetGridIndex(x, y)];
				particlesGrid[GetGridIndex(x, y)] = 0;
				continue;
			}

			int p9 = GetNeighbourParticleType(x, y, 9, particlesGrid);
			int p6 = GetNeighbourParticleType(x, y, 6, particlesGrid);
			if(p9 <= 0 && p6 <= 0) {
				particlesGrid[GetGridIndex(x + 1, y + 1)] = particlesGrid[GetGridIndex(x, y)];
				particlesGrid[GetGridIndex(x, y)] = 0;
				continue;
			}
		}
	}
}

void processInput() {
	mousePosition = GetMousePosition();
	mousePosition.x /= GRID_SIZE;
	mousePosition.y /= GRID_SIZE;

	if(IsKeyDown(KEY_LEFT_BRACKET)  || IsKeyDown(KEY_MINUS) || IsKeyDown(KEY_KP_SUBTRACT)) 
		spawnRadius = (spawnRadius <=   1) ?   1 : spawnRadius - 1;
	if(IsKeyDown(KEY_RIGHT_BRACKET) || IsKeyDown(KEY_EQUAL) || IsKeyDown(KEY_KP_ADD))
		spawnRadius = (spawnRadius >= 10) ? 10 : spawnRadius + 1;

	if(IsMouseButtonDown(0)) {
		int cx = (int)mousePosition.x;
		int cy = (int)mousePosition.y;
		int r2 = spawnRadius * spawnRadius;

		for(int x = cx - spawnRadius; x <= cx + spawnRadius; x++) {
			for(int y = cy - spawnRadius; y <= cy + spawnRadius; y++) {
			
				if(x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT)
					continue;
				
				int dx = x - cx;
				int dy = y - cy;
				
				if(dx * dx + dy * dy <= r2) {
					particlesGrid[y * GRID_WIDTH + x] = currentParticleType;
				}
			}
		}
	}

	if(IsKeyPressed(KEY_C)) clearParticleGrid();
}

void draw() {
	for(size_t i=0; i<GRID_WIDTH * GRID_HEIGHT; i++) {
		pixels[i] = ParticleTypes[particlesGrid[i]].color;
	}

	UpdateTexture(particleTexture, pixels);

	BeginDrawing();

	ClearBackground(bg);

	DrawTextureEx(particleTexture, (Vector2){0, 0}, 0, GRID_SIZE, WHITE);

	DrawCircleLines(mousePosition.x * GRID_SIZE, mousePosition.y * GRID_SIZE, spawnRadius * GRID_SIZE, spawnCircleColor);

	EndDrawing();
}


void clearParticleGrid() {
	for(size_t i=0; i<GRID_WIDTH * GRID_HEIGHT; i++) {
		particlesGrid[i] = 0;
	}
}

void sandFall() {
	static int timer = 0;
	timer ++;

	if(timer > 3) {
		timer = 0;
		particlesGrid[GetGridIndex(40, 0)] = 1;
	}
}
