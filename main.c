#include "main.h"

int main() {
  float gap = 70.0f;
  float speed = 40.0f;
  bool showTexture = false;
  bool gameOver = false;
  int counter = 0;
  float velocity = 50.0f;
  float gravity = 700.0f;
  int currentFrame = 0;
  float frameTime = 0.0f;
  float frameSpeed = 0.12f;
  float offset = 140.0f;

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

  Texture2D nums[9] = {};
  loadNums(nums);
  
  Texture2D birdFrames[3] = {bird_up, bird_mid, bird_down};

  Vector2 initialTopPipePos = {
    .x = SCREEN_WIDTH + gap + topPipeImg.width,
    .y = -(gap * 2) - offset
  };

  Vector2 initialBottomPipePos = {
    .x = SCREEN_WIDTH + gap + bottomPipeImg.width,
    .y = SCREEN_HEIGHT / 2.0f + gap - offset
  };

    Vector2 birdPos = {
    .x = SCREEN_WIDTH / 2.0 - bird_mid.width / 2.0f, 
    .y = SCREEN_HEIGHT / 2.0f
  };

  Rectangle birdArea = {
    birdPos.x,
    birdPos.y,
    (float)bird_mid.width,
    (float)bird_mid.height
  };

  Rectangle topPipeArea = {
    initialTopPipePos.x,
    initialTopPipePos.y,
    (float)topPipeImg.width,
    (float)topPipeImg.height
  };

  Rectangle bottomPipeArea = {
    initialBottomPipePos.x,
    initialBottomPipePos.y,
    (float)bottomPipeImg.width,
    (float)bottomPipeImg.height
  };

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    renderBackground(background);

    if(!gameOver) {

      if(initialTopPipePos.x + bottomPipeImg.width <= 0) {
        resetPipes(&initialTopPipePos, &initialBottomPipePos, topPipeImg.width, bottomPipeImg.width, gap, offset);
      }

      if(birdPos.y >= SCREEN_HEIGHT || birdPos.y <= 0) {
        PlaySound(die_sound);
        gameOver = true;
      }

      if(IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        showTexture = true;
      }

      if(showTexture){

        if(birdPos.x > bottomPipeArea.x + bottomPipeImg.width && counter == 0) {
          counter++;
          PlaySound(point_sound);
        }

        if(CheckCollisionRecs(birdArea, bottomPipeArea) || CheckCollisionRecs(birdArea, topPipeArea)) {
          PlaySound(hit_sound);
          gameOver = true;
        }
        const float dt = GetFrameTime();
        frameTime += dt;
        birdPos.y += dt * velocity;

        velocity += dt * gravity;
        birdArea.x = birdPos.x;
        birdArea.y = birdPos.y;
        initialTopPipePos.x -= (dt * speed) * 2;
        initialBottomPipePos.x -= (dt * speed) * 2;
        bottomPipeArea.x = initialBottomPipePos.x;
        topPipeArea.x = initialTopPipePos.x;

        if(frameTime >= frameSpeed) {
          frameTime = 0.0f;
          currentFrame++;
          if(currentFrame >= 3) {
            currentFrame = 0;
          }
        }

        renderObstacles(topPipeImg, bottomPipeImg, initialTopPipePos, initialBottomPipePos);
        renderObstacles(topPipeImg, bottomPipeImg, initialTopPipePos, initialBottomPipePos);
        renderCounter(nums,counter);
        renderBird(birdFrames[currentFrame], birdPos);
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

        // reset positions
        birdPos.x = SCREEN_WIDTH / 2.0 - bird_mid.width / 2.0f; 
        birdPos.y = SCREEN_HEIGHT / 2.0f;

        // rest pipe
        resetPipes(&initialTopPipePos, &initialBottomPipePos, topPipeImg.width, bottomPipeImg.width, gap, offset);

        //rest collision boxes
        birdArea.y = birdPos.y;
        topPipeArea.x = initialTopPipePos.x;
        topPipeArea.y = initialTopPipePos.y;
        bottomPipeArea.x = initialBottomPipePos.x;
        bottomPipeArea.y = initialBottomPipePos.y;
      }
      else {
        renderTxt(lastMsg);
        renderCounter(nums,counter);
      }
    }

    EndDrawing();
  }

  UnloadTexture(bottomPipeImg);
  UnloadTexture(topPipeImg);
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
  for (int i = 0; i < 4; i++) {
    Vector2 pos = { background.width * i, 0.0f };
    DrawTextureEx(background,pos, 0.0f, 1, WHITE);
  }
}

void renderCounter(Texture2D *nums, int counter) {
  Texture2D count = nums[counter];
  Vector2 pos = {
    .x = SCREEN_WIDTH / 2.0f - count.width / 2.0f,
    .y = SCREEN_HEIGHT / 6.0f
  };
  DrawTextureEx(count,pos, 0, 1.0f,WHITE);
}

void renderTxt(Texture2D msg) {
  Vector2 msgPos = { (SCREEN_WIDTH / 2.0f) - msg.width / 2.0f, (SCREEN_HEIGHT / 2.0f) - msg.height / 2.0f };
  DrawTextureEx(msg,msgPos, 0.0f, 1, WHITE);
}

void loadNums(Texture2D *nums) {
  for (int i = 0; i < 9; i++) {
    char* str = "./assets/sprites/%d.png";
    int n = strlen(str);
    char path[n];
    snprintf(path, n, str, i);
    nums[i] = LoadTexture(path);
  }
}
void UnLoadNums(Texture2D *nums) {
  for (int i = 0; i < 9; i++) {
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

void resetPipes(Vector2 *top, Vector2 *bottom, float topW, float bottomW, float gap, float offset) {
    top->x = SCREEN_WIDTH + gap + topW;
    top->y = -(gap * 2) - offset;
    bottom->x = SCREEN_WIDTH + gap + topW;
    bottom->y = SCREEN_HEIGHT / 2.0f + gap - offset;
}

Texture2D updatePipePos(Vector2 top, Vector2 bottom) {
  const Texture2D pos = {};
  return pos;
}
