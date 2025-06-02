#include "raylib.h"

int main() {
  const int w = 288;
  const int h = 512;

  InitWindow(w, h, "Flappybird");

  SetTargetFPS(60);

  Texture2D background = LoadTexture("../assets/sprites/background-night.png");
  Texture2D bird = LoadTexture("../assets/sprites/bluebird-midflap.png");

  float bY = h / 2.0 - bird.height / 2.0;

  while (!WindowShouldClose()) {

    if (IsKeyDown(KEY_UP))
      bY -= 5.0;

    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("Welcome to Flappybird!", 10, 10, 20, DARKGRAY);

    DrawTexture(background, 0, 0, WHITE);
    DrawTexture(bird, w / 2 - bird.width / 2, bY, WHITE);

    if (bY > h - bird.height) {
      bY = h - bird.height;
    } else if (bY < 0) {
      bY = 0;
    } else {
      bY += 3.0;
    }

    EndDrawing();
  }

  UnloadTexture(background);
  CloseWindow();
}
