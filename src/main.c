#include <stdlib.h>
#include "particles.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

void initGame();
void update();
void processInput();
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
	processInput();
}

void processInput() {
	mousePosition = GetMousePosition();

	if(IsKeyDown(KEY_LEFT_BRACKET)  || IsKeyDown(KEY_MINUS) || IsKeyDown(KEY_KP_SUBTRACT)) 
		spawnRadius = (spawnRadius <=   5) ?   5 : spawnRadius - 1;
	if(IsKeyDown(KEY_RIGHT_BRACKET) || IsKeyDown(KEY_EQUAL) || IsKeyDown(KEY_KP_ADD))
		spawnRadius = (spawnRadius >= 100) ? 100 : spawnRadius + 1;

	// if(IsMouseButtonDown(0)) {
	// 	for(int x = mousePosition.x - spawnRadius; x <= mousePosition.x + spawnRadius; x++) {
	// 		for(int y = mousePosition.y - spawnRadius; y <= mousePosition.y + spawnRadius; y++) {
	// 			if(x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
	// 				particleGrid[y * SCREEN_WIDTH + x] = currentParticleType;
	// 		}
	// 	}
	// }

	if(IsMouseButtonDown(0)) {
		int cx = (int)mousePosition.x;
		int cy = (int)mousePosition.y;
		int r2 = spawnRadius * spawnRadius;

		for(int x = cx - spawnRadius; x <= cx + spawnRadius; x++) {
			for(int y = cy - spawnRadius; y <= cy + spawnRadius; y++) {
			
				if(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
					continue;
				
				int dx = x - cx;
				int dy = y - cy;
				
				if(dx * dx + dy * dy <= r2) {
					particleGrid[y * SCREEN_WIDTH + x] = currentParticleType;
				}
			}
		}
	}

	if(IsKeyPressed(KEY_C)) clearParticleGrid();
}

void draw() {
	for(size_t i=0; i<SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
		pixels[i] = ParticleTypes[ particleGrid[i] ].color;
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
