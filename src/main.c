#include "particles.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

void initGame();
void update();
void draw();

int      spawnRadius       = 25;
unsigned currentParticleID = 1;
Vector2  mousePosition;

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
	currentParticleID = 1;
}

void update() {
	mousePosition = GetMousePosition();

	if(IsKeyDown(KEY_LEFT_BRACKET) || IsKeyDown(KEY_MINUS) || IsKeyDown(KEY_KP_SUBTRACT) || IsKeyDown(KEY_DOWN))  spawnRadius = (spawnRadius <=   5) ?   5 : spawnRadius - 1;
	if(IsKeyDown(KEY_RIGHT_BRACKET) || IsKeyDown(KEY_EQUAL) || IsKeyDown(KEY_KP_ADD) || IsKeyDown(KEY_UP)) spawnRadius = (spawnRadius >= 100) ? 100 : spawnRadius + 1;
}

void draw() {
	BeginDrawing();

	ClearBackground(bg);

	DrawCircleLinesV(mousePosition, spawnRadius, spawnCircleColor);

	EndDrawing();
}
