#include <stdlib.h>
#include <string.h>
#include "particles.h"
#include "utils.h"

void initGame();
void update();
void processInput();
void draw();
void clearParticleGrid();

int      spawnRadius         = 25;
unsigned currentParticleType = 1;
Vector2  mousePosition;
unsigned *particlesGrid;
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
	free(particlesGrid);

	CloseWindow();
	
	return 0;
}

void initGame() {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "sandbox");
	SetTargetFPS(FPS);

	spawnRadius = 25;
	currentParticleType = 1;

	particlesGrid = calloc(SCREEN_WIDTH * SCREEN_HEIGHT, sizeof(unsigned));

	pixels = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Color));

	Image img = GenImageColor(SCREEN_WIDTH, SCREEN_HEIGHT, bg);
	particleTexture = LoadTextureFromImage(img);
	UnloadImage(img);

	SetTextureFilter(particleTexture, TEXTURE_FILTER_POINT);
}

void update() {
	processInput();
	
	// 1 2 3
	// 4 5 6
	// 7 8 9

	for(int y = SCREEN_HEIGHT - 1; y >= 0 ; y--) {
		for(int x = 0; x < SCREEN_WIDTH; x++) {
			if(particlesGrid[y * SCREEN_WIDTH + x] <= 0) continue;

			int p8 = GetNeighbourParticleType(x, y, 8, particlesGrid);
			if(p8 <= 0) {
				particlesGrid[(y + 1) * SCREEN_WIDTH + x] = particlesGrid[y * SCREEN_WIDTH + x];
				particlesGrid[y * SCREEN_WIDTH + x] = 0;
				continue;
			}

			int p7 = GetNeighbourParticleType(x, y, 7, particlesGrid);
			if(p7 <= 0) {
				particlesGrid[(y + 1) * SCREEN_WIDTH + x - 1] = particlesGrid[y * SCREEN_WIDTH + x];
				particlesGrid[y * SCREEN_WIDTH + x] = 0;
				continue;
			}

			int p9 = GetNeighbourParticleType(x, y, 9, particlesGrid);
			int p6 = GetNeighbourParticleType(x, y, 6, particlesGrid);
			if(p9 <= 0 && p6 <= 0) {
				particlesGrid[(y + 1) * SCREEN_WIDTH + x + 1] = particlesGrid[y * SCREEN_WIDTH + x];
				particlesGrid[y * SCREEN_WIDTH + x] = 0;
				continue;
			}
		}
	}
}

void processInput() {
	mousePosition = GetMousePosition();

	if(IsKeyDown(KEY_LEFT_BRACKET)  || IsKeyDown(KEY_MINUS) || IsKeyDown(KEY_KP_SUBTRACT)) 
		spawnRadius = (spawnRadius <=   5) ?   5 : spawnRadius - 1;
	if(IsKeyDown(KEY_RIGHT_BRACKET) || IsKeyDown(KEY_EQUAL) || IsKeyDown(KEY_KP_ADD))
		spawnRadius = (spawnRadius >= 100) ? 100 : spawnRadius + 1;

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
					particlesGrid[y * SCREEN_WIDTH + x] = currentParticleType;
				}
			}
		}
	}

	if(IsKeyPressed(KEY_C)) clearParticleGrid();
}

void draw() {
	for(size_t i=0; i<SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
		pixels[i] = ParticleTypes[ particlesGrid[i] ].color;
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
		particlesGrid[i] = 0;
	}
}
