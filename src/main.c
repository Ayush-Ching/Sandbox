#include <stdlib.h>
#include "particles.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

void initGame();
void update();
void draw();
void clearParticleGrid();

int      spawnRadius         = 25;
unsigned currentParticleType = 1;
Vector2  mousePosition;
unsigned *particleGrid;
Texture2D particleTexture;
Color *pixels;

int main(void) {

	initGame();

	while(!WindowShouldClose()) {
		update();
		draw();
	}

	UnloadTexture(particleTexture);
	free(pixels);
	free(particleGrid);

	CloseWindow();
	
	return 0;
}

void initGame() {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "sandbox");
	SetTargetFPS(60);

	spawnRadius = 25;
	currentParticleType = 1;

	particleGrid = calloc(SCREEN_WIDTH * SCREEN_HEIGHT, sizeof(unsigned));

	pixels = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Color));

	Image img = GenImageColor(SCREEN_WIDTH, SCREEN_HEIGHT, bg);
	particleTexture = LoadTextureFromImage(img);
	UnloadImage(img);

	SetTextureFilter(particleTexture, TEXTURE_FILTER_POINT);
}

void update() {
	mousePosition = GetMousePosition();

	if(IsKeyDown(KEY_LEFT_BRACKET)  || IsKeyDown(KEY_MINUS) || IsKeyDown(KEY_KP_SUBTRACT))  spawnRadius = (spawnRadius <=   5) ?   5 : spawnRadius - 1;
	if(IsKeyDown(KEY_RIGHT_BRACKET) || IsKeyDown(KEY_EQUAL) || IsKeyDown(KEY_KP_ADD)     ) spawnRadius = (spawnRadius >= 100) ? 100 : spawnRadius + 1;

	if(IsMouseButtonDown(0)) {
		for(int i = mousePosition.x - spawnRadius; i <= mousePosition.x + spawnRadius; i++) {
			for(int j = mousePosition.y - spawnRadius; j <= mousePosition.y + spawnRadius; j++) {
				particleGrid[j * SCREEN_WIDTH + i] = currentParticleType;
			}
		}
	}

	if(IsKeyPressed(KEY_C)) clearParticleGrid();
}

void draw() {
	for(int i=0; i<SCREEN_WIDTH; i++) {
		for(int j=0; j<SCREEN_HEIGHT; j++) {
			pixels[j * SCREEN_WIDTH + i] = ParticleTypes[particleGrid[j * SCREEN_WIDTH + i]].color;
		}
	}

	UpdateTexture(particleTexture, pixels);

	BeginDrawing();

	ClearBackground(bg);

	DrawTexture(particleTexture, 0, 0, WHITE);

	DrawCircleLinesV(mousePosition, spawnRadius, spawnCircleColor);

	EndDrawing();
}


void clearParticleGrid() {
	for(size_t i=0; i<SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
		particleGrid[i] = 0;
	}
}
