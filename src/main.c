#include <stdio.h>
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

int main(void) {

	initGame();

	while(!WindowShouldClose()) {
		update();
		draw();
	}

	CloseWindow();
	
	return 0;
}

void initGame() {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "sandbox");
	SetTargetFPS(60);

	spawnRadius = 25;
	currentParticleType = 1;
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
	BeginDrawing();

	ClearBackground(bg);

	DrawCircleLinesV(mousePosition, spawnRadius, spawnCircleColor);

	for(int i=0; i<SCREEN_WIDTH; i++) {
		for(int j=0; j<SCREEN_HEIGHT; j++) {
			DrawRectangle(i, j, 1, 1, ParticleTypes[particleGrid[i][j]].color);
		}
	}

	EndDrawing();
}
