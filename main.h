#include "base.h"
#include <string.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 500
#define maxPipes 12

typedef struct {
  Vector2 topPos;
  Vector2 bottomPos;
  Rectangle topArea;
  Rectangle bottomArea;
} PipePair;

void loadNums(Texture2D *nums);
void UnLoadNums(Texture2D *nums);
void renderBackground(Texture2D background);
void renderCounter(Font fontStyle, Texture2D *nums, u16 counter);
void renderTxt(Texture2D msg);
void drawPipe(Texture2D pipeImg,Vector2 pipe);
void renderBird(Texture2D bird_mid, Vector2 birdPos);
void renderObstacles(Texture2D topPipeImg, Texture2D bottomPipeImg, Vector2 topPipe, Vector2 bottomPipe);
bool isColliding(Rectangle bird, Rectangle topPipe, Rectangle bottomPipe);
void updatePipes(Vector2 *top, Vector2 *bottom, float topW, float bottomW, float gap, float offset);
void updatePipesPos(PipePair *pipes, float topW, float bottomW, float gapV, float offset);
bool checkCollision(Rectangle bird, PipePair *pipes);
void updateBirdPos(Vector2 *pos, float width);
void updateBirdArea(Rectangle *area, Vector2 *pos, float width, float height);
void movePipes(Vector2 *topPos, Vector2 *bottomPos, float dt, float speed);
void updatePipeArea(Rectangle *topArea, Rectangle *bottomArea, Vector2* topPos, Vector2* bottomPos, Texture2D* topPipeImg, Texture2D *bottomPipeImg);
void logPosition(Vector2 pos);
void initializePipes(PipePair *pipes, Texture2D topPipeImg, Texture2D bottomPipeImg, float gapV, float offset, float gapBetweenPipes);
void updatePipesArea(PipePair *pipes, float pipeW, float pipeH);

