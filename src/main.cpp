#include "raylib.h"

enum GameState {
  PLAYING,
  START,
  END,
};

struct Bird {
  Vector2 position;
  Vector2 velocity;
  float rotation;
  float jumpForce;
};

struct Pipe {
  Vector2 position;
  float speed;
  float gapHeight;
};

void drawPipe(Rectangle sourceRect, Rectangle topDestRect,
              Rectangle bottomDestRect, Vector2 origin,
              const Texture2D &texture) {
  DrawTexturePro(texture, sourceRect, bottomDestRect, origin, 0.0f, WHITE);
  DrawTexturePro(texture, sourceRect, topDestRect, origin, 180.0f, WHITE);
}

int main() {
  const int w = 288;
  const int h = 512;

  InitWindow(w, h, "Flappybird");

  SetTargetFPS(60);

  Texture2D background = LoadTexture("../assets/sprites/background-night.png");
  Texture2D bluebird = LoadTexture("../assets/sprites/bluebird-midflap.png");
  Texture2D redpipe = LoadTexture("../assets/sprites/pipe-red.png");

  Bird player{.position = {w / 2.0f, h / 2.0f},
              .velocity = {0.0f, 0.0f},
              .rotation = 0.0f,
              .jumpForce = 4.0f};

  Pipe pipe{.position = {(float)w + redpipe.width, h - 50},
            .speed = 2.0f,
            .gapHeight = 100.0f};

  Rectangle playerRect = {0.0f, 0.0f, (float)bluebird.width,
                          (float)bluebird.height};

  Rectangle PipeRect = {0.0f, 0.0f, (float)redpipe.width,
                        (float)redpipe.height};

  Vector2 pipeOrigin = {(float)redpipe.width / 2.0f,
                        (float)redpipe.height / 2.0f};

  Vector2 playerOrgin = {(float)bluebird.width / 2.0f,
                         (float)bluebird.height / 2.0f};

  GameState state = START;
  while (!WindowShouldClose()) {

    Rectangle playerDesRect = {.x = player.position.x,
                               .y = player.position.y,
                               .width = (float)bluebird.width,
                               .height = (float)bluebird.height};

    Rectangle bottomPipeDesRect = {.x = pipe.position.x,
                                   .y = pipe.position.y,
                                   .width = (float)redpipe.width,
                                   .height = (float)redpipe.height};

    Rectangle topPipeDesRect = {.x = pipe.position.x,
                                .y = pipe.position.y - redpipe.height -
                                     pipe.gapHeight,
                                .width = (float)redpipe.width,
                                .height = (float)redpipe.height};

    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawTexture(background, 0, 0, WHITE);

    DrawTexturePro(bluebird, playerRect, playerDesRect, playerOrgin,
                   player.rotation, WHITE);

    drawPipe(PipeRect, topPipeDesRect, bottomPipeDesRect, pipeOrigin, redpipe);

    switch (state) {
    case START:
      if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        state = PLAYING;
      }
      break;
    case PLAYING:
      if (player.position.y < 0) {
        player.position.y = 0;
        player.velocity.y = 0;
      } else if (player.position.y + bluebird.height > h) {
        player.position.y = h - bluebird.height;
        player.velocity.y = 0;
      }

      if (IsKeyDown(KEY_UP) || IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        player.velocity.y -= player.jumpForce;
      }

      player.velocity.y += 0.5f; // Gravity
      player.position.y += player.velocity.y;

      pipe.position.x -= pipe.speed;
      break;
    case END:
      break;
    }

    EndDrawing();
  }

  UnloadTexture(background);
  UnloadTexture(bluebird);
  UnloadTexture(redpipe);

  CloseWindow();
}
