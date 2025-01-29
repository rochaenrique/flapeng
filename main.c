#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fplist.h"

#define SCREEN_WIDTH  800.0f
#define SCREEN_HEIGHT 600.0f

#define TUBE_COLOR GREEN
#define TUBE_AMOUNT 4
#define TUBE_SPACE_BETWEEN SCREEN_WIDTH / TUBE_AMOUNT
#define TUBE_GAP SCREEN_HEIGHT / 4.0f
#define TUBE_WIDTH TUBE_SPACE_BETWEEN / 4.0f

#define PLAYER_SIZE TUBE_GAP / 3.0f
#define PLAYER_X SCREEN_WIDTH / 2.0f - PLAYER_SIZE / 2.0f
#define PLAYER_COLOR RED
#define PLAYER_JUMP_SPEED 200.0f

#define G 500
#define GAME_SPEED 100

typedef struct {
  int left;
  int top_height;
} Tube;

typedef struct {
  Rectangle rec;
  Texture2D tex;
} TextureMap;

#define random_tube_height() (float)random() / (float)RAND_MAX * SCREEN_HEIGHT/2 + SCREEN_HEIGHT/4
void tubes_init(FP_List *);
void tubes_calc_next(FP_List *);
void tubes_draw(FP_List *, float, TextureMap *);

typedef struct {
  Rectangle *rec;
  float speed;
  FP_Node *close;
} Player;

Tube* next;

void player_update(Player *, float, int *);
bool player_tube_close(Tube *tube);
bool player_tube_colision(Player *player, Tube *tube);

enum { RUNNING, PAUSED, OVER } state = RUNNING;
void game_reset(Player *, FP_List **, int *);

int main()
{
  printf("Initing window!\n");
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flapeng!");

  SetTargetFPS(60);
  
  Texture2D ptex = LoadTexture("res/penguin.png");
  int margin = 10;
  TextureMap peng = {(Rectangle){ margin, margin, (float)ptex.width / 16.0f - margin, (float)ptex.height / 16.0f - margin}, ptex};
  
  Texture2D itex = LoadTexture("res/icepicks.png");
  TextureMap icepick = {(Rectangle){ 0.0f, 0.0f, (float)itex.width / 3.0f, (float)itex.height }, itex};

    
  FP_List* tubes = fp_list_alloc();
  tubes_init(tubes);

  Rectangle prec = { PLAYER_X, 0, PLAYER_SIZE, PLAYER_SIZE };
  Player player = { &prec, 0, tubes->head };
  next = (Tube *)tubes->head->next->this;
  
  float dt;
  int score = 0;
  while (!WindowShouldClose()) {
	ClearBackground(BLUE);
	dt = GetFrameTime();

	player_update(&player, dt, &score);
	tubes_calc_next(tubes);

	if (IsKeyPressed(KEY_P) && state == RUNNING)
	  state = PAUSED;

	if (IsKeyPressed(KEY_R))
	  game_reset(&player, &tubes, &score);
	
	BeginDrawing();
	DrawTexturePro(peng.tex, peng.rec, *player.rec,
				   (Vector2){ (*player.rec).width / 2.0f, (*player.rec).height / 2.0f },
				   player.rec->y, WHITE);

	tubes_draw(tubes, dt, &icepick);
	if (state == OVER)
	  DrawTextEx(GetFontDefault(), TextFormat("GAME OVER\nScore: %i", score),
				 (Vector2){ SCREEN_WIDTH / 2.0f - 40, SCREEN_HEIGHT / 2.0f - 20}, 30, 8, RED);
	else
	  DrawTextEx(GetFontDefault(), TextFormat("Score: %i", score),
				 (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 6.0f }, 16, 2, YELLOW);
	EndDrawing();
  }

  CloseWindow();
  return 0;
}

void player_update(Player *player, float dt, int *score)
{
  if (state == RUNNING) { 
	if (IsKeyPressed(KEY_SPACE)) 
	  player->speed = -PLAYER_JUMP_SPEED;

	if (!player_tube_colision(player, (Tube *)player->close->this)) { 
	  player->rec->y += player->speed * dt;
	  player->speed += G * dt;

	  Tube* tclose;
	  if ((tclose = (Tube *)player->close->this) != next && (player->rec->x > tclose->left + TUBE_WIDTH)) { 
		(*score)++;
		next = tclose;
		player->close = player->close->next;
	  }
	} else
	  state = OVER;
  }
};

bool player_tube_colision(Player *player, Tube *tube)
{
  int rad = (player->rec->width + player->rec->height) / 2;
  return !(player->rec->x + rad / 2 <= tube->left || player->rec->x - rad / 2>= tube->left + TUBE_WIDTH ||
		   (player->rec->y - rad / 2>= tube->top_height && player->rec->y + rad / 2 <= tube->top_height + TUBE_GAP));
}

void tubes_init(FP_List *tubes)
{
  FP_Node *p;
  Tube *t;
  for (int i = 0; i <= TUBE_AMOUNT; i++) 
	if ((p = fp_list_create_append(tubes))) {
	  t = (Tube *)malloc(sizeof(Tube *));
	  t->left = SCREEN_WIDTH + TUBE_SPACE_BETWEEN * i;
	  t->top_height = random_tube_height();
	  p->this = t;
	}
}

void tubes_calc_next(FP_List *tubes)
{
  Tube *t;
  if ((t = (Tube *)tubes->head->this)->left + TUBE_WIDTH <= 0) {
	t->left = ((Tube *)tubes->tail->this)->left + TUBE_SPACE_BETWEEN;
	t->top_height = random_tube_height();
	fp_list_rotate(tubes);
  }
}


void tubes_draw(FP_List *tubes, float dt, TextureMap *ip)
{
  FP_Node *p;
  Tube *t;
  Rectangle dest;
  for (p = tubes->head; p; p = p->next) {
	t = (Tube *)p->this;
	ip->rec.x = (t->top_height % 3) * ip->rec.width;
	DrawRectangleRec(dest = (Rectangle){ t->left, t->top_height + TUBE_GAP, TUBE_WIDTH, SCREEN_WIDTH - (t->top_height + TUBE_GAP) }, WHITE);
	/* DrawTexturePro(ip->tex, ip->rec, */
	/* 			   dest, */
	/* 			   (Vector2){ dest.width / 2.0f, dest.height / 2.0f }, */
	/* 			   0, WHITE); */
	
	DrawRectangleRec(dest = (Rectangle){ t->left, 0, TUBE_WIDTH, t->top_height }, WHITE);
	/* DrawTexturePro(ip->tex, ip->rec, */
	/* 			   dest, */
	/* 			   (Vector2){ dest.width / 2.0f, dest.height / 2.0f }, */
	/* 			   180, WHITE); */
	if (state == RUNNING)
	  t->left -= GAME_SPEED * dt;
  }
}

void game_reset(Player *player, FP_List **tubes, int *score)
{
  *score = 0;
  fp_list_free(*tubes);
  *tubes = fp_list_alloc();
  tubes_init(*tubes);
  
  player->rec->y = player->speed = 0;
  player->rec->x = PLAYER_X;
  player->rec->width = player->rec->height = PLAYER_SIZE;
  player->close = (*tubes)->head;
  next = (Tube *)(*tubes)->head->next->this;
  state = RUNNING;
};
