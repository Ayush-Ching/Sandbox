#include <raylib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

const Color bg               = { 10,  20,  30, 255};
const Color spawnCircleColor = {210, 220, 230, 255};

void initGame();
void update();
void draw();

int spawnRadius = 25;
Vector2 mousePosition;

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
