#include "main.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>

int main() {
  float vertical_gap = 70.0f;
  float speed = 40.0f;
  bool showTexture = false;
  bool gameOver = false;
  u16 counter = 0;
  float velocity = 50.0f;
  float gravity = 700.0f;
  u8 currentFrame = 0;
  float frameTime = 0.0f;
  float frameSpeed = 0.12f;
  float offset = 140.0f;
  float gapBetweenPipes = 200.0f;

  typedef struct {
    Vector2 pos;
    Rectangle area;
  } Bird;

  Bird bird = {0};

  PipePair pipes[maxPipes];

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Bird");
  InitAudioDevice();
  SetTargetFPS(60);

  Texture2D background = LoadTexture("./assets/sprites/background-day.png");
  Texture2D message = LoadTexture("./assets/sprites/message.png");
  Texture2D bird_up = LoadTexture("./assets/sprites/bluebird-upflap.png");
  Texture2D bird_mid = LoadTexture("./assets/sprites/bluebird-midflap.png");
  Texture2D bird_down = LoadTexture("./assets/sprites/bluebird-downflap.png");
  Texture2D bottomPipeImg = LoadTexture("./assets/sprites/pipe-green.png");
  Texture2D topPipeImg = LoadTexture("./assets/sprites/pipe-green-flipped.png");
  Texture2D lastMsg = LoadTexture("./assets/sprites/gameover.png");
  Sound wing_sound = LoadSound("./assets/audio/wing.wav");
  Sound die_sound = LoadSound("./assets/audio/die.wav");
  Sound hit_sound = LoadSound("./assets/audio/hit.wav");
  Sound jump_sound = LoadSound("./assets/audio/swoosh.wav");
  Sound point_sound = LoadSound("./assets/audio/point.wav");
  Font customFont = LoadFont("./assets/ttyclock.ttf");

  SetSoundVolume(jump_sound, 0.2f);

  Texture2D birdFrames[3] = {bird_up, bird_mid, bird_down};

  Texture2D nums[10];
  loadNums(nums);

  updateBirdPos(&bird.pos, bird.area.width);
  updateBirdArea(&bird.area, &bird.pos, bird_mid.width, bird_mid.height);

  initializePipes(pipes, topPipeImg, bottomPipeImg, vertical_gap, offset, gapBetweenPipes);
  updatePipesArea(pipes, topPipeImg.width, topPipeImg.height);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    renderBackground(background);

    if(!gameOver) {

      if(pipes[maxPipes-1].topPos.x + bottomPipeImg.width <= 0) {
        updatePipesPos(pipes, topPipeImg.width, bottomPipeImg.width, vertical_gap, offset);
      }

      if(bird.pos.y >= SCREEN_HEIGHT || bird.pos.y <= 0) {
        PlaySound(die_sound);
        gameOver = true;
      }

      if(IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        showTexture = true;
      }

      if(showTexture){

        if(bird.pos.x > pipes[counter].bottomArea.x + bottomPipeImg.width) {
          PlaySound(point_sound);
          counter++;
        }

        if(checkCollision(bird.area, pipes)) {
          PlaySound(hit_sound);
          gameOver = true;
        }

        const float dt = GetFrameTime();
        frameTime += dt;
        bird.pos.y += dt * velocity;

        velocity += dt * gravity;

        for (u8 i = 0; i < maxPipes; i++) {
          movePipes(&pipes[i].topPos, &pipes[i].bottomPos, dt, speed);
        }
        updatePipesArea(pipes, topPipeImg.width, topPipeImg.height);

        updateBirdArea(&bird.area, &bird.pos, bird_mid.width, bird_mid.height);

        if(frameTime >= frameSpeed) {
          frameTime = 0.0f;
          currentFrame++;
          if(currentFrame >= 3) {
            currentFrame = 0;
          }
        }
        for (u8 i = 0; i < maxPipes; i++) {
            renderObstacles(topPipeImg, bottomPipeImg, pipes[i].topPos, pipes[i].bottomPos);
        }

        renderCounter(customFont, nums,counter);
        renderBird(birdFrames[currentFrame], bird.pos);
        PlaySound(wing_sound);

        if(IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
          PlaySound(jump_sound);
          velocity = -250.0f;
        }
      } 
      else {
        renderTxt(message);
      }
    }
    else {
      if(IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { 
        gameOver = false;
        showTexture = true;
        velocity = 0;
        counter = 0;
        frameTime = 0;

        // update bird
        updateBirdArea(&bird.area, &bird.pos, bird_mid.width, bird_mid.height);
        updateBirdPos(&bird.pos, bird.area.width);

        // rest pipe
        updatePipesPos(pipes, topPipeImg.width, bottomPipeImg.width, vertical_gap, offset);

        initializePipes(pipes, topPipeImg, bottomPipeImg, vertical_gap, offset, gapBetweenPipes);

        //rest collision boxes
        updatePipesArea(pipes, topPipeImg.width, topPipeImg.height);
      }
      else {
        renderTxt(lastMsg);
        renderCounter(customFont, nums,counter);
      }
    }

    EndDrawing();
  }

  UnloadTexture(topPipeImg);
  UnloadTexture(bottomPipeImg);
  UnloadTexture(lastMsg);
  UnloadTexture(background);
  UnloadTexture(message);
  UnloadTexture(bird_up);
  UnloadTexture(bird_mid);
  UnloadTexture(bird_down);
  UnloadSound(jump_sound);
  UnloadSound(wing_sound);
  UnloadSound(die_sound);
  UnloadSound(hit_sound);
  UnloadSound(point_sound);
  UnLoadNums(nums);

  CloseWindow();
  return 0;
}

void renderBackground(Texture2D background) {
  for (u8 i = 0; i < 4; i++) {
    Vector2 pos = { background.width * i, 0.0f };
    DrawTextureEx(background,pos, 0.0f, 1, WHITE);
  }
}

void renderCounter(Font fontStyle, Texture2D *nums, u16 counter) {
  Texture2D count = nums[counter % 10];
  // printf("count before: %d\n",counter % 10);
  // counter/=10;
  // printf("count: after %d\n",counter);
  Vector2 pos = {
    .x = 50.0f,
    .y = 50.0f
  };
  DrawTextEx(fontStyle,TextFormat("%d", counter), pos, 30, 2.0f, WHITE);
  // if(counter > 9) {
  //   Texture2D nextcount = nums[counter];
  //   Vector2 nextPos = {
  //     .x = pos.x + count.width,
  //     .y = 50.0f
  //   };
  //   DrawTextureEx(count,pos, 0, 1.0f,RED);
  //   DrawTextureEx(nextcount,pos, 0, 1.0f,RED);
  // }
  // else {
    // DrawTextureEx(count,pos, 0, 1.0f,WHITE);
  // }
}

void renderTxt(Texture2D msg) {
  Vector2 msgPos = { (SCREEN_WIDTH / 2.0f) - msg.width / 2.0f, (SCREEN_HEIGHT / 2.0f) - msg.height / 2.0f };
  DrawTextureEx(msg,msgPos, 0.0f, 1, WHITE);
}

void loadNums(Texture2D *nums) {
  for (u8 i = 0; i < 10; i++) {
    char* str = "./assets/sprites/%d.png";
    u8 n = strlen(str);
    char path[n];
    snprintf(path, n, str, i);
    nums[i] = LoadTexture(path);
  }
}
void UnLoadNums(Texture2D *nums) {
  for (u8 i = 0; i < 10; i++) {
    UnloadTexture(nums[i]);
  }
}

void renderBird(Texture2D bird_mid, Vector2 birdPos) {
  DrawTextureEx(bird_mid,birdPos,0, 1, WHITE);
}

void renderObstacles(Texture2D topPipeImg, Texture2D bottomPipeImg, Vector2 topPipe, Vector2 bottomPipe) {
  drawPipe(topPipeImg, topPipe);
  drawPipe(bottomPipeImg, bottomPipe);
}

void drawPipe(Texture2D pipeImg,Vector2 pipe) {
  DrawTextureEx(pipeImg, pipe, 0, 1, WHITE);
}

bool checkCollision(Rectangle bird, PipePair *pipes) {
  for (u8 i = 0; i < maxPipes; i++) {
    if(CheckCollisionRecs(bird, pipes[i].topArea) || CheckCollisionRecs(bird, pipes[i].bottomArea)) return true;
  }
  return false;
}

void updateBirdPos(Vector2 *pos, float width) {
    pos->x = SCREEN_WIDTH / 2.0 - width / 2.0f;
    pos->y = SCREEN_HEIGHT / 2.0f;
}

void updateBirdArea(Rectangle *area, Vector2 *pos, float width, float height) {
    area->x = pos->x;
    area->y = pos->y;
    area->width = width;
    area->height = height;
}

void movePipes(Vector2 *topPos, Vector2 *bottomPos, float dt, float speed) {
    topPos->x -= (dt * speed) * 2;
    bottomPos->x -= (dt * speed) * 2;
}

void updatePipesPos(PipePair *pipes, float topW, float bottomW, float gapV, float offset) {
  for (u8 i = 0; i < maxPipes; i++) {
    pipes[i].topPos.x = SCREEN_WIDTH / 1.5f + gapV + topW;
    pipes[i].topPos.y = -(gapV * 2) - offset;
    pipes[i].bottomPos.x = SCREEN_WIDTH / 1.5f + gapV + bottomW;
    pipes[i].bottomPos.y = SCREEN_HEIGHT / 2.0f + gapV - offset;
  }
}

void updatePipesArea(PipePair *pipes, float pipeW, float pipeH) {
  for (u8 i = 0; i < maxPipes; i++) {
    pipes[i].topArea.x = pipes[i].topPos.x;
    pipes[i].topArea.y = pipes[i].topPos.y;
    pipes[i].bottomArea.x = pipes[i].bottomPos.x;
    pipes[i].bottomArea.y = pipes[i].bottomPos.y;

    pipes[i].topArea.width = pipeW;
    pipes[i].bottomArea.width = pipeW;
    pipes[i].topArea.height = pipeH;
    pipes[i].bottomArea.height = pipeH;
  }
}

void logPosition(Vector2 pos) {
  printf("x: %f y: %f\n", pos.x, pos.y);
}

void initializePipes(PipePair *pipes, Texture2D topPipeImg, Texture2D bottomPipeImg, float gapV, float offset, float gapBetweenPipes) {

  updatePipesPos(pipes, topPipeImg.width, bottomPipeImg.width, gapV, offset);

  for (u8 i = 1; i < maxPipes; i++) {
    pipes[i].topPos.x = pipes[i-1].topPos.x + gapBetweenPipes;
    pipes[i].bottomPos.x = pipes[i-1].topPos.x + gapBetweenPipes;
  }
  updatePipesArea(pipes, topPipeImg.width, topPipeImg.height);
}
