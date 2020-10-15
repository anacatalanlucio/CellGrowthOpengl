#include <windows.h>
#include <thread>
#include <iostream>
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "cellGrowth.h"


#define FPS 15 //trying 15 frames per second to be able to see what cells do

#define COLUMNS 50//trying 50 by 50 to be able to see what cells do
#define ROWS 50

using namespace std;

int cell[COLUMNS][ROWS] = { {0} };

int cancerStatus = 0;
int healthyStatus = 1;
int medicineStatus = 2;


void display();
void reshape(int,int);
void timer(int);

//counting cells around(x,y) cell
static int countCancer(int,int);
static int countHealthy(int,int);
static int countMedicine(int,int);

//checking status: cancer = 0, healthy = 1, medicine = 2
static int checkCancerStatus(int, int, int, int, int);
static int checkHealthyStatus(int, int);
static int checkMedicineStatus(int, int, int, int);


//initialization
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    initGrid(COLUMNS, ROWS);
}

void threadFunc() {
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("First OpenGL Project");
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(500, 500);

    thread t1(threadFunc);//t1 starts
    thread t2(threadFunc);//t2 starts
    thread t3(threadFunc);//t3 starts
   
    t1.join();//wait for t1 to finish 
    t2.join();//wait for t2 to finish
    t3.join();//wait for t3 to finish

    glutTimerFunc(0, timer, 0);
    init();
    glutMainLoop();
    return 0;
}



int index = 0;
void display()
{
    GLfloat red, green, blue;
    glClear(GL_COLOR_BUFFER_BIT);

    drawGrid();

    int cell = 0;
    for (int x = 0; x < COLUMNS; x++) {
        for (int y = 0; y < ROWS; y++) {
            //Initialize each pixel with an arbitry cancer(0)/healthy(1)/medicine value(2).
            cell = rand() % 3;

            if (cell == 0) {
                glColor3f(1.0f, 0.0f, 0.0f);//red cancer cell
                glRectd(x, y, x + 1, y + 1); //size of a cell  
                cancerStatus = true;
                //this represents one pixel 
                //(y)--------(y+1)
                // '           '
                // '           '
                // '           '
                //(x)--------(x+1)
            }
            if (cell == 1) {
                glColor3f(0.0f, 1.0f, 0.0f);//green healthy cell
                glRectd(x, y, x + 1, y + 1);
                healthyStatus = true;

            }
            if (cell == 2) {
                glColor3f(1.0f, 1.0f, 0.0f);//yellow medicine cell
                glRectd(x, y, x + 2, y + 2);
                medicineStatus = true;

            }
        }

        for (int x = 0; x < COLUMNS; x++) {
            for (int y = 0; y < ROWS; y++) {
               //count the number of cancer cells, healthy cells, medicine cells around each cell. 
               int cancerCells = countCancer(x, y);
               int healthyCells = countHealthy(x, y);
               int medicineCells = countMedicine(x, y);
               //what is the status of each cell?
               cancerStatus = checkCancerStatus(cancerStatus, x, y, healthyCells, medicineCells);
               healthyStatus = checkHealthyStatus(healthyStatus, cancerCells);
               medicineStatus = checkMedicineStatus(medicineStatus, x, y, medicineCells);

               //update color according to the rules.
               if (cancerStatus == true) {
                   red = 1.0;
                   green = 0.0;
                   blue = 0.0;
               }
               //if healthy status is true then color pixel green 
               else if (healthyStatus == true) {
                   red = 0.0f;
                   green = 1.0f;
                   blue = 0.0f;
               }
               //if medicine status is true then color pixel yellow
               else if (medicineStatus == true) {
                   red = 1.0f;
                   green = 1.0f;
                   blue = 0.0f;
               }



            }
        }
    }
    //testing when the cell moves outside of the screen area and timer
    //int index = 0;
    /*index++;
    if (index > 40)
    {
        index= 0;
    }*/

    glutSwapBuffers();

}


//screen
void reshape(int w, int h) {
    glViewport(0,0,(GLsizei)w,(GLsizei)h); //viewport starts recangle at 0,0 to w,h of the window
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); //no changes at the beginning 
    glOrtho(0.0, COLUMNS,0.0,ROWS,-1.0,1.0); //widow from (0,0) to x=40 y=40
    glMatrixMode(GL_MODELVIEW);
}


void timer(int) {

    glutPostRedisplay(); //calling the display function 
    glutTimerFunc(1000 / FPS, timer,0); 

}


static int countCancer(int x, int y) {
    int cancer = 0;
    for (int i = (x - 1); i < (x + 2); i++) {
        //bottom row cells
        if (cell[i][y - 1] == 0) {
            cancer++;
        }
        //top row cells 
        if (cell[i][y + 1] == 0) {
            cancer++;
        }
        //left cell
        if (cell[x - 1][y] == 0) {
            cancer++;
        }
        //right cell 
        if (cell[x + 1][y] == 0) {
            cancer++;
        }
    }
    return cancer;
}


// Count healthy status of individual cells around[x][y] cell:
static int countHealthy(int x, int y) {
    int healthy = 0;
    for (int i = (x - 1); i < (x + 2); i++) {
        //bottom row cells
        if (cell[i][y - 1] == 1) {
            healthy++;
        }
        //top row cells 
        if (cell[i][y + 1] == 1) {
            healthy++;
        }
        //left cell
        if (cell[x - 1][y] == 1) {
            healthy++;
        }
        //right cell 
        if (cell[x + 1][y] == 1) {
            healthy++;
        }
    }
    return healthy;
}

//Count medicine status of individual cells around [x][y] cell:
static int countMedicine(int x, int y) {
    int medicine = 0;
    for (int i = (x - 1); i < (x + 2); i++) {
        //bottom row cells
        if (cell[i][y - 1] == 2) {
            medicine++;
        }
        //top row cells 
        if (cell[i][y + 1] == 2) {
            medicine++;
        }
        //left cell
        if (cell[x - 1][y] == 2) {
            medicine++;
        }
        //right cell 
        if (cell[x + 1][y] == 2) {
            medicine++;
        }
    }
    return medicine;
}

int temp; 
//Check the status of each cell and update cells according to rules.
static int checkCancerStatus(int status, int x, int y, int healthy, int medicine) {
    //cancer cell surrounded by at least 6 of 8 medicine neighbours turns into healthy cell.
    if (status == 0) {
        if (medicine >= 6) {
            status = 1;
            //all the surrounding cells also become healthy cells
            for (int i = (x - 1); i < (x + 2); i++) {
                //bottom row cells
                if ((cell[i][y - 1] == 0)|| (cell[i][y - 1] == 1) || (cell[i][y - 1] == 2 )) {
                    cell[i][y - 1] == 2;
                    healthy++;
                }
                //top row cells 
                if ((cell[i][y + 1] == 0) || (cell[i][y + 1] == 1) || (cell[i][y + 1] == 2)) {
                    cell[i][y + 1] == 2;
                    healthy++;
                }
                //left cell
                if ((cell[x - 1][y] == 0) || (cell[x - 1][y] == 1) || (cell[x - 1][y] == 2) ){
                    cell[x - 1][y] == 2;
                    healthy++;
                }
                //right cell 
                if ((cell[x + 1][y] == 0) || (cell[x + 1][y] == 1) || (cell[x + 1][y] == 2)) {
                    cell[x + 1][y] == 2;
                    healthy++;
                }
            }
        }
        //If medicine cells are not enough to be absorbed by cancer cell
        else if (medicine < 6) {
            //cancer cell remains cancer cell
            status = 0;
            //and the medicine cells move radially outward by one cell --> colliding cells get swapped.
            for (int i = (x - 1); i < (x + 2); i++) {
                //bottom row cells
                if (cell[i][y - 1] == 2) {
                    //if it is the middle bottom cell (x)(y-1)
                    if (i == x) {
                        temp = cell[i][y - 1];
                        cell[i][y - 1] = cell[i][y - 2];
                        cell[i][y - 2] = temp;
                    }
                    //if it is left bottom cell (x-1)(y-1)
                    else if( i == (x - 1)){
                        temp = cell[i][y - 1];
                        cell[i][y - 1] = cell[i - 1][y - 2];
                        cell[i - 1][y - 1] = temp;
                        
                    }
                    //if it is right bottom cell (x+1)(y-1)
                    else if (i == (x + 1)) {
                        temp = cell[i][y - 1];
                        cell[i][y - 1] = cell[i + 1][y + 2];
                        cell[i + 1][y + 1] = temp;
                    }
                }
                //top row cells 
                if (cell[i][y + 1] == 2) {
                    //if it is the top middle cell (x)(y+1)
                    if (i == x) {
                        temp = cell[i][y + 1];
                        cell[i][y + 1] = cell[i][y + 2];
                        cell[i][y + 2] = temp;
                    }
                    //if it is left top cell (x-1)(y+1)
                    else if (i == (x - 1)) {
                        temp = cell[i][y + 1];
                        cell[i][y + 1] = cell[i - 1][y + 2];
                        cell[i - 1][y + 2] = temp;

                    }
                    //if it is right top cell (x+1)(y+1)
                    else if (i == (x + 1)) {
                        temp = cell[i][y + 1];
                        cell[i][y + 1] = cell[i + 1][y + 2];
                        cell[i + 1][y + 2] = temp;
                    }

                }
                //left cell (x-1)(y)
                if (cell[x - 1][y] == 2) {
                    temp = cell[x - 2][y];
                    cell[x - 2][y] = cell[x - 1][y];
                    cell[x - 1][y] = temp;
                }
                //right cell (x+1)(y)
                if (cell[x + 1][y] == 2) {
                    temp = cell[x - 2][y];
                    cell[x - 2][y] = cell[x + 1][y];
                    cell[x + 1][y] = temp;
                }
            }

        }
        //otherwise cancer cell remains cancer cell
        else {
            status = 0;
        }
    }
    return status;
}

static int checkHealthyStatus(int status, int cancer) {
    //healthy cell surrounded by at least 6 of 8 cancer neighbours turns into cancer cell.
    if (status == 1) {
        if (cancer >= 6) {
            status = 0;
        }
        //healthy cell surrounded by healthy remains healthy
        else {
            status = 1;
        }
    }
    return status;
}

static int checkMedicineStatus(int status, int cancer, int x, int y) {
    if (status == 2) {
        //medicine cell surrounded by at least 6 of 8 cancer neighbours turns into cancer cell???
        if(cancer >= 6){
            status = 0;
        }
    }
    //otherwise medicine remains medicine
    else {
        status = 2;
    }
    return status;
}