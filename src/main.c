#include <stdio.h>
#include <direct.h>
#include <stdlib.h>
#include <string.h>
#include "particles.h"
#include "utils.h"
#include "constants.h"

void initGame();
void updateParticles();
void processInput();
void draw();
void clearParticleGrid();
void drawSpawnCircle();
void sandFall();
void takeScreenshots();

const int  srMin               = 10 / GRID_SIZE;
const int  srMax               = 250 / GRID_SIZE;
int        spawnRadius         = 100 / GRID_SIZE;
size_t     frameCount          = 0;
unsigned   currentParticleType = 1;
Vector2    mousePosition;
unsigned   *particlesGrid;
unsigned   *velocityGrid;
Texture2D  particleTexture;
Color      *pixels;

int main(void) {
	printf("Working directory: %s\n", GetWorkingDirectory());

	int result = _mkdir("frames");

	printf("mkdir result: %d\n", result);
	printf("Frames directory exists: %d\n", DirectoryExists("frames"));


	initGame();

	while(!WindowShouldClose()) {
		processInput();
		updateParticles();
		sandFall();
		draw();
		
		// takeScreenshots();
		frameCount++;
	}

	UnloadTexture(particleTexture);
	free(pixels);
	free(particlesGrid);
	free(velocityGrid);

	CloseWindow();
	
	return 0;
}

void initGame() {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "sandbox");
	SetTargetFPS(FPS);

	spawnRadius = 100 / GRID_SIZE;
	currentParticleType = 1;
	frameCount = 0;

	particlesGrid = calloc(GRID_WIDTH * GRID_HEIGHT, sizeof(unsigned));
	velocityGrid  = calloc(GRID_WIDTH * GRID_HEIGHT, sizeof(unsigned));

	pixels = malloc(GRID_WIDTH * GRID_HEIGHT * sizeof(Color));

	Image img = GenImageColor(GRID_WIDTH, GRID_HEIGHT, bg);
	particleTexture = LoadTextureFromImage(img);
	UnloadImage(img);

	SetTextureFilter(particleTexture, TEXTURE_FILTER_POINT);
}

void updateParticles() {
	// 1 2 3
	// 4 5 6
	// 7 8 9

	for(int y = GRID_HEIGHT - 1; y >= 0 ; y--) {
		for(int x = 0; x < GRID_WIDTH; x++) {
			if(particlesGrid[GetGridIndex(x, y)] <= 0) continue;


			velocityGrid[GetGridIndex(x, y)]++;

			int repeat = (int)velocityGrid[GetGridIndex(x, y)];
			while(repeat--) {
				int p8 = GetNeighbourParticleType(x, y, 8, particlesGrid);
				if(p8 <= 0) {
					particlesGrid[GetGridIndex(x, y + 1)] = particlesGrid[GetGridIndex(x, y)];
					velocityGrid[GetGridIndex(x, y + 1)] = velocityGrid[GetGridIndex(x, y)];
					particlesGrid[GetGridIndex(x, y)] = 0;
					velocityGrid[GetGridIndex(x, y)] = 0;
					y = y + 1;
					continue;
				}

				int p7 = GetNeighbourParticleType(x, y, 7, particlesGrid);
				if(p7 <= 0) {
					particlesGrid[GetGridIndex(x - 1, y + 1)] = particlesGrid[GetGridIndex(x, y)];
					velocityGrid[GetGridIndex(x - 1, y + 1)] = velocityGrid[GetGridIndex(x, y)];
					particlesGrid[GetGridIndex(x, y)] = 0;
					velocityGrid[GetGridIndex(x, y)] = 0;
					x = x - 1;
					y = y + 1;
					continue;
				}

				int p9 = GetNeighbourParticleType(x, y, 9, particlesGrid);
				int p6 = GetNeighbourParticleType(x, y, 6, particlesGrid);
				if(p9 <= 0 && p6 <= 0) {
					particlesGrid[GetGridIndex(x + 1, y + 1)] = particlesGrid[GetGridIndex(x, y)];
					velocityGrid[GetGridIndex(x + 1, y + 1)] = velocityGrid[GetGridIndex(x, y)];
					particlesGrid[GetGridIndex(x, y)] = 0;
					velocityGrid[GetGridIndex(x, y)] = 0;
					x = x + 1;
					y = y + 1;
					continue;
				}

				velocityGrid[GetGridIndex(x, y)] = 0;

			}
		}
	}
}

void processInput() {
	mousePosition = GetMousePosition();
	mousePosition.x /= GRID_SIZE;
	mousePosition.y /= GRID_SIZE;

	if(IsKeyDown(KEY_LEFT_BRACKET)  || IsKeyDown(KEY_MINUS) || IsKeyDown(KEY_KP_SUBTRACT)) 
		spawnRadius = (spawnRadius <= srMin) ? srMin : spawnRadius - 1;
	if(IsKeyDown(KEY_RIGHT_BRACKET) || IsKeyDown(KEY_EQUAL) || IsKeyDown(KEY_KP_ADD))
		spawnRadius = (spawnRadius >= srMax) ? srMax : spawnRadius + 1;

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
				
				if(dx * dx + dy * dy <= r2 && particlesGrid[y * GRID_WIDTH + x] <= 0)
					particlesGrid[y * GRID_WIDTH + x] = currentParticleType;
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

	drawSpawnCircle();

	EndDrawing();
}

void drawSpawnCircle() {
	int cx = (int)mousePosition.x;
	int cy = (int)mousePosition.y;
	int r2 = spawnRadius * spawnRadius;

	for(int x = cx - spawnRadius; x <= cx + spawnRadius; x++) {
		for(int y = cy - spawnRadius; y <= cy + spawnRadius; y++) {
		
			if(x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT)
				continue;
			
			int dx = x - cx > 0 ? x - cx : cx - x;
			int dy = y - cy > 0 ? y - cy : cy - y;
			
			if(dx * dx + dy * dy <= r2 && (((dx+1)*(dx+1) + dy*dy > r2) || ((dy+1)*(dy+1) + dx*dx > r2)))
				DrawRectangle(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE, WHITE);
		}
	}
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
		particlesGrid[GetGridIndex(GRID_WIDTH / 2, 0)] = 1;
	}
}

void takeScreenshots() {
	Image screenshot = LoadImageFromScreen();
	Rectangle crop = {0, 225, 1600, 900};
	ImageCrop(&screenshot, crop);
	ExportImage(screenshot, TextFormat("frames/frame_%04zu.png", frameCount));
	UnloadImage(screenshot);
}
