#include <math.h>
#include "cprocessing.h"

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y)
{
	if ((click_x >= area_center_x - area_width/2 && click_x <= area_center_x + area_width/2) &&
		click_y >= area_center_y - area_height/2 && click_y <= area_center_y + area_height/2) {
		return 1;
	}
	else {
		return 0;
	}
}


int squared(int x) {
	return x * x;
}


int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y)
{
	if (sqrt(squared(circle_center_x - click_x) + squared(circle_center_y- click_y)) <= diameter / 2) {
		return 1;
	}
	else {
		return 0;
	}
}

