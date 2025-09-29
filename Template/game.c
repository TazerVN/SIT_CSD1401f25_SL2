#include "cprocessing.h"
#include "mainmenu.h"

typedef struct Player {
	CP_Vector position;
	CP_Vector direction;
	int diamter;
	CP_Color color;
	CP_BOOL control;
};


struct Player p1;
struct Player p2;


void Game_Init(void)
{


}


void Game_Update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	CP_Graphics_DrawCircle(100, 100, 100);

	if (CP_Input_KeyDown(KEY_Q)) {
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}
}

void Game_Exit(void)
{

}