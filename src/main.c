#include <stdlib.h>
#include "particles.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

void initGame();
void update();
void draw();

int      spawnRadius         = 25;
unsigned currentParticleType = 1;
Vector2  mousePosition;
unsigned particleGrid[SCREEN_WIDTH][SCREEN_HEIGHT] = {0};
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

	CloseWindow();
	
	return 0;
}

void initGame() {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "sandbox");
	SetTargetFPS(1000);

	spawnRadius = 25;
	currentParticleType = 1;

	pixels = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Color));

	Image img = GenImageColor(SCREEN_WIDTH, SCREEN_HEIGHT, bg);
	particleTexture = LoadTextureFromImage(img);
	UnloadImage(img);

	SetTextureFilter(particleTexture, TEXTURE_FILTER_POINT);
}

void update() {
	mousePosition = GetMousePosition();

	if(IsKeyDown(KEY_LEFT_BRACKET) || IsKeyDown(KEY_MINUS) || IsKeyDown(KEY_KP_SUBTRACT) || IsKeyDown(KEY_DOWN))  spawnRadius = (spawnRadius <=   5) ?   5 : spawnRadius - 1;
	if(IsKeyDown(KEY_RIGHT_BRACKET) || IsKeyDown(KEY_EQUAL) || IsKeyDown(KEY_KP_ADD) || IsKeyDown(KEY_UP)) spawnRadius = (spawnRadius >= 100) ? 100 : spawnRadius + 1;

	if(IsMouseButtonDown(0)) {
		for(int i = mousePosition.x - spawnRadius; i <= mousePosition.x + spawnRadius; i++) {
			for(int j = mousePosition.y - spawnRadius; j <= mousePosition.y + spawnRadius; j++) {
				particleGrid[i][j] = currentParticleType;
			}
		}
	}
}

void draw() {
	for(int i=0; i<SCREEN_WIDTH; i++) {
		for(int j=0; j<SCREEN_HEIGHT; j++) {
			pixels[j * SCREEN_WIDTH + i] = ParticleTypes[particleGrid[i][j]].color;
		}
	}

	UpdateTexture(particleTexture, pixels);

	BeginDrawing();

	ClearBackground(bg);

	DrawTexture(particleTexture, 0, 0, WHITE);

	DrawCircleLinesV(mousePosition, spawnRadius, spawnCircleColor);

	EndDrawing();
}
