#include <stdio.h>
#include <time.h>
#include <direct.h>
#include <stdlib.h>
#include <string.h>
#include "particles.h"
#include "utils.h"
#include "constants.h"
#include "colors.h"

void  initGame();
void  processInput();
void  draw();
void  clearParticleGrid();
void  drawSpawnCircle();
void  sandFall();
void  takeScreenshots();
Color getRandomColorShift(Color color);

int          spawnRadius          = 50 / GRID_SIZE;   // spawn radius
const int    srMin                = 10 / GRID_SIZE;   // min spawn radius
const int    srMax                = 250 / GRID_SIZE;  // max spawn radius
size_t       frameCount           = 0;
unsigned     currentParticleType  = 1;
const float  gravity              = 0.1f * (5.0f / GRID_SIZE);
const float  maxVelocity          = 3.0f * (5.0f / GRID_SIZE);
const int    gridLength           = GRID_WIDTH * GRID_HEIGHT;
const int    colorVariance        = 7;

Vector2      mousePosition;
unsigned     *particlesGrid;
float        *velocityGrid;
Color        *pixelColors;
Texture2D    particleTexture;

extern Particle ParticlesList[];

int main(void) {
	printf("Working directory: %s\n", GetWorkingDirectory());

	int result = _mkdir("frames");

	printf("mkdir result: %d\n", result);
	printf("Frames directory exists: %d\n", DirectoryExists("frames"));

	SetRandomSeed(time(NULL));

	initGame();

	while(!WindowShouldClose()) {
		processInput();
		updateParticles(gravity, maxVelocity);
		// sandFall();
		draw();
		
		// takeScreenshots();
		frameCount++;
	}

	UnloadTexture(particleTexture);
	free(pixelColors);
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
	pixelColors   = malloc(gridLength * sizeof(Color));

	if(!particlesGrid || !velocityGrid || !pixelColors) {
		fprintf(stderr, "Failed to allocate memory for the grids\n");
	}

	Image img = GenImageColor(GRID_WIDTH, GRID_HEIGHT, bg);
	particleTexture = LoadTextureFromImage(img);
	UnloadImage(img);

	SetTextureFilter(particleTexture, TEXTURE_FILTER_POINT);
}

void processInput() {
	float mouseWheel = GetMouseWheelMove();
	mousePosition = GetMousePosition();
	mousePosition.x /= GRID_SIZE;
	mousePosition.y /= GRID_SIZE;

	if(IsKeyDown(KEY_LEFT_BRACKET)  || IsKeyDown(KEY_MINUS) || IsKeyDown(KEY_KP_SUBTRACT) || (mouseWheel < 0)) 
		spawnRadius = (spawnRadius <= srMin) ? srMin : spawnRadius - 1;
	if(IsKeyDown(KEY_RIGHT_BRACKET) || IsKeyDown(KEY_EQUAL) || IsKeyDown(KEY_KP_ADD) || (mouseWheel > 0))
		spawnRadius = (spawnRadius >= srMax) ? srMax : spawnRadius + 1;

	if(IsMouseButtonDown(0)) {
		int cx = (int)mousePosition.x;
		int cy = (int)mousePosition.y;
		int r2 = spawnRadius * spawnRadius;
		int cm = (cx + cy) % 2;    // this is just to make sure that we can spawn a particle with 1 pixel size circle.

		for(int x = cx - spawnRadius; x <= cx + spawnRadius; x++) {
			for(int y = cy - spawnRadius; y <= cy + spawnRadius; y++) {
			
				if(x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) continue;
				if(particlesGrid[y * GRID_WIDTH + x] > 0) continue;

				if(currentParticleType != 2 && (x + y) % 2 != cm) continue;
				
				int dx = x - cx;
				int dy = y - cy;
				
				if(dx * dx + dy * dy <= r2) {
					particlesGrid[y * GRID_WIDTH + x] = currentParticleType;
					velocityGrid [y * GRID_WIDTH + x] = 0;
					pixelColors  [y * GRID_WIDTH + x] = getRandomColorShift(ParticlesList[currentParticleType].color);
				}
			}
		}
	}

	if(IsMouseButtonDown(1)) {
		int cx = (int)mousePosition.x;
		int cy = (int)mousePosition.y;
		int r2 = spawnRadius * spawnRadius;

		for(int x = cx - spawnRadius; x <= cx + spawnRadius; x++) {
			for(int y = cy - spawnRadius; y <= cy + spawnRadius; y++) {
			
				if(x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) continue;

				int dx = x - cx;
				int dy = y - cy;
				
				if(dx * dx + dy * dy <= r2) {
					particlesGrid[y * GRID_WIDTH + x] = 0;
					velocityGrid [y * GRID_WIDTH + x] = 0;
					pixelColors  [y * GRID_WIDTH + x] = getRandomColorShift(ParticlesList[0].color);
				}
			}
		}
	}

	if(IsKeyPressed(KEY_C)) clearParticleGrid();
}

void draw() {
	UpdateTexture(particleTexture, pixelColors);

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
		
			if(x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) continue;
			
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
		pixelColors[i]   = transparent;
	}
}

void sandFall() {
	static int timer = 0;
	timer ++;

	if(timer > 3) {
		timer = 0;
		particlesGrid[GetGridIndex(GRID_WIDTH / 2, 0)] = 1;
		velocityGrid [GetGridIndex(GRID_WIDTH / 2, 0)] = 0;
		pixelColors  [GetGridIndex(GRID_WIDTH / 2, 0)] = getRandomColorShift(ParticlesList[1].color);
	}
}

void takeScreenshots() {
	Image screenshot = LoadImageFromScreen();
	Rectangle crop = {0, 225, 1600, 900};
	ImageCrop(&screenshot, crop);
	ExportImage(screenshot, TextFormat("frames/frame_%04zu.png", frameCount));
	UnloadImage(screenshot);
}

Color getRandomColorShift(Color color) {
	return (Color) {
			color.r + GetRandomValue(-colorVariance, colorVariance),
			color.g + GetRandomValue(-colorVariance, colorVariance),
			color.b + GetRandomValue(-colorVariance, colorVariance),
			color.a
		};
}
