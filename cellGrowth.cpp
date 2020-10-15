#include <windows.h>
#include <thread>
#include <iostream>
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "cellGrowth.h"


int gridX, gridY;

void initGrid(int x, int y) {

	gridX = x;
	gridY = y;
}


void units(int, int);

//the whole grid 
void drawGrid() {
	

	int index = 0;
	//units(20, 20);

	//draws gid of the size of the screen
	for (int x = 0; x < gridX; x++) {
		for (int y = 0; y < gridY; y++) {
			units(x, y);
		}
	}

}


//square unit making the gRid
void units(int x, int y) {

	glLineWidth(1.0);
	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_LINE_LOOP);
	glVertex2f(x, y);
	glVertex2f(x + 1, y);
	glVertex2f(x + 1, y + 1);
	glVertex2f(x, y + 1);

	//this is one unit 
	//(x,y+1)------(x+1, y+1)
	// '                  '
	// '                  '
	// '                  '
	//(x ,y )--------(x+1, y)

	glEnd(); //finish drawing grid
}
