#include <stdio.h>
#include <math.h>
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

int          spawnRadius          = 50 / GRID_SIZE;   // spawn radius
const int    srMin                = 10 / GRID_SIZE;   // min spawn radius
const int    srMax                = 250 / GRID_SIZE;  // max spawn radius
size_t       frameCount           = 0;
unsigned     currentParticleType  = 1;
const float  gravity              = 0.1f;
const float  maxVelocity          = 3.0f;
const int    gridLength           = GRID_WIDTH * GRID_HEIGHT;

Vector2      mousePosition;
unsigned     *particlesGrid;
float        *velocityGrid;
Texture2D    particleTexture;
Color        *pixels;

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

	spawnRadius = 50 / GRID_SIZE;
	currentParticleType = 1;
	frameCount = 0;

	particlesGrid = calloc(gridLength, sizeof(unsigned));
	velocityGrid  = calloc(gridLength, sizeof(float));
	pixels        = malloc(gridLength * sizeof(Color));

	if(!particlesGrid || !velocityGrid || !pixels) {
		fprintf(stderr, "Failed to allocate memory for the grids\n");
	}

	Image img = GenImageColor(GRID_WIDTH, GRID_HEIGHT, bg);
	particleTexture = LoadTextureFromImage(img);
	UnloadImage(img);

	SetTextureFilter(particleTexture, TEXTURE_FILTER_POINT);
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
		int cm = (cx + cy) % 2;    // this is just to make sure that we can spawn a particle with 1 pixel size circle.

		for(int x = cx - spawnRadius; x <= cx + spawnRadius; x++) {
			for(int y = cy - spawnRadius; y <= cy + spawnRadius; y++) {
			
				if(x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) continue;
				if((x + y) % 2 != cm) continue;
				if(particlesGrid[y * GRID_WIDTH + x] > 0) continue;
				
				int dx = x - cx;
				int dy = y - cy;
				
				if(dx * dx + dy * dy <= r2) {
					particlesGrid[y * GRID_WIDTH + x] = currentParticleType;
					velocityGrid [y * GRID_WIDTH + x] = 0;
				}
			}
		}
	}

	if(IsKeyPressed(KEY_C)) clearParticleGrid();
}

void updateParticles() {
	// 1 2 3
	// 4 5 6
	// 7 8 9

	for(int y = GRID_HEIGHT - 1; y >= 0 ; y--) {
		for(int x = 0; x < GRID_WIDTH; x++) {
			if(particlesGrid[GetGridIndex(x, y)] <= 0) continue;
			
			int newX = x;
			int newY = y;

			velocityGrid[GetGridIndex(newX, newY)] += gravity;
			if (velocityGrid[GetGridIndex(newX, newY)] > maxVelocity) {
				velocityGrid[GetGridIndex(newX, newY)] = maxVelocity;
			}

			int repeat = (int)ceil(velocityGrid[GetGridIndex(newX, newY)]);
			while(repeat--) {
				int p8 = GetNeighbourParticleType(newX, newY, 8, particlesGrid);
				if(p8 <= 0) {
					particlesGrid[GetGridIndex(newX, newY + 1)] = particlesGrid[GetGridIndex(newX, newY)];
					velocityGrid[GetGridIndex(newX, newY + 1)] = velocityGrid[GetGridIndex(newX, newY)];
					particlesGrid[GetGridIndex(newX, newY)] = 0;
					velocityGrid[GetGridIndex(newX, newY)] = 0.0f;
					newY = newY + 1;
					continue;
				}

				int p7 = GetNeighbourParticleType(newX, newY, 7, particlesGrid);
				if(p7 <= 0) {
					particlesGrid[GetGridIndex(newX - 1, newY + 1)] = particlesGrid[GetGridIndex(newX, newY)];
					velocityGrid[GetGridIndex(newX - 1, newY + 1)] = velocityGrid[GetGridIndex(newX, newY)];
					particlesGrid[GetGridIndex(newX, newY)] = 0;
					velocityGrid[GetGridIndex(newX, newY)] = 0.0f;
					newX = newX - 1;
					newY = newY + 1;
					continue;
				}

				int p9 = GetNeighbourParticleType(newX, newY, 9, particlesGrid);
				int p6 = GetNeighbourParticleType(newX, newY, 6, particlesGrid);
				if(p9 <= 0 && p6 <= 0) {
					particlesGrid[GetGridIndex(newX + 1, newY + 1)] = particlesGrid[GetGridIndex(newX, newY)];
					velocityGrid[GetGridIndex(newX + 1, newY + 1)] = velocityGrid[GetGridIndex(newX, newY)];
					particlesGrid[GetGridIndex(newX, newY)] = 0;
					velocityGrid[GetGridIndex(newX, newY)] = 0.0f;
					newX = newX + 1;
					newY = newY + 1;
					continue;
				}

				velocityGrid[GetGridIndex(newX, newY)] = 0.0f;

			}
		}
	}
}

void draw() {
	for(size_t i=0; i<gridLength; i++) {
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
		velocityGrid[i]  = 0.0f;
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
