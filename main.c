#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

// player state
#define PLAYER_SPEED 200
#define PLAYER_WIDTH 20
#define FALLING 0
#define JUMPING 1
#define STUCK 2

//tubes
#define MAX_TUBES 2
#define TUBE_COLOR GREEN
#define TUBE_WIDTH 100

#define GAME_SPEED 200

typedef struct Player {
  Vector2 position;
  float speed;
  char state;
} Player;

typedef struct Tube {
  int y;
  int height;
} Tube;

int main()
{
  printf("Initing window!\n");
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flapeng!");

  SetTargetFPS(60);

  Player player = {0};
  player.position = (Vector2){ 100, 100 };
  player.speed = -PLAYER_SPEED;
  player.state = FALLING;

  Tube tubes[MAX_TUBES] = {
	{0, 200},
	{SCREEN_HEIGHT-300, 300}
  };
  
  int tubesX = SCREEN_WIDTH-TUBE_WIDTH;
  float dt;
  float jumpTime = 0;

  while (!WindowShouldClose()) {
	dt = GetFrameTime();
	jumpTime++;
	
	if (IsKeyDown(KEY_SPACE) && player.state != JUMPING) {
	  jumpTime = 0;
	  player.state = JUMPING;
	  player.speed = PLAYER_SPEED;
	}
	if (IsKeyDown(KEY_S)) {
	  player.state = STUCK;
	  player.speed = 0;
	}
	if (IsKeyDown(KEY_F) || (player.state == JUMPING && jumpTime > 120)) {
	  player.state = FALLING;
	  player.speed = -PLAYER_SPEED;
	  jumpTime = 0;
	}
	
	player.position.y -= player.speed * dt;

	if (player.position.y > SCREEN_HEIGHT || player.position.y < 0) {
	  player.position.y = 0;
  	  player.position.x = 0;
	}

	tubesX -= GAME_SPEED * dt;

	if (tubesX <= 0) {
	  tubesX = SCREEN_WIDTH - TUBE_WIDTH;
	  for (int i = 0; i < MAX_TUBES/2; i += 2) {
		int r = (float)random() / (float)RAND_MAX * SCREEN_HEIGHT/2 + SCREEN_HEIGHT/4;
		tubes[i].height = r;
		tubes[i+1].y = r + r/3;
		tubes[i+1].height = SCREEN_HEIGHT - (r + r/3);
	  }
	}

	ClearBackground(BLUE);
	BeginDrawing();
	for (int i = 0; i < MAX_TUBES; i++)
	  DrawRectangle(tubesX, tubes[i].y, TUBE_WIDTH, tubes[i].height, TUBE_COLOR);
	DrawRectangle(player.position.x, player.position.y, PLAYER_WIDTH, PLAYER_WIDTH, RED);
	EndDrawing();
  }

  CloseWindow();
  return 0;
}
