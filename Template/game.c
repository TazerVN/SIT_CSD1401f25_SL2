#include "cprocessing.h"
#include "mainmenu.h"
#include "utils.h"

typedef struct Player {
	CP_Vector position;
	CP_Vector direction;
	int diameter;
	CP_Color color;
	CP_BOOL control;
	int speed;
	int alpha;
	int rotation;
};


struct Player p1;
struct Player p2;

CP_Font font;

CP_Vector mouse_position;

void init_player(struct Player* player, CP_Vector position, int diameter, CP_Color color, CP_BOOL control, int speed) {
	player->position.x = position.x;
	player->position.y = position.y;
	player->diameter = diameter;
	player->color = color;
	player->control = control;
	player->speed = speed;
	player->direction.x = 0;
	player->direction.y = 0;
	player->rotation = 0;

}

void drawPlayer(struct Player* player) {
	int window_w = CP_System_GetWindowWidth();
	int window_h = CP_System_GetWindowHeight();

	float px = player->position.x;
	float py = player->position.y;

	float dx = player->direction.x;
	float dy = player->direction.y;

	int diameter = player->diameter;

	int rotation = player->rotation;

	float speed = player->speed;

	float dt = CP_System_GetDt();


	//wall collision
	if (px + diameter / 2 + dx * speed >= window_w && dx > 0 || px - diameter / 2 + dx * speed <= 0 && dx < 0 ||
		(py + diameter / 2 + dy * speed >= window_h && dy > 0) || (py - diameter / 2 + dy * speed <= 0 && dy < 0)) {
		if (player->control == FALSE) {
			if (p1.direction.x * p2.direction.x + p1.direction.y * p2.direction.y > 0) { //dot production experiment
				if (CP_Random_GetBool() == TRUE) {
					rotation = rotation + 90 >= 360 ? rotation + 90 - 360 : rotation + 90;
				}
				else {
					rotation = rotation + 270 >= 360 ? rotation + 270 - 360 : rotation + 270;
				}
			}
			else {
				rotation = rotation + 180 >= 360 ? rotation + 180 - 360 : rotation + 180;
			}
		}
	}
	


	if (player->control == TRUE) {
		if (CP_Input_KeyDown(KEY_D)) {
			if (dx < 1) {
				dx += dt*5;
			}
			rotation = 0;
		}
		if (CP_Input_KeyDown(KEY_A)) {
			if (dx > -1) {
				dx -= dt*5;
			}
			rotation = 180;
		}
		if (CP_Input_KeyDown(KEY_W)) {
			if (dy > -1) {
				dy -= dt*5;
			}
			rotation = 270;

		}
		if (CP_Input_KeyDown(KEY_S)) {
			if (dy < 1) {
				dy += dt*5;
			}
			rotation = 90;


		}
	}
	else {
		if (rotation == 0) {
			dx = 1;
		}
		if (rotation == 180) {
			dx = -1;
		}
		if (rotation == 90) {
			dy = 1;
		}
		if (rotation == 270) {
			dy = -1;
		}
	}

	player->rotation = rotation;

	if (dx > 0.00) {
		dx = dx - dt < 0 ? 0 : dx - dt;
	}
	if (dx < 0.00) {
		dx = dx + dt > 0 ? 0 : dx + dt;
	}
	if (dy > 0.00) {
		dy = dy - dt < 0 ? 0 : dy - dt;
	}
	if (dy < 0.00) {
		dy = dy + dt > 0 ? 0 : dy + dt;
	}

	
	dx = CP_Math_ClampFloat(dx, -1, 1);
	dy = CP_Math_ClampFloat(dy, -1, 1);

	px += dx * player->speed;
	py += dy * player->speed;

	px = CP_Math_ClampFloat(px, 0 + diameter / 2, window_w - diameter / 2);
	py = CP_Math_ClampFloat(py, 0 + diameter / 2, window_h - diameter / 2);

	player->position.x = px;
	player->position.y = py;
	player->direction.x = dx;
	player->direction.y = dy;

	CP_Settings_Fill(player->color);
	CP_Graphics_DrawCircle(px, py, diameter);
	CP_Settings_Fill(CP_Color_Create(255 ,255 ,255 ,255));
	CP_Graphics_DrawTriangleAdvanced(px - diameter / 4, py + diameter / 2 - 10 , px - diameter/4, py - diameter / 2 + 10, px + diameter/2, py, rotation);
}



int velocity_col(struct Player* p1, struct Player* p2) {
	if (p1->position.x + p1->diameter / 2 + p1->direction.x * p1->speed >= p2->position.x - p2->diameter / 2 + p2->direction.x * p2->speed
		&& p1->position.x - p1->diameter / 2 + p1->direction.x * p1->speed <= p2->position.x + p2->diameter / 2 + p2->direction.x * p2->speed
		&& p1->position.y + p1->diameter / 2 + p1->direction.y * p1->speed >= p2->position.y - p2->diameter / 2 + p2->direction.y * p2->speed
		&& p1->position.y - p1->diameter / 2 + p1->direction.y * p1->speed <= p2->position.y + p2->diameter / 2 + p2->direction.y * p2->speed) {
		return 1;
	}
	else {
		return 0;
	}
}



void Game_Init(void)
{
	font = CP_Font_Load("./Assets/Exo2-Regular.ttf");
	init_player(&p1, CP_Vector_Set(100, 100), 100, CP_Color_Create(255, 0, 0, 255), TRUE, 10);
	init_player(&p2, CP_Vector_Set(100, CP_System_GetWindowHeight() - 100/2), 100, CP_Color_Create(0, 255, 0, 255), FALSE, 10);


}

void playerswitch(struct Player* p1, struct Player* p2, CP_Vector mouse) {
	if (CP_Input_MouseClicked()) {
		if (IsCircleClicked(p1->position.x, p1->position.y, p1->diameter, mouse.x, mouse.y)) {
			p1->control = TRUE;
			p2->control = FALSE;
		}
		else if (IsCircleClicked(p2->position.x, p2->position.y, p2->diameter, mouse.x, mouse.y)) {
			p2->control = TRUE;
			p1->control = FALSE;
		}
	}
}



void Game_Update(void)
{
	mouse_position = CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY());
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));

	drawPlayer(&p1);
	drawPlayer(&p2);
	playerswitch(&p1, &p2, mouse_position);


	if (velocity_col(&p1, &p2)) {
		if (p1.control == FALSE) {
			if (p2.direction.y == 0 || p2.direction.x == 0) {
				p1.rotation = p1.rotation + 180 >= 360 ? p1.rotation + 180 - 360 : p1.rotation + 180;
			}
			else {
				p1.rotation = p2.rotation;
			}
			p2.direction.x = p1.direction.x * 0.5;
			p2.direction.y = p1.direction.x * 0.5;
		}
		if (p2.control == FALSE) {
			if (p1.direction.y == 0 || p1.direction.x == 0) {
				p2.rotation = p2.rotation + 180 >= 360 ? p2.rotation + 180 - 360 : p2.rotation + 180;
			}
			else {
				p2.rotation = p1.rotation;
			}

			p1.direction.x = p2.direction.x * 0.5;
			p1.direction.y = p2.direction.y * 0.5;
		}
	}

	


	if (CP_Input_KeyDown(KEY_Q)) {
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}

	CP_Font_Set(font);
	CP_Settings_TextSize(20.0f);
	char string[100] = { 0 };
	sprintf_s(string, 100, "%lf %lf %lf %lf", p1.direction.x, p1.direction.y, mouse_position.x, mouse_position.y);
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	CP_Font_DrawText(string, 1500, 500);

}

void Game_Exit(void)
{
	CP_Font_Free(font);
}


