#include "main.h"

int main() {
  bool mode = 0;
  float vertGap = 70.0f;
  float pipeSpeed = 40.0f;
  bool showTexture = false;
  bool gameOver = false;
  u16 counter = 0;
  float velocity = 50.0f;
  float gravity = 700.0f;
  u16 currentFrame = 0;
  float frameTime = 0.0f;
  float frameSpeed = 0.12f;
  float offset = 140.0f;
  float gapBetweenPipes = 250.0f;

  typedef struct {
    Vector2 pos;
    Rectangle area;
  } Bird;

  Bird bird = {0};

  PipePair pipes[maxPipes];

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Bird");
  InitAudioDevice();
  SetTargetFPS(60);

  Texture2D day = LoadTexture("./assets/sprites/background-day.png");
  Texture2D night = LoadTexture("./assets/sprites/background-night.png");
  Texture2D message = LoadTexture("./assets/sprites/message.png");
  Texture2D blue_bird_up = LoadTexture("./assets/sprites/bluebird-upflap.png");
  Texture2D blue_bird_mid = LoadTexture("./assets/sprites/bluebird-midflap.png");
  Texture2D blue_bird_down = LoadTexture("./assets/sprites/bluebird-downflap.png");
  Texture2D red_bird_up = LoadTexture("./assets/sprites/redbird-upflap.png");
  Texture2D red_bird_mid = LoadTexture("./assets/sprites/redbird-midflap.png");
  Texture2D red_bird_down = LoadTexture("./assets/sprites/redbird-downflap.png");
  Texture2D greenBottomPipeImg = LoadTexture("./assets/sprites/pipe-green.png");
  Texture2D greenTopPipeImg = LoadTexture("./assets/sprites/pipe-green-flipped.png");
  Texture2D redTopPipeImg = LoadTexture("./assets/sprites/pipe-red-flipped.png");
  Texture2D redBottomPipeImg = LoadTexture("./assets/sprites/pipe-red.png");
  Texture2D lastMsg = LoadTexture("./assets/sprites/gameover.png");
  Sound wing_sound = LoadSound("./assets/audio/wing.wav");
  Sound die_sound = LoadSound("./assets/audio/die.wav");
  Sound hit_sound = LoadSound("./assets/audio/hit.wav");
  Sound jump_sound = LoadSound("./assets/audio/swoosh.wav");
  Sound point_sound = LoadSound("./assets/audio/point.wav");
  Font customFont = LoadFont("./assets/ttyclock.ttf");

  Texture2D birdFrames[3];
  birdFrames[0] = blue_bird_up;
  birdFrames[1] = blue_bird_mid;
  birdFrames[2] = blue_bird_down;

  Texture2D nums[10];
  loadNums(nums);

  Texture2D topPipeImg = redTopPipeImg;
  Texture2D bottomPipeImg = redBottomPipeImg;

  float pipeWidth = topPipeImg.width;
  float pipeHeight = topPipeImg.height;
  float birdWidth = birdFrames[1].width;
  float birdHeight = birdFrames[1].height;

  updateBirdPos(&bird.pos, bird.area.width);
  updateBirdArea(&bird.area, &bird.pos, birdWidth, birdHeight);

  initializePipes(pipes, topPipeImg, bottomPipeImg, vertGap, offset, gapBetweenPipes);
  updatePipesArea(pipes, pipeWidth, pipeHeight);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    if(mode) {
      renderBackground(night);
      topPipeImg = redTopPipeImg;
      bottomPipeImg = redBottomPipeImg;
      birdFrames[0] = red_bird_up;
      birdFrames[1] = red_bird_mid;
      birdFrames[2] = red_bird_down;
    }
    else {
      renderBackground(day);
      topPipeImg = greenTopPipeImg;
      bottomPipeImg = greenBottomPipeImg;
      birdFrames[0] = blue_bird_up;
      birdFrames[1] = blue_bird_mid;
      birdFrames[2] = blue_bird_down;
    }

    if(!gameOver) {

      for (u16 i = 0; i < maxPipes; i++) {
        if (pipes[i].topPos.x + pipeWidth <= 0) {

          // find rightmost pipe
          float maxX = 0;
          for (u16 j = 0; j < maxPipes; j++) {
            if (pipes[j].topPos.x > maxX) {
              maxX = pipes[j].topPos.x;
            }
          }

          // move this pipe to the right of the last pipe
          pipes[i].topPos.x = maxX + gapBetweenPipes;
          pipes[i].bottomPos.x = maxX + gapBetweenPipes;

        }
      }
      if(bird.pos.y >= SCREEN_HEIGHT || bird.pos.y <= 0) {
        PlaySound(die_sound);
        gameOver = true;
      }

      if(IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        showTexture = true;
      }

      if(showTexture){
        if(bird.pos.x > pipes[counter%maxPipes].bottomArea.x + pipeWidth) {
          PlaySound(point_sound);
          if((counter + 1) % 5 == 0) {
            mode = !mode;
            pipeSpeed += 5;
          }
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

        for (u16 i = 0; i < maxPipes; i++) {
          movePipes(&pipes[i].topPos, &pipes[i].bottomPos, dt, pipeSpeed);
        }

        updatePipesArea(pipes, pipeWidth, pipeHeight);

        updateBirdArea(&bird.area, &bird.pos, birdWidth, birdHeight);

        if(frameTime >= frameSpeed) {
          frameTime = 0.0f;
          currentFrame++;
          if(currentFrame >= 3) {
            currentFrame = 0;
          }
        }
        for (u16 i = 0; i < maxPipes; i++) {
          renderObstacles(topPipeImg, bottomPipeImg, pipes[i].topPos, pipes[i].bottomPos);
        }

        DrawTextEx(customFont,TextFormat("%d", counter), (Vector2){50.0f, 50.0f}, 50, 2.0f, WHITE);
        renderBird(birdFrames[currentFrame], bird.pos);
        if(!IsSoundPlaying(wing_sound)) PlaySound(wing_sound);

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
        pipeSpeed = 40.0f;

        // update bird
        updateBirdArea(&bird.area, &bird.pos, birdWidth, birdHeight);
        updateBirdPos(&bird.pos, bird.area.width);

        // rest pipe
        // updatePipesPos(pipes, topPipeImg.width, bottomPipeImg.width, vertGap, offset);

        initializePipes(pipes, topPipeImg, bottomPipeImg, vertGap, offset, gapBetweenPipes);

        //rest collision boxes
        updatePipesArea(pipes, pipeWidth, pipeHeight);
      }
      else {
        renderTxt(lastMsg);
        const char *counterTxt = TextFormat("%d", counter);
        DrawTextEx(customFont,counterTxt,(Vector2){SCREEN_WIDTH / 2.0f - MeasureText(counterTxt, 50), SCREEN_HEIGHT / 4.0f}, 50, 2.0f, WHITE);
      }
    }

    EndDrawing();
  }

  UnloadTexture(greenTopPipeImg);
  UnloadTexture(greenBottomPipeImg);
  UnloadTexture(redTopPipeImg);
  UnloadTexture(redBottomPipeImg);

  UnloadTexture(day);
  UnloadTexture(night);
  UnloadTexture(message);
  UnloadTexture(lastMsg);

  UnloadTexture(blue_bird_up);
  UnloadTexture(blue_bird_mid);
  UnloadTexture(blue_bird_down);

  UnloadTexture(red_bird_up);
  UnloadTexture(red_bird_mid);
  UnloadTexture(red_bird_down);

  UnLoadNums(nums);

  UnloadSound(jump_sound);
  UnloadSound(wing_sound);
  UnloadSound(die_sound);
  UnloadSound(hit_sound);
  UnloadSound(point_sound);

  UnloadFont(customFont);

  CloseWindow();
  return 0;
}

void renderBackground(Texture2D background) {
  for (u16 i = 0; i < 4; i++) {
    Vector2 pos = { background.width * i, 0.0f };
    DrawTextureEx(background,pos, 0.0f, 1, WHITE);
  }
}

void renderTxt(Texture2D msg) {
  Vector2 msgPos = { (SCREEN_WIDTH / 2.0f) - msg.width / 2.0f, (SCREEN_HEIGHT / 2.0f) - msg.height / 2.0f };
  DrawTextureEx(msg,msgPos, 0.0f, 1, WHITE);
}

void loadNums(Texture2D *nums) {
  for (u16 i = 0; i < 10; i++) {
    char* str = "./assets/sprites/%d.png";
    u16 n = strlen(str);
    char path[n];
    snprintf(path, n, str, i);
    nums[i] = LoadTexture(path);
  }
}
void UnLoadNums(Texture2D *nums) {
  for (u16 i = 0; i < 10; i++) {
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
  for (u16 i = 0; i < maxPipes; i++) {
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
  pipes[0].topPos.x = SCREEN_WIDTH + topW;
  pipes[0].topPos.y = -(SCREEN_HEIGHT + offset * 1.5f) + offset;
  pipes[0].bottomPos.x = SCREEN_WIDTH + bottomW;
  pipes[0].bottomPos.y = SCREEN_HEIGHT/ 4.0f + topW - gapV;

  i16 offsetV = 70;

  for (u16 i = 1; i < maxPipes; i++) {
    pipes[i].topPos.x = pipes[i-1].topPos.x;
    pipes[i].topPos.y = pipes[i-1].topPos.y + offsetV;
    pipes[i].bottomPos.x = pipes[i-1].bottomPos.x;
    pipes[i].bottomPos.y = pipes[i-1].bottomPos.y + offsetV;
    if (pipes[i].topPos.y == -150.0f || pipes[i].topPos.y == -570.0f) {
      offsetV = -offsetV;
    }
  }
}

void updatePipesArea(PipePair *pipes, float pipeW, float pipeH) {
  for (u16 i = 0; i < maxPipes; i++) {
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

void initializePipes(PipePair *pipes, Texture2D topPipeImg, Texture2D bottomPipeImg, float gapV, float offset, float gapBetweenPipes) {

  updatePipesPos(pipes, topPipeImg.width, bottomPipeImg.width, gapV, offset);

  for (u16 i = 1; i < maxPipes; i++) {
    pipes[i].topPos.x = pipes[i-1].topPos.x + gapBetweenPipes;
    pipes[i].bottomPos.x = pipes[i-1].topPos.x + gapBetweenPipes;
  }
  updatePipesArea(pipes, topPipeImg.width, topPipeImg.height);
}
