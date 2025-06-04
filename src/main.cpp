#include "raylib.h"
#include <iostream>
#include <random>

#define FRAME_SPEED 8
#define WIDTH 288
#define HEIGHT 512

enum Status {
  PLAYING,
  START,
  END,
};

struct GameState {
  float floor_height;
  float speed;
  Status status;
  int score;
};

struct Bird {
  Vector2 position;
  Vector2 velocity;
  float rotation;
  float jumpForce;
  float maxVelocity;
  bool canFlap;
  float flapCooldownTimer;
  float flapCooldownDuration;
};

struct Pipe {
  Vector2 position;
  float halfGapHeight;
  bool passed;
};

void resetGame(Pipe &pipes, Bird &player, GameState &state) {
  pipes.position.x = (float)(WIDTH + 100);
  pipes.position.y = (float)(HEIGHT / 2.0f);
  pipes.passed = false;
  player.position.x = (float)(WIDTH / 2.0f);
  player.position.y = (float)(HEIGHT / 2.0f);
  player.velocity = {0.0f, 0.0f};
  player.rotation = 0.0f;
  player.canFlap = true;
  player.flapCooldownTimer = 0.0f;
  state.score = 0;
}

int main(int argc, char *argv[]) {
  std::mt19937 engine;
  std::uniform_int_distribution<int> distribution(-100, 100);

  if (argc > 1) {
    unsigned int seed = std::stoi(argv[1]);
    std::cout << "seed: " << seed << std::endl;
    engine.seed(seed);
  } else {
    unsigned int seed = 123456789;
    std::cout << "seed: " << seed << std::endl;
    engine.seed(seed);
  }

  auto get_random_pipe_y = [&]() {
    return (float)(HEIGHT / 2.0f) + distribution(engine);
  };

  InitWindow(WIDTH, HEIGHT, "Flappybird");

  SetTargetFPS(60);

  Texture2D background = LoadTexture("../assets/sprites/background-night.png");
  Texture2D base = LoadTexture("../assets/sprites/base.png");
  Texture2D bluebird_midflap =
      LoadTexture("../assets/sprites/bluebird-midflap.png");
  Texture2D bluebird_upflap =
      LoadTexture("../assets/sprites/bluebird-upflap.png");
  Texture2D bluebird_downflap =
      LoadTexture("../assets/sprites/bluebird-downflap.png");
  Texture2D pipe = LoadTexture("../assets/sprites/pipe-red.png");
  Texture2D gameover = LoadTexture("../assets/sprites/gameover.png");

  int frameCounter = 0;
  int framesSpeed = 8;
  int currentFrame = 0;

  Texture2D bluebird_textures[] = {bluebird_upflap, bluebird_midflap,
                                   bluebird_downflap, bluebird_midflap};

  GameState State{
      .floor_height = base.height / 3.0f,
      .speed = 2.0f,
      .status = START,
      .score = 0,
  };

  Bird player{
      .position = {WIDTH / 2.0f, HEIGHT / 2.0f - State.floor_height},
      .velocity = {0.0f, 0.0f},
      .rotation = 0.0f,
      .jumpForce = 13.0f,
      .maxVelocity = 8.0f,
      .canFlap = true,
      .flapCooldownTimer = 0.0f,
      .flapCooldownDuration = 0.2f, // 0.2 seconds cooldown
  };

  Rectangle playerRect = {0.0f, 0.0f, (float)bluebird_midflap.width,
                          (float)bluebird_midflap.height};

  Vector2 playerOrgin = {(float)bluebird_midflap.width / 2.0f,
                         (float)bluebird_midflap.height / 2.0f};

  Pipe pipes{.position = {(float)(WIDTH + pipe.width),
                          HEIGHT / 2.0f - State.floor_height},
             .halfGapHeight = 60.0f,
             .passed = false};

  Rectangle bottomPipeSource = {0.0f, 0.0f, (float)pipe.width,
                                (float)pipe.height};

  Vector2 bottomPipeOrigin = {(float)pipe.width / 2.0f, 0.0f};

  Rectangle topPipeSource = {0.0f, 0.0f, (float)pipe.width,
                             -(float)pipe.height};

  Vector2 topPipeOrigin = {(float)pipe.width / 2.0f, 0.0f};

  Vector2 floor = {0.0f, (float)(HEIGHT - State.floor_height)};

  while (!WindowShouldClose()) {
    Rectangle playerDesRect = {.x = player.position.x,
                               .y = player.position.y,
                               .width = (float)bluebird_midflap.width,
                               .height = (float)bluebird_midflap.height};

    Rectangle bottomPipeDesRect = {.x = pipes.position.x -
                                        (float)pipe.width / 2.0f,
                                   .y = pipes.position.y + pipes.halfGapHeight,
                                   .width = (float)pipe.width,
                                   .height = (float)pipe.height};

    Rectangle topPipeDesRect = {
        .x = pipes.position.x - (float)pipe.width / 2.0f,
        .y = pipes.position.y - pipes.halfGapHeight - (float)pipe.height,
        .width = (float)pipe.width,
        .height = (float)pipe.height};

    BeginDrawing();
    ClearBackground(RAYWHITE);

    // background
    DrawTexture(background, 0, 0, WHITE);

    // burd
    DrawTexturePro(bluebird_textures[currentFrame], playerRect, playerDesRect,
                   playerOrgin, player.rotation, WHITE);

    // Pipe 1
    DrawTexturePro(pipe, bottomPipeSource, bottomPipeDesRect, bottomPipeOrigin,
                   0.0f, WHITE);
    DrawTexturePro(pipe, topPipeSource, topPipeDesRect, topPipeOrigin, 0.0f,
                   WHITE);

    // Base
    DrawTexture(base, floor.x, HEIGHT - State.floor_height, WHITE);
    DrawTexture(base, floor.x + WIDTH, HEIGHT - State.floor_height, WHITE);

    switch (State.status) {
    case START:
      if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        State.status = PLAYING;
      }
      break;
    case PLAYING:
      if (player.position.y < 0) {
        player.position.y = 0;
        player.velocity.y = 0;
      }
      if (player.position.y - bluebird_midflap.height - State.floor_height >
          HEIGHT - State.floor_height) {
        player.position.y = HEIGHT - bluebird_midflap.height / 2.0f;
        player.velocity.y = 0;
        State.status = END;
      }

      if (floor.x <= -base.width) {
        floor.x = 0;
      }

      if (pipes.position.x < -pipe.width) {
        pipes.position.x = (float)(WIDTH + pipe.width);
        pipes.position.y = get_random_pipe_y();
        pipes.passed = false;
      }

      if (player.position.x > pipes.position.x && !pipes.passed) {
        State.score++;
        pipes.passed = true;
      }

      // Collision detection
      if (CheckCollisionRecs(playerDesRect, bottomPipeDesRect) ||
          CheckCollisionRecs(playerDesRect, topPipeDesRect)) {
        State.status = END;
      }

      if ((IsKeyPressed(KEY_UP) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) &&
          player.canFlap) {
        player.velocity.y -= player.jumpForce;
        player.canFlap = false;
        player.flapCooldownTimer = player.flapCooldownDuration;
      }

      if (!player.canFlap) {
        player.flapCooldownTimer -= GetFrameTime();
        if (player.flapCooldownTimer <= 0) {
          player.canFlap = true;
        }
      }

      player.velocity.y += 0.5f; // Gravity
      if (player.velocity.y > player.maxVelocity) {
        player.velocity.y = player.maxVelocity;
      }
      player.position.y += player.velocity.y;
      player.rotation = player.velocity.y * 3.0f;

      frameCounter++;
      if (frameCounter >= (60 / framesSpeed)) {
        frameCounter = 0;
        currentFrame++;
        if (currentFrame > 3) {
          currentFrame = 0;
        }
      }

      pipes.position.x -= State.speed;
      floor.x -= State.speed;

      break;
    case END:
      DrawTexture(gameover, WIDTH / 2 - gameover.width / 2,
                  HEIGHT / 2 - gameover.height / 2, WHITE);
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        State.status = START;
        resetGame(pipes, player, State);
      }
      break;
    }
    DrawText(TextFormat("SCORE: %i", State.score), 10, 10, 20, BLACK);
    EndDrawing();
  }

  UnloadTexture(background);
  UnloadTexture(base);
  UnloadTexture(bluebird_midflap);
  UnloadTexture(bluebird_upflap);
  UnloadTexture(bluebird_downflap);
  UnloadTexture(pipe);
  UnloadTexture(gameover);

  CloseWindow();
}
