#include <raylib.h>
#include <stdio.h>
#include <string.h>


#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 500

void loadNums(Texture2D *nums);
void UnLoadNums(Texture2D *nums);
void renderBackground(Texture2D background);
void renderCounter(Texture2D *nums, int counter);
void renderTxt(Texture2D msg);
void drawPipe(Texture2D pipeImg,Vector2 pipe);
void renderBird(Texture2D bird_mid, Vector2 birdPos);
void renderObstacles(Texture2D topPipeImg, Texture2D bottomPipeImg, Vector2 topPipe, Vector2 bottomPipe);
bool isColliding(Rectangle bird, Rectangle topPipe, Rectangle bottomPipe);
void resetPipes(Vector2 *top, Vector2 *bottom, float topW, float bottomW, float gap, float offset);
Texture2D updatePipePos(Vector2 top, Vector2 bottom);
