#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include <math.h>


//define a type called rectangle with given properties
typedef struct Rectangle {
	CP_Vector position;
	CP_Vector scale;
	CP_Color color;
	float lerp;
	CP_BOOL exit_activation;
};

//declare 2 rectangle with the type Rectange
struct Rectangle rect1;
struct Rectangle rect2;



//declare window size
CP_Vector window_size;

CP_Vector mouse_position;

CP_Font font;

float alpha;

float exit_timer;




void Main_Menu_Exit(void)
{
	CP_Font_Free(font);
}



void exit_menu(void) {
	CP_Engine_Terminate();
}

void nextState(void) {
	CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit);
}

//draw a rectangle and check if its being clicked
void rect_button(struct Rectangle* rect, CP_Vector mouse_position, void (*func)(void), const char* text, float* alpha) {
	if (rect->exit_activation == TRUE) {
		exit_timer += CP_System_GetDt();
		*alpha -= (*alpha) * (1 - exp(-CP_System_GetDt() * 5));
		if (exit_timer >= 0.5) {
			func();
		}
	}

	if (IsAreaClicked(rect->position.x, rect->position.y, rect->scale.x, rect->scale.y, mouse_position.x, mouse_position.y)) {
		rect->lerp += (1 - rect->lerp) * (1 - exp(-CP_System_GetDt() * 5));
		if (rect->lerp >= 1) {
			rect->lerp = 1;
		}
	}
	else {
		rect->lerp -= (1 - rect->lerp) * (1 - exp(-CP_System_GetDt() * 7));
		if (rect->lerp < 0) {
			rect->lerp = 0;
		}
	}

	CP_Vector draw_scale = CP_Vector_Set((CP_Math_LerpFloat(rect->scale.x, rect->scale.x * 1.2, rect->lerp)),
										(CP_Math_LerpFloat(rect->scale.y, rect->scale.y * 1.2, rect->lerp)));

	rect->color = CP_Color_Create(255, 0, 0, *alpha);
	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, *alpha));
	CP_Settings_Fill(rect->color);
	CP_Graphics_DrawRect(rect->position.x, rect->position.y, draw_scale.x, draw_scale.y);


	CP_Settings_Fill(CP_Color_Create(255, 255, 255, *alpha));
	CP_Font_Set(font);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_TextSize(CP_Math_LerpInt(50, 75, rect->lerp));
	CP_Font_DrawText(text, rect->position.x, rect->position.y);


	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(rect->position.x, rect->position.y, rect->scale.x, rect->scale.y, mouse_position.x, mouse_position.y)) {
			rect->exit_activation = TRUE;
		}
	}

}


void Main_Menu_Init(void)
{
	//get window size
	window_size = CP_Vector_Set(CP_System_GetWindowWidth(), CP_System_GetWindowHeight());
	exit_timer = 0;

	alpha = 255;

	//set position, width and height for rectangle 1
	rect1.position = CP_Vector_Set(window_size.x / 2, window_size.y / 2);
	rect1.scale = CP_Vector_Set(window_size.x / 8, window_size.y / 8);
	rect1.lerp = 0;
	rect1.exit_activation = FALSE;

	//set position, width and height for rectangle 2
	rect2.position = CP_Vector_Set(window_size.x / 2, window_size.y / 2 + 200);
	rect2.scale = CP_Vector_Set(window_size.x / 8, window_size.y / 8);
	rect2.lerp = 0;
	rect2.exit_activation = FALSE;
	font = CP_Font_Load("Assets/Exo2-Regular.ttf");

}

void Main_Menu_Update(void)
{
	mouse_position = CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY());

	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_RectMode(CP_POSITION_CENTER);


	rect_button(&rect1, mouse_position, nextState, "START", &alpha);
	rect_button(&rect2, mouse_position, exit_menu, "EXIT", &alpha);
}

