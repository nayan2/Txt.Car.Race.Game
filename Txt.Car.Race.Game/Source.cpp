#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
using namespace std;

#define CTRL_COUNT 100  //a constant for bezier curve


int ctrlPointsCount;
double ctrlPointsX[CTRL_COUNT], ctrlPointsY[CTRL_COUNT];
double X1[3] = { 0.2, 0.299248120300752, 00.398496240601504 }, Y1[3] = { 0.475, 0.5, 0.475 };

///point for b first part(writing bus stop)
double x13[3] = { -0.654135338345865, -0.554887218045113, -0.654135338345865 }, y13[3] = { 0.475, 0.3625, 0.25 };
/////point for b second part(writing bus stop)
double x19[3] = { -0.654135338345865, -0.542857142857143, -0.654135338345865 }, y19[3] = { 0.25, 0.0875, 0.003125 };
///point for u(writing bus stop)
double x14[3] = { -0.494736842105263, -0.46015037593985, -0.403007518796992 }, y14[3] = { 0.346875, -0.028125, 0.346875 };
///point for 1s(writing bus stop)
double x15[4] = { -0.275187969924812, -0.338345864661654, -0.275187969924812, -0.35187969924812 }, y15[4] = { 0.328125, 0.265625, 0.053125, 0.009375 };
///point for 2s(writing bus stop)
double x16[4] = { -0.0827067669172933, -0.156390977443609, -0.075187969924812, -0.156390977443609 }, y16[4] = { 0.321875, 0.271875, 0.06875, 0.003125 };
//point for t(writing bus stop)
double x17[3] = { -0.0225563909774437, -0.0225563909774437, 0.0255639097744361 }, y17[3] = { 0.46875, 0.003125, -0.015625 };
//point for p(writing bus stop)
double x18[3] = { 0.231578947368421, 0.338345864661654, 0.231578947368421 }, y18[3] = { 0.359375, 0.1875, 0.06875 };
//point for truck curve line(drawing road truck)
double x20[3] = { 0.159398496240602, 0.189473684210526, 0.218045112781955 }, y20[3] = { -0.78125, -0.74375, -0.74375 };
//point for truck curve line(drawing road truck)
double x21[3] = { 0.218045112781955, 0.221052631578947, 0.246616541353383 }, y21[3] = { -0.74375, -0.678125, -0.625 };
//Initializes 3D rendering
void initRendering() {

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
}
float getNextBezierPointX(float t)
{
	float x = 0.0;

	for (int i = 0; i<ctrlPointsCount; i++)
	{
		int c;
		if (i == 0 || i == ctrlPointsCount - 1)
			c = 1;
		else
		{
			c = ctrlPointsCount - 1;
		}
		x += c * pow(t, i) * pow(1 - t, ctrlPointsCount - 1 - i) * ctrlPointsX[i];
	}


	return x;
}
float getNextBezierPointY(float t)
{
	float y = 0.0;

	for (int i = 0; i<ctrlPointsCount; i++)
	{
		int c;
		if (i == 0 || i == ctrlPointsCount - 1)
			c = 1;
		else
		{
			c = ctrlPointsCount - 1;
		}
		y += c * pow(t, i) * pow(1 - t, ctrlPointsCount - 1 - i) * ctrlPointsY[i];
	}



	return y;
}
void drawline(double xx, double yy, double z)
{
	// draw control points using red color
	for (int i = 0; i < 3; i++)
	{
		glBegin(GL_POINTS);
		glVertex2i(ctrlPointsX[i], ctrlPointsY[i]);
		glEnd();
		glFlush();
	}
	// draw bezier curve using control poitns by calculating next points using cubic bezier curve formula
	float oldX = ctrlPointsX[0]
		, oldY = ctrlPointsY[0];
	for (double t = 0.0; t <= 1.0; t += 0.01) {

		float x = getNextBezierPointX(t);
		float y = getNextBezierPointY(t);
		//glColor3f(1.0,t,1.0);
		glColor3f(xx, yy, z);
		//glPolygonMode(GL_POLYGON_OFFSET_LINE,GL_FILL);
		glBegin(GL_LINE_LOOP);
		glVertex2f(oldX, oldY);
		glVertex2f(x, y);
		glEnd();
		glFlush();

		oldX = x;
		oldY = y;
	}
} 
void writeBitmapString(void *font, char *string)
{
	char *c;

	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

//Called when the window is resized
void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

float zoom = -40.0;
float _angle = 0.0;
float _cameraAngle = 0.0;
float _ang_tri = 0.0; 
float move_right_cloud = 0.0; //variable for moving 1st cloud(for do translation)
float move_left_cloud = 0.0; //variable for moving 2nd cloud(for do translation)
float move_1st_bus = 0.00; //variable for moving 1st bus(for do translation)
float move_2nd_bus = 0.00; //variable for moving 2nd bus(for do translation)
float move_truck = 0.0; //variable for moving the only truck(for do translation)
float move_car = 0.0; //variable for moving car(for do translation)
int day_night = 0; //variable for changing day night effect
int rain = 0; //variable for changing rain effect
float move_rain = 0.0; //variable from moving rain along y exis

static char theStringBuffer[10]; // String buffer.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.


//func for drawing a polygon (for 4 vertex)
void drawpoly(double a, double b, double c, double d, double e, double f, double g, double h, double x, double y, double z) //draw the road middle break point
{
	glPushMatrix();
	glColor3f(x, y, z); //set color to black

	////glLineWidth(2.0);
	glBegin(GL_POLYGON);
	glVertex3f(a, b, 0);
	glVertex3f(c, d, 0);
	glVertex3f(e, f, 0);
	glVertex3f(g, h, 0);
	glEnd();
	glPopMatrix();
}
//func for draw road breaker
void draw_road_breaker()
{
	double x1 = -0.807518796992481;//variable for x
	double x2 = -0.807518796992481;//variable for x
	double x3 = -0.896240601503759;//variable for x
	double x4 = -0.896240601503759;//variable for x

	for (int i = 0; i < 8; i++) //for loop for 8 road breaker
	{
		glPushMatrix();
		glColor3f(1, 1, 1);
		glBegin(GL_POLYGON);
		glVertex3f(x1, -0.21875, 0); //y is contant
		glVertex3f(x2, -0.1875, 0);//y is contant
		glVertex3f(x3, -0.1875, 0);//y is contant
		glVertex3f(x4, -0.21875, 0);//y is contant
		glEnd();
		glPopMatrix();
		x1 = x1 + 0.232; //add a constant with x1(for gretting next point)(change in every for loop)
		x2 = x2 + 0.232;//add a constant with x2(for gretting next point)(change in every for loop)
		x3 = x3 + 0.232;//add a constant with x2(for gretting next point)(change in every for loop)
		x4 = x4 + 0.232;//add a constant with x4(for gretting next point)(change in every for loop)
	}
}
//func for drawing a building
//x,y,z for translation
//a,b,c for scalling 
void draw_building(double x, double y, double z, double a, double b, double c)
{
	glPushMatrix();
	glScalef(a, b, c);

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(-0.861654135338346, 0.44375, 0);
	glVertex3f(-0.861654135338346, 0.609375, 0);
	glVertex3f(-0.998496240601504, 0.609375, 0);
	glVertex3f(-0.998496240601504, 0.44375, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(-0.861654135338346, 0.1875, 0);
	glVertex3f(-0.861654135338346, 0.325, 0);
	glVertex3f(-0.998496240601504, 0.325, 0);
	glVertex3f(-0.998496240601504, 0.1875, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(1.000, 0.271, 0.000);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.861654135338346, 0.05, 0);
	glVertex3f(-0.861654135338346, 0.68125, 0);
	glVertex3f(-0.998496240601504, 0.68125, 0);
	glVertex3f(-0.998496240601504, 0.05, 0);
	glEnd();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(1.000, 0.271, 0.000);
	glBegin(GL_POLYGON);
	glVertex3f(-0.861654135338346, 0.05, 0);
	glVertex3f(-0.861654135338346, 0.68125, 0);
	glVertex3f(-0.998496240601504, 0.68125, 0);
	glVertex3f(-0.998496240601504, 0.05, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(1.000, 0.271, 0.000);
	glBegin(GL_LINES);
	glVertex3f(-0.861654135338346, 0.68125, 0);
	glVertex3f(-0.818045112781955, 0.76875, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(1.000, 0.271, 0.000);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.818045112781955, 0.05, 0);
	glVertex3f(-0.818045112781955, 0.76875, 0);
	glVertex3f(-0.94, 0.76875, 0);
	glVertex3f(-0.998496240601504, 0.68125, 0);
	glVertex3f(-0.998496240601504, 0.05, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(0.902, 0.902, 0.980);
	glBegin(GL_POLYGON);
	glVertex3f(-0.818045112781955, 0.05, 0);
	glVertex3f(-0.818045112781955, 0.76875, 0);
	glVertex3f(-0.94, 0.76875, 0);
	glVertex3f(-0.998496240601504, 0.68125, 0);
	glVertex3f(-0.998496240601504, 0.05, 0);
	glEnd();
	glPopMatrix();


	glPopMatrix();
}
//func for drawing only building which is larger than others
//x,y,z for translation
//a,b,c for scalling 
void draw_large_building(double x, double y, double z, double a, double b, double c)
{
	glPushMatrix();
	glScalef(a, b, c);

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(0.792481203007519, 0.04375, 0);
	glVertex3f(0.792481203007519, 0.128125, 0);
	glVertex3f(0.593984962406015, 0.128125, 0);
	glVertex3f(0.593984962406015, 0.04375, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(0.792481203007519, 0.596875, 0);
	glVertex3f(0.792481203007519, 0.753125, 0);
	glVertex3f(0.593984962406015, 0.753125, 0);
	glVertex3f(0.593984962406015, 0.596875, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(0.792481203007519, 0.275, 0);
	glVertex3f(0.792481203007519, 0.44375, 0);
	glVertex3f(0.593984962406015, 0.44375, 0);
	glVertex3f(0.593984962406015, 0.275, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.792481203007519, 0.04375, 0);
	glVertex3f(0.792481203007519, 0.815625, 0);
	glVertex3f(0.593984962406015, 0.815625, 0);
	glVertex3f(0.593984962406015, 0.04375, 0);
	glEnd();
	glPopMatrix();



	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(0.294, 0.000, 0.510);
	glBegin(GL_POLYGON);
	glVertex3f(0.792481203007519, 0.04375, 0);
	glVertex3f(0.792481203007519, 0.815625, 0);
	glVertex3f(0.593984962406015, 0.815625, 0);
	glVertex3f(0.593984962406015, 0.04375, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex3f(0.792481203007519, 0.815625, 0);
	glVertex3f(0.852631578947368, 0.971875, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.852631578947368, 0.04375, 0);
	glVertex3f(0.852631578947368, 0.971875, 0);
	glVertex3f(0.639097744360902, 0.971875, 0);
	glVertex3f(0.593984962406015, 0.815625, 0);
	glVertex3f(0.593984962406015, 0.04375, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(0.294, 0.000, 0.510);
	glBegin(GL_POLYGON);
	glVertex3f(0.852631578947368, 0.04375, 0);
	glVertex3f(0.852631578947368, 0.971875, 0);
	glVertex3f(0.639097744360902, 0.971875, 0);
	glVertex3f(0.593984962406015, 0.815625, 0);
	glVertex3f(0.593984962406015, 0.04375, 0);
	glEnd();
	glPopMatrix();


	glPopMatrix();
}
//fucn for drawing foothpath which
void draw_foothpath()
{
	int c = 0; //inisilizing a variable with valure o(for changing color)
	double x1 = -0.673684210526316; //a variable for store x1 value()
	double x2 = -0.673684210526316;//a variable for store x2 value
	double x3 = -0.833082706766917;//a variable for store x3 value
	double x4 = -0.833082706766917;//a variable for store x4 value
	for (int i = 0; i < 15; i++) //for loop for drawing 15 block of foothpath
	{
		glPushMatrix();
		if (c == 0)
		{
			glColor3f(0.7, 0.7, 0.7);  //changing color
			c = 1; //change variable value
		}
		else
		{
			glColor3f(0.5, 0.28, 0.28);  //changing color
			c = 0; //change variable value
		}
		glBegin(GL_POLYGON);
		glVertex3f(x1, -0.575, 0);
		glVertex3f(x2, -0.44375, 0);
		glVertex3f(x3, -0.44375, 0);
		glVertex3f(x4, -0.575, 0);
		glEnd();
		glPopMatrix();
		x3 = x2;  //inisialize x2 value in x3(for starting new a block with the last point of previouse block)
		x4 = x1; //inisialize x1 value in x4(for starting new a block with the last point of previouse block)
		x1 = x1 + 0.15; //increasing x1 value
		x2 = x2 + 0.15; //increasing x2 value
	}
}

//func for drawing red withte combination (like light but not)
//x,y,z for translation
void helping_hand(double x, double y, double z)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);
	glVertex3f(-0.789473684210526, -0.453125, 0);
	glVertex3f(-0.789473684210526, -0.36875, 0);
	glVertex3f(-0.798496240601504, -0.36875, 0);
	glVertex3f(-0.798496240601504, -0.453125, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glVertex3f(-0.798496240601504, -0.453125, 0);
	glVertex3f(-0.798496240601504, -0.36875, 0);
	glVertex3f(-0.806015037593985, -0.36875, 0);
	glVertex3f(-0.806015037593985, -0.453125, 0);
	glEnd();
	glPopMatrix();
}

void draw_km_sign(double x, double y, double z)
{
	//draw 5 sign
	glPushMatrix();
	glTranslatef(0.001, y, z);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(-0.806015037593985, 0.09375, 0);
	glVertex3f(-0.771428571428571, 0.09375, 0);

	glVertex3f(-0.771428571428571, 0.09375, 0);
	glVertex3f(-0.771428571428571, 0.134375, 0);

	glVertex3f(-0.771428571428571, 0.134375, 0);
	glVertex3f(-0.806015037593985, 0.134375, 0);

	glVertex3f(-0.806015037593985, 0.134375, 0);
	glVertex3f(-0.806015037593985, 0.165625, 0);

	glVertex3f(-0.806015037593985, 0.165625, 0);
	glVertex3f(-0.771428571428571, 0.165625, 0);
	glEnd();
	glPopMatrix();

	//draw k sign
	glPushMatrix();
	glTranslatef(0.001, y, z);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(-0.76390977443609, 0.09375, 0);
	glVertex3f(-0.76390977443609, 0.165625, 0);

	glVertex3f(-0.742857142857143, 0.09375, 0);
	glVertex3f(-0.76390977443609, 0.13125, 0);

	glVertex3f(-0.76390977443609, 0.13125, 0);
	glVertex3f(-0.742857142857143, 0.146875, 0);

	glEnd();
	glPopMatrix();

	//draw m sign
	glPushMatrix();
	glTranslatef(0.001, y, z);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(-0.735338345864662, 0.09375, 0);
	glVertex3f(-0.735338345864662, 0.165625, 0);

	glVertex3f(-0.735338345864662, 0.165625, 0);
	glVertex3f(-0.720300751879699, 0.103125, 0);

	glVertex3f(-0.720300751879699, 0.103125, 0);
	glVertex3f(-0.708270676691729, 0.165625, 0);

	glVertex3f(-0.708270676691729, 0.165625, 0);
	glVertex3f(-0.708270676691729, 0.09375, 0);

	glEnd();
	glPopMatrix();

	//board design(where (5km) will be appear)
	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3d(255, 255, 0);
	glBegin(GL_POLYGON);
	glVertex3f(-0.684210526315789, 0.065625, 0);
	glVertex3f(-0.684210526315789, 0.175, 0);
	glVertex3f(-0.790977443609023, 0.175, 0);
	glVertex3f(-0.790977443609023, 0.065625, 0);
	glEnd();
	glPopMatrix();
}

//func for drawing only dustbin
//x,y,z for translation
//a,b,c for scalling
void dustbin(double x, double y, double z, double a, double b, double c)
{
	glPushMatrix();
	glScalef(a, b, c);
	glPushMatrix();
	glTranslatef(x, y, z);
	glLineWidth(2.0);
	//drawing a berier curve
	ctrlPointsCount = 3;
	for (int i = 0; i<3; i++)
	{
		ctrlPointsX[i] = X1[i];
		ctrlPointsY[i] = Y1[i];
	}
	//calling the drawline func with passing color paramater
	drawline(0.180, 0.545, 0.341);
	glPopMatrix();

	//drawing multiple lines up to the dustbin
	double x1 = 0.21203007518797;
	double x2 = 0.21203007518797;
	for (int i = 1; i < 8; i++)
	{
		glPushMatrix();
		glTranslatef(x, y, z);
		glColor3f(0, 0, 0);
		glLineWidth(4.0);
		glBegin(GL_LINES);
		glVertex3f(x1, 0.090625, 0);
		glVertex3f(x2, 0.428125, 0);
		glEnd();
		glPopMatrix();
		x1 = x1 + 0.03;
		x2 = x2 + 0.03;
	}

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(0.180, 0.545, 0.341);
	glBegin(GL_POLYGON);
	glVertex3f(0.398496240601504, 0.04375, 0);
	glVertex3f(0.398496240601504, 0.090625, 0);
	glVertex3f(0.2, 0.090625, 0);
	glVertex3f(0.2, 0.04375, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(0.180, 0.545, 0.341);
	glBegin(GL_POLYGON);
	glVertex3f(0.398496240601504, 0.428125, 0);
	glVertex3f(0.398496240601504, 0.475, 0);
	glVertex3f(0.2, 0.475, 0);
	glVertex3f(0.2, 0.428125, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(0.180, 0.545, 0.341);
	glBegin(GL_POLYGON);
	glVertex3f(0.392481203007519, 0.090625, 0);
	glVertex3f(0.392481203007519, 0.428125, 0);
	glVertex3f(0.207518796992481, 0.428125, 0);
	glVertex3f(0.207518796992481, 0.090625, 0);
	glEnd();
	glPopMatrix();
	glPopMatrix();

}
//func for drawing a bus stop board
//x,y,z for translation 
//a,b,c for scaling
void draw_bus_stop_board(double x, double y, double z, double a, double b, double c)
{
	glPushMatrix();
	glScalef(a, b, c);
	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(1,1,0);
	glBegin(GL_POLYGON);
	glVertex3f(0.491729323308271, 0.05, 0);
	glVertex3f(0.491729323308271, 0.525, 0);
	glVertex3f(0.413533834586466, 0.525, 0);
	glVertex3f(0.413533834586466, 0.05, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(0.180, 0.545, 0.341);
	glBegin(GL_POLYGON);
	glVertex3f(0.58796992481203, 0.525, 0);
	glVertex3f(0.58796992481203, 0.79, 0);
	glVertex3f(0.330827067669173, 0.79, 0);
	glVertex3f(0.330827067669173, 0.525, 0);
	glEnd();
	glPopMatrix();
	glPopMatrix();
}
//func for the bus which is inside the bus stop board
//x,y,z for translation
//a,b,c scalling
void draw_bus_stop_board_bus(double x, double y, double z, double a, double b, double c)
{
	glPushMatrix();
	glScalef(a, b, c);
	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3d(0, 128, 128);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.566917293233083, 0.603125, 0);
	glVertex3f(0.566917293233083, 0.65625, 0);
	glVertex3f(0.557894736842105, 0.71, 0);
	glVertex3f(0.360902255639098, 0.71, 0);
	glVertex3f(0.360902255639098, 0.603125, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(0, 1, 0);
	glBegin(GL_POLYGON);
	glVertex3f(0.384962406015038, 0.65625, 0);
	glVertex3f(0.384962406015038, 0.69375, 0);
	glVertex3f(0.366917293233083, 0.69375, 0);
	glVertex3f(0.366917293233083, 0.65625, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(0, 1, 0);
	glBegin(GL_POLYGON);
	glVertex3f(0.416541353383459, 0.65625, 0);
	glVertex3f(0.416541353383459, 0.69375, 0);
	glVertex3f(0.38796992481203, 0.69375, 0);
	glVertex3f(0.38796992481203, 0.65625, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(0, 1, 0);
	glBegin(GL_POLYGON);
	glVertex3f(0.451127819548872, 0.65625, 0);
	glVertex3f(0.451127819548872, 0.69375, 0);
	glVertex3f(0.421052631578947, 0.69375, 0);
	glVertex3f(0.421052631578947, 0.65625, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	glVertex3f(0.470676691729323, 0.615625, 0);
	glVertex3f(0.470676691729323, 0.69375, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(0, 1, 0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.484210526315789, 0.615625, 0);
	glVertex3f(0.484210526315789, 0.69375, 0);
	glVertex3f(0.457142857142857, 0.69375, 0);
	glVertex3f(0.457142857142857, 0.615625, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(0, 1, 0);
	glBegin(GL_POLYGON);
	glVertex3f(0.518796992481203, 0.65625, 0);
	glVertex3f(0.518796992481203, 0.69375, 0);
	glVertex3f(0.490225563909774, 0.69375, 0);
	glVertex3f(0.490225563909774, 0.65625, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(0, 1, 0);
	glBegin(GL_POLYGON);
	glVertex3f(0.559398496240601, 0.65625, 0);
	glVertex3f(0.556390977443609, 0.69375, 0);
	glVertex3f(0.524812030075188, 0.69375, 0);
	glVertex3f(0.524812030075188, 0.65625, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0.57, 0.608, 0.0);
	glutSolidSphere(0.02, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0.44, 0.61, 0.0);
	glutSolidSphere(0.02, 30, 30);
	glPopMatrix();
	glPopMatrix();
}
//func for write (bus stop) into the bus stop board
void write_bus_stop(double x, double y, double z, double a, double b, double c)
{

	glPushMatrix();
	glScalef(a, b, c);
	//B design
	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex3f(-0.654135338345865, 0.475, 0);
	glVertex3f(-0.654135338345865, 0.003125, 0);
	glEnd();
	ctrlPointsCount = 3;
	for (int i = 0; i<3; i++)
	{
		ctrlPointsX[i] = x13[i];
		ctrlPointsY[i] = y13[i];
	}
	drawline(1, 1, 1);
	ctrlPointsCount = 3;
	for (int i = 0; i<3; i++)
	{
		ctrlPointsX[i] = x19[i];
		ctrlPointsY[i] = y19[i];
	}
	drawline(1, 1, 1);
	glPopMatrix();
	//u design
	glPushMatrix();
	glTranslatef(x, y, z);
	ctrlPointsCount = 3;
	for (int i = 0; i<3; i++)
	{
		ctrlPointsX[i] = x14[i];
		ctrlPointsY[i] = y14[i];
	}
	drawline(1, 1, 1);
	glPopMatrix();
	//1s design
	glPushMatrix();
	glTranslatef(x, y, z);
	ctrlPointsCount = 4;
	for (int i = 0; i<4; i++)
	{
		ctrlPointsX[i] = x15[i];
		ctrlPointsY[i] = y15[i];
	}
	drawline(1, 1, 1);
	glPopMatrix();
	//2s design
	glPushMatrix();
	glTranslatef(x, y, z);
	ctrlPointsCount = 4;
	for (int i = 0; i<4; i++)
	{
		ctrlPointsX[i] = x16[i];
		ctrlPointsY[i] = y16[i];
	}
	drawline(1, 1, 1);
	glPopMatrix();
	//t design
	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex3f(-0.0586466165413534, 0.334375, 0);
	glVertex3f(0.0285714285714286, 0.334375, 0);
	glEnd();
	ctrlPointsCount = 3;
	for (int i = 0; i<3; i++)
	{
		ctrlPointsX[i] = x17[i];
		ctrlPointsY[i] = y17[i];
	}
	drawline(1, 1, 1);
	glPopMatrix();
	//o design
	glPushMatrix();
	glTranslatef(x, y, z);
	glPushMatrix();
	glTranslatef(0.1, 0.1, z);
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i<200; i++)
	{
		float pi = 3.1416;
		float A = (i * 2 * pi) / 50;
		float r = 0.08;
		float x = r * cos(A);
		float y = r * sin(A);
		glVertex2f(x, y);
	}
	glEnd();
	glPopMatrix();
	glPopMatrix();
	//p design
	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex3f(0.231578947368421, 0.359375, 0);
	glVertex3f(0.231578947368421, -0.190625, 0);
	glEnd();
	ctrlPointsCount = 3;
	for (int i = 0; i<3; i++)
	{
		ctrlPointsX[i] = x18[i];
		ctrlPointsY[i] = y18[i];
	}
	drawline(1, 1, 1);
	glPopMatrix();
	glPopMatrix();
}
void draw_cloud(double x, double y, double z)
{
	glPushMatrix();
	glScalef(0.8, 0.7, 0);
	glTranslatef(x, y, z);
	glPushMatrix();
	glScalef(1, 1, 0);
	glTranslatef(-0.03, 0, 0);
	glColor3f(1, 1, 1);
	glutSolidSphere(.10, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, 0);
	glTranslatef(0.12, 0, 0);
	glColor3f(1, 1, 1);
	glutSolidSphere(.10, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, 0);
	glTranslatef(0.05, 0.05, 0);
	glColor3f(1, 1, 1);
	glutSolidSphere(.10, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, 0);
	glTranslatef(0.05, -0.05, 0);
	glColor3f(1, 1, 1);
	glutSolidSphere(.10, 30, 30);
	glPopMatrix();
	glPopMatrix();

}
//func for drawing passenger stand
//x,y,z translation
///a,b,c for scalling
void draw_passenger_stand(double x, double y, double z, double a, double b, double c)
{

	glPushMatrix();
	glScalef(a, b, c);
	glPushMatrix();
	glScalef(0.9, 1.0, 0);
	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(0.678, 1.000, 0.184);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.168421052631579, 0.046875, 0);
	glVertex3f(0.168421052631579, 0.675, 0);
	glVertex3f(-0.455639097744361, 0.675, 0);
	glVertex3f(-0.455639097744361, 0.046875, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0.144360902255639, 0.046875, 0);
	glVertex3f(0.144360902255639, 0.675, 0);

	glVertex3f(-0.434586466165414, 0.046875, 0);
	glVertex3f(-0.434586466165414, 0.675, 0);

	glVertex3f(0.181954887218045, 0.675, 0);
	glVertex3f(-0.469172932330827, 0.675, 0);

	glVertex3f(0.196992481203008, 0.70625, 0);
	glVertex3f(-0.478195488721805, 0.70625, 0);

	glVertex3f(0.181954887218045, 0.675, 0);
	glVertex3f(0.196992481203008, 0.70625, 0);

	glVertex3f(-0.469172932330827, 0.675, 0);
	glVertex3f(-0.478195488721805, 0.70625, 0);

	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(0.196992481203008, 0.70625, 0);
	glVertex3f(0.196992481203008, 0.74375, 0);
	glVertex3f(-0.478195488721805, 0.74375, 0);
	glVertex3f(-0.478195488721805, 0.70625, 0);
	glEnd();

	glColor3f(0.196, 0.804, 0.196);
	glBegin(GL_POLYGON);
	glVertex3f(0.196992481203008, 0.74375, 0);
	glVertex3f(0.196992481203008, 0.8125, 0);
	glVertex3f(-0.478195488721805, 0.8125, 0);
	glVertex3f(-0.478195488721805, 0.74375, 0);
	glEnd();

	glLineWidth(4.5);
	glBegin(GL_LINES);
	glVertex3f(-0.0706766917293233, 0.046875, 0);
	glVertex3f(-0.0706766917293233, 0.675, 0);

	glVertex3f(-0.299248120300752, 0.046875, 0);
	glVertex3f(-0.299248120300752, 0.675, 0);

	glEnd();

	glBegin(GL_LINE_LOOP);

	glVertex3f(0.144360902255639, 0.309375, 0);
	glVertex3f(0.144360902255639, 0.35625, 0);
	glVertex3f(-0.434586466165414, 0.35625, 0);
	glVertex3f(-0.434586466165414, 0.309375, 0);

	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(0.144360902255639, 0.09375, 0);
	glVertex3f(0.144360902255639, 0.1375, 0);
	glVertex3f(-0.434586466165414, 0.1375, 0);
	glVertex3f(-0.434586466165414, 0.09375, 0);
	glEnd();

	glLineWidth(2);

	glColor3f(0.000, 0.392, 0.000);
	glBegin(GL_POLYGON);
	glVertex3f(0.0796992481203008, 0.046875, 0);
	glVertex3f(0.0796992481203008, 0.265625, 0);
	glVertex3f(0.0526315789473684, 0.265625, 0);
	glVertex3f(0.0526315789473684, 0.046875, 0);
	glEnd();

	glColor3f(0.000, 0.392, 0.000);
	glBegin(GL_POLYGON);
	glVertex3f(0.0526315789473684, 0.23125, 0);
	glVertex3f(0.0526315789473684, 0.265625, 0);
	glVertex3f(-0.374436090225564, 0.265625, 0);
	glVertex3f(-0.374436090225564, 0.23125, 0);
	glEnd();

	glColor3f(0.000, 0.392, 0.000);
	glBegin(GL_POLYGON);
	glVertex3f(-0.374436090225564, 0.046875, 0);
	glVertex3f(-0.374436090225564, 0.265625, 0);
	glVertex3f(-0.392481203007519, 0.265625, 0);
	glVertex3f(-0.392481203007519, 0.046875, 0);
	glEnd();

	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glVertex3f(0.0796992481203008, 0.303125, 0);
	glVertex3f(0.0796992481203008, 0.35625, 0);
	glVertex3f(-0.390977443609023, 0.35625, 0);
	glVertex3f(-0.390977443609023, 0.303125, 0);
	glEnd();

	glColor3f(0.000, 0.392, 0.000);
	glBegin(GL_POLYGON);
	glVertex3f(0.0857142857142857, 0.284375, 0);
	glVertex3f(0.0857142857142857, 0.49375, 0);
	glVertex3f(-0.398496240601504, 0.49375, 0);
	glVertex3f(-0.398496240601504, 0.284375, 0);
	glEnd();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

}
//func for drawing lamp post
//xx,yy,zz for color value which is the down part of the lamp post
//x,y,z for translation
///a,b,c for scalling
//d,e,f for color which is apperar in night (yellow color)
void draw_lamppost(double xx, double yy, double zz, double x, double y, double z, double a, double b, double c, int d, int e, int f)
{
	///-0.2,1,1
	glPushMatrix();
	glColor3f(xx,yy,zz);
	glScalef(a, b, c);
	glTranslatef(x, y, z);

	glBegin(GL_POLYGON);
	glVertex3f(-0.43609022556391, -0.396875, 0);
	glVertex3f(-0.43609022556391, -0.340625, 0);
	glVertex3f(-0.493233082706767, -0.353125, 0);
	glVertex3f(-0.493233082706767, -0.396875, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-0.443609022556391, -0.353125, 0);
	glVertex3f(-0.443609022556391, -0.19375, 0);
	glVertex3f(-0.484210526315789, -0.209375, 0);
	glVertex3f(-0.484210526315789, -0.353125, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-0.452631578947368, -0.196875, 0);
	glVertex3f(-0.452631578947368, -0.071875, 0);
	glVertex3f(-0.478195488721805, -0.071875, 0);
	glVertex3f(-0.476691729323308, -0.2125, 0);
	glEnd();

	glPushMatrix();
	glColor3d(d,e,f);
	glBegin(GL_POLYGON);
	glVertex3f(-0.452631578947368, -0.090625, 0);
	glVertex3f(-0.243609022556391, -0.040625, 0);
	glVertex3f(-0.243609022556391, -0.01875, 0);
	glVertex3f(-0.457142857142857, -0.06875, 0);
	glEnd();
	glPopMatrix();

	glPopMatrix();

}
//func for drawing car
//x,y,z for translation
void draw_car(double x, double y, double z)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(0.6, 0.5, 0);
	glLineWidth(2.2);


	glPushMatrix();
	glColor3f(0.412, 0.412, 0.412);
	glTranslatef(0.325, -0.65, 0);
	glRotatef(_angle, 0.0, 0.0, 1.0);
	glutWireSphere(0.03, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.412, 0.412, 0.412);
	glTranslatef(0.00, -0.65, 0);
	glRotatef(_angle, 0.0, 0.0, 1.0);
	glutWireSphere(0.03, 30, 30);
	glPopMatrix();



	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0.325, -0.65, 0);
	glRotatef(_angle, 0.0, 0.0, 1.0);
	glutWireSphere(0.06, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0.00, -0.65, 0);
	glRotatef(_angle, 0.0, 0.0, 1.0);
	glutWireSphere(0.06, 30, 30);
	glPopMatrix();


	glPushMatrix();
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glVertex3f(0.239097744360902, -0.465625, 0);
	glVertex3f(0.234586466165414, -0.44375, 0);
	glVertex3f(0.227067669172932, -0.45, 0);
	glVertex3f(0.227067669172932, -0.4625, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glVertex3f(0.234586466165414, -0.44375, 0);
	glVertex3f(0.231578947368421, -0.415625, 0);
	glVertex3f(0.222556390977444, -0.41875, 0);
	glVertex3f(0.225563909774436, -0.44375, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex3f(0.433082706766917, -0.54375, 0);
	glVertex3f(0.440601503759398, -0.54375, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex3f(0.430075187969925, -0.528125, 0);
	glVertex3f(0.437593984962406, -0.528125, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.196, 0.804, 0.196);
	glBegin(GL_POLYGON);
	glVertex3f(0.442105263157895, -0.653125, 0);
	glVertex3f(0.442105263157895, -0.615625, 0);
	glVertex3f(0.428571428571429, -0.61875, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.196, 0.804, 0.196);
	glBegin(GL_POLYGON);
	glVertex3f(0.153383458646617, -0.5, 0);
	glVertex3f(0.153383458646617, -0.48125, 0);
	glVertex3f(0.129323308270677, -0.48125, 0);
	glVertex3f(0.129323308270677, -0.5, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.196, 0.804, 0.196);
	glBegin(GL_POLYGON);
	glVertex3f(0.0120300751879699, -0.4875, 0);
	glVertex3f(0.0120300751879699, -0.478125, 0);
	glVertex3f(-0.00601503759398492, -0.478125, 0);
	glVertex3f(-0.00601503759398492, -0.4875, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.196, 0.804, 0.196);
	glBegin(GL_POLYGON);
	glVertex3f(-0.0766917293233083, -0.503125, 0);
	glVertex3f(-0.0766917293233083, -0.475, 0);
	glVertex3f(-0.115789473684211, -0.475, 0);
	glVertex3f(-0.114285714285714, -0.503125, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.196, 0.804, 0.196);
	glBegin(GL_POLYGON);
	glVertex3f(-0.126315789473684, -0.55, 0);
	glVertex3f(-0.109774436090226, -0.534375, 0);
	glVertex3f(-0.127819548872181, -0.528125, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.196, 0.804, 0.196);
	glBegin(GL_POLYGON);
	glVertex3f(0.428571428571429, -0.540625, 0);
	glVertex3f(0.422556390977444, -0.50625, 0);
	glVertex3f(0.386466165413534, -0.496875, 0);
	glVertex3f(0.396992481203008, -0.534375, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.196, 0.804, 0.196);
	glBegin(GL_POLYGON);
	glVertex3f(0.4435, -0.565625, 0);
	glVertex3f(0.442105263157895, -0.534375, 0);
	glVertex3f(0.433082706766917, -0.515625, 0);
	glVertex3f(0.427067669172932, -0.515625, 0);
	glVertex3f(0.434586466165414, -0.5625, 0);
	glEnd();
	glPopMatrix();


	glPushMatrix();
	glColor3f(0.196, 0.804, 0.196);
	glBegin(GL_LINES);
	glVertex3f(0.276691729323308, -0.659375, 0);
	glVertex3f(0.0451127819548872, -0.659375, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.196, 0.804, 0.196);
	glBegin(GL_LINES);
	glVertex3f(0.439097744360902, -0.659375, 0);
	glVertex3f(0.381954887218045, -0.659375, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.196, 0.804, 0.196);
	glBegin(GL_LINES);
	glVertex3f(-0.114285714285714, -0.446875, 0);
	glVertex3f(0.258646616541353, -0.459375, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.196, 0.804, 0.196);
	glBegin(GL_LINES);
	glVertex3f(0.258646616541353, -0.459375, 0);
	glVertex3f(0.353383458646617, -0.46875, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.196, 0.804, 0.196);
	glBegin(GL_LINES);
	glVertex3f(0.353383458646617, -0.46875, 0);
	glVertex3f(0.413533834586466, -0.4875, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.196, 0.804, 0.196);
	glBegin(GL_LINES);
	glVertex3f(0.413533834586466, -0.4875, 0);
	glVertex3f(0.442105263157895, -0.540625, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.196, 0.804, 0.196);
	glBegin(GL_LINES);
	glVertex3f(0.442105263157895, -0.540625, 0);
	glVertex3f(0.439097744360902, -0.659375, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.196, 0.804, 0.196);
	glBegin(GL_LINES);
	glVertex3f(-0.114285714285714, -0.446875, 0);
	glVertex3f(-0.126315789473684, -0.503125, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.196, 0.804, 0.196);
	glBegin(GL_LINES);
	glVertex3f(-0.126315789473684, -0.503125, 0);
	glVertex3f(-0.126315789473684, -0.55, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.196, 0.804, 0.196);
	glBegin(GL_LINES);
	glVertex3f(-0.126315789473684, -0.55, 0);
	glVertex3f(-0.118796992481203, -0.6125, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(00.196, 0.804, 0.196);
	glBegin(GL_LINES);
	glVertex3f(-0.118796992481203, -0.6125, 0);
	glVertex3f(-0.0827067669172933, -0.646875, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.196, 0.804, 0.196);
	glBegin(GL_LINES);
	glVertex3f(-0.0827067669172933, -0.646875, 0);
	glVertex3f(-0.0556390977443609, -0.646875, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex3f(0.133834586466165, -0.625, 0);
	glVertex3f(0.120300751879699, -0.45, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.000, 0.000, 1.000);
	glBegin(GL_POLYGON);
	glVertex3f(0.261654135338346, -0.625, 0);
	glVertex3f(0.261654135338346, -0.459375, 0);
	glVertex3f(0.120300751879699, -0.45, 0);
	glVertex3f(0.133834586466165, -0.625, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.000, 0.000, 1.000);
	glBegin(GL_POLYGON);
	glVertex3f(0.133834586466165, -0.625, 0);
	glVertex3f(0.120300751879699, -0.45, 0);
	glVertex3f(-0.0150375939849624, -0.45, 0);
	glVertex3f(-0.00751879699248126, -0.54375, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex3f(0.120300751879699, -0.45, 0);
	glVertex3f(0.111278195488722, -0.346875, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.000, 0.000, 0.502);
	glBegin(GL_POLYGON);
	glVertex3f(0.261654135338346, -0.459375, 0);
	glVertex3f(0.157894736842105, -0.35, 0);
	glVertex3f(0.037593984962406, -0.353125, 0);
	glVertex3f(-0.0150375939849624, -0.45, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.196, 0.804, 0.196);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.276691729323308, -0.4625, 0);
	glVertex3f(0.172932330827068, -0.34375, 0);
	glVertex3f(0.141353383458647, -0.33125, 0);
	glVertex3f(0.0210526315789474, -0.33125, 0);
	glVertex3f(-0.0691729323308271, -0.45, 0);
	glEnd();
	glPopMatrix();

	glPopMatrix();
}
//func for drawing a bus
//x,y,z for translation
//a,b,c for bus body color
//d,e,f for bus door color
void draw_bus(double x, double y, double z, double a, double b, double c, double d, double e, double f)
{
	glPushMatrix();
	glScalef(1, 1, 0);
	glTranslatef(x, y, z);

	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(0.0075187969924812, -0.775, 0);
	glVertex3f(0.0075187969924812, -0.709375, 0);

	glVertex3f(-0.0691729323308271, -0.775, 0);
	glVertex3f(-0.0691729323308271, -0.709375, 0);

	glVertex3f(-0.142857142857143, -0.775, 0);
	glVertex3f(-0.142857142857143, -0.709375, 0);

	glEnd();

	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glVertex3f(0.0857142857142857, -0.775, 0);
	glVertex3f(0.0857142857142857, -0.709375, 0);
	glVertex3f(-0.22406015037594, -0.709375, 0);
	glVertex3f(-0.22406015037594, -0.775, 0);
	glEnd();

	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex3f(-0.267669172932331, -0.85, 0);
	glVertex3f(-0.267669172932331, -0.7125, 0);
	glEnd();

	//0.502, 0.0, 0.0
	glColor3f(d, e, f);
	glBegin(GL_POLYGON);
	glVertex3f(-0.239097744360902, -0.85, 0);
	glVertex3f(-0.239097744360902, -0.7125, 0);
	glVertex3f(-0.294736842105263, -0.7125, 0);
	glVertex3f(-0.294736842105263, -0.85, 0);
	glEnd();


	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.335338345864662, -0.778125, 0);
	glVertex3f(-0.303759398496241, -0.778125, 0);
	glVertex3f(-0.303759398496241, -0.7125, 0);
	glVertex3f(-0.31, -0.7125, 0);
	glEnd();

	glPushMatrix();
	glColor3f(0.412, 0.412, 0.412);
	glTranslatef(-0.2, -0.86, 0);
	glRotatef(_angle, 0.0, 0.0, 1.0);
	glutWireSphere(0.019, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.412, 0.412, 0.412);
	glTranslatef(-0.0, -0.86, 0);
	glRotatef(_angle, 0.0, 0.0, 1.0);
	glutWireSphere(0.019, 30, 30);
	glPopMatrix();


	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-0.2, -0.86, 0);
	glRotatef(_angle, 0.0, 0.0, 1.0);
	glutWireSphere(0.038, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-0.0, -0.86, 0);
	glRotatef(_angle, 0.0, 0.0, 1.0);
	glutWireSphere(0.038, 30, 30);
	glPopMatrix();

	glColor3f(a, b, c);
	glBegin(GL_POLYGON);
	glVertex3f(0.0992481203007519, -0.86875, 0);
	glVertex3f(0.0992481203007519, -0.69375, 0);
	glVertex3f(-0.303759398496241, -0.69375, 0);
	glVertex3f(-0.335338345864662, -0.778125, 0);
	glVertex3f(-0.335338345864662, -0.86875, 0);
	glEnd();

	glPopMatrix();
}

//a func for drawing a truck
//x,y,z for translation
void draw_truck(double x, double y, double z)
{
	glPushMatrix();
	glScalef(0.7, 0.6, 0);
	glTranslatef(x, y, z);

	glPushMatrix();
	glColor3f(0.412, 0.412, 0.412);
	glScalef(1.5, 1.9, 0);
	glTranslatef(0.17, -0.465, 0);
	glRotatef(_angle, 0.0, 0.0, 1.0);
	glutSolidSphere(0.02, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.412, 0.412, 0.412);
	glScalef(1.5, 1.9, 0);
	glTranslatef(0.4, -0.465, 0);
	glRotatef(_angle, 0.0, 0.0, 1.0);
	glutSolidSphere(0.02, 30, 30);
	glPopMatrix();


	glPushMatrix();
	glColor3f(0, 0, 0);
	glScalef(1.5, 1.9, 0);
	glTranslatef(0.17, -0.465, 0);
	glRotatef(_angle, 0.0, 0.0, 1.0);
	glutSolidSphere(0.04, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glScalef(1.5, 1.9, 0);
	glTranslatef(0.4, -0.465, 0);
	glRotatef(_angle, 0.0, 0.0, 1.0);
	glutSolidSphere(0.04, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glLineWidth(2.2);
	glBegin(GL_LINES);
	glVertex3f(0.694736842105263, -0.740625, 0);
	glVertex3f(0.369924812030075, -0.740625, 0);
	glEnd();
	glPopMatrix();

	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(0.694736842105263, -0.878125, 0);
	glVertex3f(0.369924812030075, -0.878125, 0);
	glEnd();

	glPushMatrix();
	glColor3f(0.0, 0.392, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.696240601503759, -0.9125, 0);
	glVertex3f(0.696240601503759, -0.6125, 0);
	glVertex3f(0.369924812030075, -0.6125, 0);
	glVertex3f(0.369924812030075, -0.9125, 0);
	glEnd();
	glPopMatrix();

	glColor3f(0.133, 0.545, 0.133);
	glBegin(GL_POLYGON);
	glVertex3f(0.353383458646617, -0.75, 0);
	glVertex3f(0.359398496240601, -0.6375, 0);
	glVertex3f(0.258646616541353, -0.6375, 0);
	glVertex3f(0.228571428571429, -0.75, 0);
	glEnd();

	glColor3f(0.133, 0.545, 0.133);
	glBegin(GL_LINES);
	glVertex3f(0.350375939849624, -0.784375, 0);
	glVertex3f(0.311278195488722, -0.784375, 0);
	glEnd();

	glPushMatrix();
	glLineWidth(2.0);
	glColor3f(0.133, 0.545, 0.133);
	glBegin(GL_LINES);
	glVertex3f(0.348872180451128, -0.765625, 0);
	glVertex3f(0.312781954887218, -0.765625, 0);
	glEnd();
	glPopMatrix();

	glColor3f(0.0, 0.392, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.369924812030075, -0.9125, 0);
	glVertex3f(0.145864661654135, -0.9125, 0);
	glEnd();

	glColor3f(0.0, 0.392, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.145864661654135, -0.9125, 0);
	glVertex3f(0.145864661654135, -0.875, 0);
	glEnd();

	glColor3f(0.0, 0.392, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.145864661654135, -0.875, 0);
	glVertex3f(0.156390977443609, -0.875, 0);
	glEnd();

	glColor3f(0.0, 0.392, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.369924812030075, -0.621875, 0);
	glVertex3f(0.246616541353383, -0.621875, 0);
	glEnd();

	glPushMatrix();
	glColor3f(0.0, 0.392, 0.0);
	glLineWidth(2.2);
	glBegin(GL_LINES);
	glVertex3f(0.156390977443609, -0.875, 0);
	glVertex3f(0.159398496240602, -0.78125, 0);
	glEnd();
	glPopMatrix();
     
	//drawing curve line
	glPushMatrix();
	ctrlPointsCount = 3;
	for (int i = 0; i<3; i++)
	{
		ctrlPointsX[i] = x20[i];
		ctrlPointsY[i] = y20[i];
	}
	drawline(0.0, 0.392, 0.0);
	glPopMatrix();

	glPushMatrix();
	//drawing curve line
	ctrlPointsCount = 3;
	for (int i = 0; i<3; i++)
	{
		ctrlPointsX[i] = x21[i];
		ctrlPointsY[i] = y21[i];
	}
	drawline(0.0, 0.392, 0.0);
	glPopMatrix();


	glPopMatrix();
}
//draw a flag of banglaesh
//x,y,z for translation
//a,b,c for scalling
void draw_flag(double x, double y, double z, double a, double b, double c)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(a, b, c);

	glPushMatrix();
	glColor3f(1.000, 1.000, 0.000);
	glBegin(GL_QUADS);
	glVertex3f(0.657142857142857, 0.025, 0);
	glVertex3f(0.657142857142857, 0.26875, 0);
	glVertex3f(0.64812030075188, 0.26875, 0);
	glVertex3f(0.64812030075188, 0.025, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(0.69, 0.23, 0);
	glutSolidSphere(0.025, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.000, 0.502, 0.000);
	glBegin(GL_POLYGON);
	glVertex3f(0.724812030075188, 0.178125, 0);
	glVertex3f(0.724812030075188, 0.271875, 0);
	glVertex3f(0.657142857142857, 0.271875, 0);
	glVertex3f(0.657142857142857, 0.178125, 0);
	glEnd();
	glPopMatrix();

	glPopMatrix();
}
//draw a tree using 5 solid sphare and 2 or 3 polygon
//x,y,z for translation
//a,b,c for scalling
void draw_tree(double x, double y, double z, double a, double b, double c)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(a, b, c);

	glPushMatrix();
	glColor3f(0, 1, 0);
	glScalef(1, 1, 0);
	glTranslatef(0.07, -0.0, 0);
	glutSolidSphere(0.13, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 1, 0);
	glScalef(1, 1, 0);
	glTranslatef(0.2, 0.0, 0);
	glutSolidSphere(0.13, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 1, 0);
	glScalef(1, 1, 0);
	glTranslatef(0.1, 0.2, 0);
	glutSolidSphere(0.13, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 1, 0);
	glScalef(1, 1, 0);
	glTranslatef(0.16, 0.2, 0);
	glutSolidSphere(0.13, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 1, 0);
	glScalef(1, 1, 0);
	glTranslatef(0.13, 0.37, 0);
	glutSolidSphere(0.1, 30, 30);
	glPopMatrix();


	glPushMatrix();
	glColor3f(1.000, 0.498, 0.314);
	glBegin(GL_POLYGON);
	glVertex3f(0.165413533834586, -0.821875, 0);
	glVertex3f(0.165413533834586, -0.75, 0);
	glVertex3f(0.157894736842105, -0.75, 0);
	glVertex3f(0.111278195488722, -0.821875, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.000, 0.498, 0.314);
	glBegin(GL_POLYGON);
	glVertex3f(0.157894736842105, -0.75, 0);
	glVertex3f(0.157894736842105, -0.4625, 0);
	glVertex3f(0.150375939849624, -0.4625, 0);
	glVertex3f(0.117293233082707, -0.56875, 0);
	glVertex3f(0.111278195488722, -0.56875, 0);
	glVertex3f(0.111278195488722, -0.821875, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.000, 0.498, 0.314);
	glBegin(GL_POLYGON);
	glVertex3f(0.150375939849624, -0.4625, 0);
	glVertex3f(0.150375939849624, -0.171875, 0);
	glVertex3f(0.147368421052632, -0.171875, 0);
	glVertex3f(0.124812030075188, -0.284375, 0);
	glVertex3f(0.118796992481203, -0.284375, 0);
	glVertex3f(0.118796992481203, -0.56875, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.000, 0.498, 0.314);
	glBegin(GL_POLYGON);
	glVertex3f(0.147368421052632, -0.171875, 0);
	glVertex3f(0.150375939849624, -0.028125, 0);
	glVertex3f(0.121804511278195, -0.028125, 0);
	glVertex3f(0.124812030075188, -0.284375, 0);
	glEnd();
	glPopMatrix();

	glPopMatrix();
}
//func for drawing tree using solid cone with rotation 
//x,y,z for translation
//a,b,c for scalling
void cone_tree(double x, double y, double z, double a, double b, double c)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(a, b, c);

	glPushMatrix();
	glColor3f(0.22, 0.78, 0.25);
	glTranslatef(-0.46, -0.2, 0.0);
	glScalef(0.4, 0.5, 0.0);
	glRotatef(-71, 1.0, 0.0, 0.0);
	glutSolidCone(.22, .95, 20, 20);
	glPopMatrix();


	glPushMatrix();
	glColor3f(0.22, 0.78, 0.25);
	glTranslatef(-0.46, -0.07, 0.0);
	glScalef(0.5, 0.6, 0.0);
	glRotatef(-71, 1.0, 0.0, 0.0);
	glutSolidCone(.23, .95, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.22, 0.78, 0.25);
	glTranslatef(-0.46, 0.05, 0.0);
	glScalef(0.5, 0.6, 0.0);
	glRotatef(-71, 1.0, 0.0, 0.0);
	glutSolidCone(.24, .95, 20, 20);
	glPopMatrix();



	glPushMatrix();
	glTranslatef(-0.6, -0.1, 0);
	glPushMatrix();
	glColor3f(1.000, 0.498, 0.314);
	glBegin(GL_POLYGON);
	glVertex3f(0.165413533834586, -0.821875, 0);
	glVertex3f(0.165413533834586, -0.75, 0);
	glVertex3f(0.157894736842105, -0.75, 0);
	glVertex3f(0.111278195488722, -0.821875, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.000, 0.498, 0.314);
	glBegin(GL_POLYGON);
	glVertex3f(0.157894736842105, -0.75, 0);
	glVertex3f(0.157894736842105, -0.4625, 0);
	glVertex3f(0.150375939849624, -0.4625, 0);
	glVertex3f(0.117293233082707, -0.56875, 0);
	glVertex3f(0.111278195488722, -0.56875, 0);
	glVertex3f(0.111278195488722, -0.821875, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.000, 0.498, 0.314);
	glBegin(GL_POLYGON);
	glVertex3f(0.150375939849624, -0.4625, 0);
	glVertex3f(0.150375939849624, -0.171875, 0);
	glVertex3f(0.147368421052632, -0.171875, 0);
	glVertex3f(0.124812030075188, -0.284375, 0);
	glVertex3f(0.118796992481203, -0.284375, 0);
	glVertex3f(0.118796992481203, -0.56875, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.000, 0.498, 0.314);
	glBegin(GL_POLYGON);
	glVertex3f(0.147368421052632, -0.171875, 0);
	glVertex3f(0.150375939849624, -0.028125, 0);
	glVertex3f(0.121804511278195, -0.028125, 0);
	glVertex3f(0.124812030075188, -0.284375, 0);
	glEnd();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}
//func for drawing rain
///x,y,z for translation
void draw_rain(double x, double y, double z){
	glPushMatrix();
	glLineWidth(1.0);//set line width
	glRotatef(10,0,0,1);//rotate 10 in z exis
	glTranslatef(x,y,z);//callinf translation with parameter x,y,z
	glColor3d(255,255,255);//set white color
	for (float j = -1.8; j <= 100; j += 0.09) //for loop for y exis
	{
		for (float i = -1; i <= 2; i += 0.05) //for loop for x exis
		{
			glBegin(GL_LINES);
			glVertex3f(-0.52 + i, 0.975 + j, 0); //add i and j in x and y exis
			glVertex3f(-0.52 + i, 0.945 + j, 0); //add i and j in x and y exis
			glEnd();
		}
	}
	glPopMatrix();

}
//func for draw sun and moon (solid sphare) 
//x,y,z for translation
///a,b,c for rotation
//color_r,color_g,color_b for change color with day night effect(day >> sun,night >>moon)
void draw_sun_moon(double x, double y, double z, double a, double b, double c, double color_r, double color_g, double color_b)
{
	glPushMatrix();
	glScalef(a, b, c);
	glColor3f(color_r, color_g, color_b);
	glTranslatef(x, y, z);
	glutSolidSphere(0.1, 30, 30);
	glPopMatrix();
}
//drawing a man character with unbrella
//x,y,z for translation
//a,b,c for scalling
void draw_man_with_unbrella(double x, double y, double z, double a, double b, double c)
{
	int i = 0;
	float xx = 0.078195488;
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(a, b, c);

	for (int j = 0; j < 2000; j++)
	{

		if (i == 0)
		{
			glPushMatrix();
			glColor3f(1, 1, 1);
			glBegin(GL_LINES);
			glVertex3f(0.0180451127819549 + xx, -0.546875, 0);
			glVertex3f(-0.0210526315789473 + xx, -0.396875, 0);
			glVertex3f(-0.0210526315789473 + xx, -0.109375, 0);
			glVertex3f(-0.0210526315789473 + xx, -0.396875, 0);
			glEnd();
			glPopMatrix();
			i = 1;
		}

		if (i == 1)
		{
			glPushMatrix();
			glColor3f(1, 1, 1);
			glBegin(GL_LINES);
			glVertex3f(-0.0210526315789473 + xx, -0.396875, 0);
			glVertex3f(-0.0601503759398496 + xx, -0.546875, 0);
			glEnd();
			glPopMatrix();
			i = 0;
		}

			glPushMatrix();
			glColor3f(1, 1, 1);
			glBegin(GL_LINE_LOOP);
			glVertex3f(-0.0195488721804511 + xx, -0.159375, 0);
			glVertex3f(0.0105263157894737 + xx, -0.215625, 0);
			glVertex3f(0.0406015037593985 + xx, -0.159375, 0);
			glVertex3f(0.00902255639097744 + xx, -0.24375, 0);
			glVertex3f(-0.0195488721804511 + xx, -0.19375, 0);
			glEnd();
			glPopMatrix();

			glPushMatrix();
			glColor3f(1, 1, 1);
			glBegin(GL_LINES);
			glVertex3f(0.0406015037593985 + xx, -0.159375, 0);
			glVertex3f(0.0406015037593985 + xx, 0.115625, 0);
			glEnd();
			glPopMatrix();


		glPushMatrix();
		glColor3f(1, 1, 1);
		glBegin(GL_LINES);
		glVertex3f(0.171428571428571 + xx, 0.1125, 0);
		glVertex3f(-0.087218045112782 + xx, 0.1125, 0);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glColor3f(1, 1, 1);
		glTranslatef(0.0, 0.01, 0.0);
		glBegin(GL_POLYGON);
		for (int i = 0; i<50; i++)
		{
			float pi = 3.1416;
			float A = (i * 2 * pi) / 100;
			float r = 0.14;
			float x = r * cos(A);
			float y = r * sin(A);
			glVertex2f(x, y);
		}
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glColor3f(1, 1, 1);
		glScalef(0.8, 1, 0);
		glTranslatef(-0.03, -0.08, 0);
		glutSolidSphere(.04, 20, 20);
		glPopMatrix();

	}

	glPopMatrix();
}

//Draws the 3D scene
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity(); //Reset the drawing perspective


	////draw_man_with_unbrella(0,0,0,1,1,0);
	
	///writeBitmapString((void*)font, "Spotlight attenuation: ");
	////writeBitmapString((void*)font, theStringBuffer);

	///translating tree in different places
	draw_tree(0.5, -0.7, 0, 0.3, 0.3, 0);
	///translating tree in different places
	draw_tree(0.3, -0.5, 0, 0.3, 0.3, 0);
	///translating tree in different places
	draw_tree(0.7, -0.5, 0, 0.3, 0.3, 0);
	///translating tree in different places
	draw_tree(0.9, -0.7, 0, 0.3, 0.3, 0);
	///translating tree in different places
	draw_tree(-0.5, -0.7, 0, 0.3, 0.3, 0);
	///translating tree in different places
	draw_tree(-0.2, -0.6, 0, 0.3, 0.3, 0);
	///translating tree in different places
	draw_tree(-0.9, -0.7, 0, 0.3, 0.3, 0);
	///translating cone tree in different places
	cone_tree(-0.5, -0.55, 0, 0.4, 0.4, 0);
	///translating cone tree in different places
	cone_tree(-0.1, -0.55, 0, 0.4, 0.4, 0);
	///translating cone tree in different places
	cone_tree(0.3, -0.55, 0, 0.4, 0.4, 0);

	//draw the bd flag
	draw_flag(-0.18, 0.05, 0, 1, 1, 0);

	//draw the first cloud which is moving from the left to right (with translation 0.001)
	if (move_right_cloud <= 3.0)
	{
		move_right_cloud += 0.001;//increasing translation variable value
		draw_cloud(-1.2 + move_right_cloud, 1.2, 0); //add value with its actual translated point
	}
	if (move_right_cloud > 3.0) //if its cross the border of screen
	{
		move_right_cloud = -0.001; //agin translate it to start from it actual translated point
	}

	//draw the second cloud which is moving from the right to left (with translation 0.001)
	if (move_left_cloud >= -2.62)
	{
		move_left_cloud -= 0.001; //increasing translation variable value
		draw_cloud(1.12 + move_left_cloud, 1.27, 0);//add value with its actual translated point
	}

	if (move_left_cloud < -2.62) //if its cross the border of screen
	{
		move_left_cloud = +0.001; //agin translate it to start from it actual translated point
	}

	///draw sun with day and night effect
	glPushMatrix();

	if (day_night == 0) //if it is day effect
	{
		glEnable(GL_LIGHTING);//enable lighting
		glEnable(GL_LIGHT0); //Enable light #0
		glEnable(GL_LIGHT1);//enable lighting #1

		GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //Color (0.2, 0.2, 0.2)
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

		//Add positioned light
		GLfloat lightColor0[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //Color (0.5, 0.5, 0.5)
		GLfloat lightPos0[] = { 4.0f, 0.0f, 8.0f, 1.0f }; //Positioned at (4, 0, 8)
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

		//Add directed light
		GLfloat lightColor1[] = { 0.5f, 0.2f, 0.2f, 1.0f }; //Color (0.5, 0.2, 0.2)
		//Coming from the direction (-1, 0.5, 0.5)
		GLfloat lightPos1[] = { 0.0f, 0.0f, 1.0f, 0.0f };
		glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
		glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

		if (day_night == 0 && rain == 1) //if it is day and rain effect
		{
			//draw_rain();
			if (move_rain <= 2.0)
			{
				move_rain -= 0.1;
				draw_rain(0.0, 2.0 + move_rain, 0); //translating rain in y axis
			}
		}
		if (day_night == 0 && rain == 0) //if it is day but no rain
		{
			draw_sun_moon(-0.6, 0.89, 0, 0.8, 1, 0, 1, 1, 0); //draw sun with translation and scalling and color yellow 
		}
		
		glDisable(GL_LIGHT0); //disable light 0
		glDisable(GL_LIGHT1); //disable light 1
		glDisable(GL_LIGHTING); //disable light 


	}
	if (day_night == 1) //night
	{
		///glDisable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);//enable lighting
		glEnable(GL_LIGHT0); //Enable light #0
		glEnable(GL_LIGHT1);//enable light 1

		GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //Color (0.2, 0.2, 0.2)
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

		//Add positioned light
		GLfloat lightColor0[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //Color (0.5, 0.5, 0.5)
		GLfloat lightPos0[] = { 4.0f, 0.0f, 8.0f, 1.0f }; //Positioned at (4, 0, 8)
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

		//Add directed light
		GLfloat lightColor1[] = { 0.5f, 0.2f, 0.2f, 1.0f }; //Color (0.5, 0.2, 0.2)
		//Coming from the direction (-1, 0.5, 0.5)
		GLfloat lightPos1[] = { 0.0f, 0.0f, 1.0f, 0.0f };
		glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
		glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

		if (day_night == 1 && rain == 1) //if it is night and rain
		{
			///draw_rain();
			if (move_rain <= 2.0) //rain comes from out of the screen
			{
				move_rain -= 0.1; //decrease value
				draw_rain(0.0, 2.0 + move_rain, 0); //translating and calling function
			}
		}
		if (day_night == 1 && rain == 0)//if it is night but not rain
		{
			draw_sun_moon(-0.6, 0.89, 0, 0.8, 1, 0, 1.000, 0.980, 0.941);//moon
		}
		
		glDisable(GL_LIGHT0);//disable light0
		glDisable(GL_LIGHT1);// disable light
	}
	glPopMatrix();

	glPushMatrix();
	if (day_night == 0)//day
	{
		glEnable(GL_LIGHTING);//enable lighting
		glEnable(GL_LIGHT0); //Enable light #0
		glEnable(GL_LIGHT1);//enable light1

		GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //Color (0.2, 0.2, 0.2)
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

		//Add positioned light
		GLfloat lightColor0[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //Color (0.5, 0.5, 0.5)
		GLfloat lightPos0[] = { 4.0f, 0.0f, 8.0f, 1.0f }; //Positioned at (4, 0, 8)
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

		//Add directed light
		GLfloat lightColor1[] = { 0.5f, 0.2f, 0.2f, 1.0f }; //Color (0.5, 0.2, 0.2)
		//Coming from the direction (-1, 0.5, 0.5)
		GLfloat lightPos1[] = { 0.0f, 0.0f, 1.0f, 0.0f };
		glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
		glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
		////draw_sun_moon(-0.6, 0.89, 0, 0.8, 1, 0, 1, 1, 0);

		//draw first lamp post bottom which is start from left
		draw_lamppost(0.9,0.9,0.9,-3.0, -0.059, 0, 0.2, 1, 1,255,255,255);

		//draw second lamp post bottom which is start from left
		draw_lamppost(0.9, 0.9, 0.9, -0.0, -0.059, 0, 0.2, 1, 1, 255, 255, 255);

		//draw third lamp post bottom which is start from left
		draw_lamppost(0.9, 0.9, 0.9, 3.0, -0.059, 0, 0.2, 1, 1, 255, 255, 255);

		//draw first lamp post up which is start from left
		draw_lamppost(0.9, 0.9, 0.9, 3.0, 0.45, 0, -0.2, 1, 1, 255, 255, 255);

		//draw second lamp post up which is start from left
		draw_lamppost(0.9, 0.9, 0.9, 0.0, 0.45, 0, -0.2, 1, 1, 255, 255, 255);

		//draw third lamp post up which is start from left
		draw_lamppost(0.9, 0.9, 0.9, -3.0, 0.45, 0, -0.2, 1, 1, 255, 255, 255);

		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHTING);
	}
	if (day_night == 1)//night
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0); //Enable light #0
		glEnable(GL_LIGHT1);

		GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //Color (0.2, 0.2, 0.2)
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

		//Add positioned light
		GLfloat lightColor0[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //Color (0.5, 0.5, 0.5)
		GLfloat lightPos0[] = { 4.0f, 0.0f, 8.0f, 1.0f }; //Positioned at (4, 0, 8)
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

		//Add directed light
		GLfloat lightColor1[] = { 0.5f, 0.2f, 0.2f, 1.0f }; //Color (0.5, 0.2, 0.2)
		//Coming from the direction (-1, 0.5, 0.5)
		GLfloat lightPos1[] = { 0.0f, 0.0f, 1.0f, 0.0f };
		glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
		glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
		////draw_sun_moon(-0.6, 0.89, 0, 0.8, 1, 0, 1.000, 0.980, 0.941);


		//draw first lamp post bottom which is start from left
		draw_lamppost(0.9, 0.9, 0.9, -3.0, -0.059, 0, 0.2, 1, 1, 255, 255, 0);

		//draw second lamp post bottom which is start from left
		draw_lamppost(0.9, 0.9, 0.9, -0.0, -0.059, 0, 0.2, 1, 1, 255, 255, 0);

		//draw third lamp post bottom which is start from left
		draw_lamppost(0.9, 0.9, 0.9, 3.0, -0.059, 0, 0.2, 1, 1, 255, 255, 0);

		//draw first lamp post up which is start from left
		draw_lamppost(0.9, 0.9, 0.9, 3.0, 0.45, 0, -0.2, 1, 1, 255, 255, 0);

		//draw second lamp post up which is start from left
		draw_lamppost(0.9, 0.9, 0.9, 0.0, 0.45, 0, -0.2, 1, 1, 255, 255, 0);

		//draw third lamp post up which is start from left
		draw_lamppost(0.9, 0.9, 0.9, -3.0, 0.45, 0, -0.2, 1, 1, 255, 255, 0);

		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
	}


	glPopMatrix();

	if (move_car >= -1.5)//move car
	{
		move_car += 0.02;//increase value
		draw_car(-1.5 + move_car, 0.18, 0);//move car towards x axis
	}
	if (move_car > 3.0)//if car cross the end point
	{
		move_car = -0.02;//move car from the beinning
	}
	//draw_car(1.3, 0.18, 0);

	//helping hand degin down start from left
	helping_hand(0, 0, 0);
	helping_hand(0.2, 0, 0);
	helping_hand(0.4, 0, 0);
	helping_hand(0.6, 0, 0);
	helping_hand(0.8, 0, 0);
	helping_hand(1.0, 0, 0);
	helping_hand(1.2, 0, 0);
	helping_hand(1.4, 0, 0);
	helping_hand(1.6, 0, 0);
	helping_hand(1.77, 0, 0);

	//helping hend degin up start from left
	helping_hand(-0.15, 0.42, 0);
	helping_hand(0, 0.42, 0);
	helping_hand(0.2, 0.42, 0);
	helping_hand(0.4, 0.42, 0);
	helping_hand(0.6, 0.42, 0);
	helping_hand(0.8, 0.42, 0);
	helping_hand(1.0, 0.42, 0);
	helping_hand(1.2, 0.42, 0);
	helping_hand(1.4, 0.42, 0);
	helping_hand(1.6, 0.42, 0);


	// draw the main road side line up
	glPushMatrix();
	glColor3f(1, 1, 1);
	glLineWidth(1.5);
	glBegin(GL_LINES);
	glVertex3f(-0.99, 0.01875, 0);
	glVertex3f(0.855639097744361, 0.01875, 0);
	glEnd();
	glPopMatrix();

	// draw the main road side line down
	glPushMatrix();
	glColor3f(1, 1, 1);
	glLineWidth(1.5);
	glBegin(GL_LINES);
	glVertex3f(-0.89, -0.409375, 0);
	glVertex3f(0.99, -0.409375, 0);
	glEnd();
	glPopMatrix();


	//draw the first truck
	if (move_truck <= 1.7)//move truck
	{
		move_truck -= 0.025;//increase value
		draw_truck(1.7 + move_truck, 0.26, 0);//move car towards x axis
	}
	if (move_truck < -4.9)//if truck goes to end point
	{
		move_truck = +0.025;//starting from beginning
	}

	///draw the 1st bus and move it from right to left
	if (move_1st_bus >= -2.7)//move bus
	{
		move_1st_bus -= 0.011;//increase value
		draw_bus(1.5 + move_1st_bus, 0.5, 0, 0, 1, 0, 0.502, 0.0, 0.0);//move bus towards x axis
	}
	if (move_1st_bus < -2.7)//if bus goes to end point
	{
		move_1st_bus = +0.011;//starting from beginning
	}

	///draw the 2nd bus and move it from right to left
	if (move_2nd_bus >= -4.5)
	{
		move_2nd_bus -= 0.015;
		draw_bus(2.5 + move_2nd_bus, 0.5, 0, 0.678, 1.000, 0.184, 0.196, 0.804, 0.196);
	}
	if (move_2nd_bus < -4.5)
	{
		move_2nd_bus = +0.015;
	}


	//draw passenger stand
	draw_passenger_stand(-0.11, 0.04, 0, 0.8, 0.8, 0);


	//write into the bus stop board
	write_bus_stop(2.90, 6.0, 0, 0.15, 0.10, 0);


	//draw the bus which is inside the bus stop board
	draw_bus_stop_board_bus(0.04, 0, 0, 0.8, 0.8, 0);

	//draw only bus stop board
	draw_bus_stop_board(0.04, 0.04, 0, 0.8, 0.8, 0);


	//draw only dust bin
	dustbin(0.09, 0.08, 0, 0.6, 0.6, 0);


	//draw road km board
	draw_km_sign(-0.02, 0, 0);


	//draw the maid rode devider
	draw_road_breaker();


	//deaw the main road widh----
	drawpoly(1.0015037593985, -0.453125, 0.852631578947368, 0.05, -1, 0.05, -0.879699248120301, -0.453125, 0.2, 0.2, 0.2);


	//draw like foothpath up side
	glPushMatrix();
	glColor3f(0.3, 0.3, 0.3);
	glBegin(GL_POLYGON);
	glVertex3f(0.852631578947368, 0.05, 0);
	glVertex3f(0.835, 0.109375, 0);
	glVertex3f(-1, 0.109375, 0);
	glVertex3f(-1, 0.05, 0);
	glEnd();
	glPopMatrix();

	//draw foothpath down side(1st)
	glPushMatrix();
	glColor3f(0.5, 0.28, 0.28);
	glBegin(GL_POLYGON);
	glVertex3f(-0.833082706766917, -0.577, 0);
	glVertex3f(-0.833082706766917, -0.45, 0);
	glVertex3f(-0.88, -0.45, 0);
	glVertex3f(-0.88, -0.577, 0);
	glEnd();
	glPopMatrix();


	//soli behind the road
	drawpoly(1.0015037593985, -1, 1.0015037593985, -0.575, -1, -0.575, -1, -1, 0.0, 0.60, 0.0);


	//draw foothpath down side(2nd-last)
	draw_foothpath();


	//draw first building from left side
	draw_building(0, 0.065, 0, 1, 0.8, 0);

	//draw third building from left side
	draw_building(0.34, 0.065, 0, 1, 0.8, 0);

	//draw second building from left side
	draw_building(0.2, 0.2, 0, 1, 0.8, 0);

	//draw first building from right side
	draw_large_building(-0.03, 0.0, 0.0, 1, 0.8, 0);

	if (day_night == 0)//day
	{
		glClearColor(0.0, 0.7, 1.5, 0.0);//set background color
	}

	if (day_night == 1)//night
	{
		glClearColor(0.0, 0.0, 0.0, 0.0);//set background color
	}


	glutSwapBuffers();
}

void update(int value) {
	_angle += 2.0f;
	if (_angle > 360) {
		_angle -= 360;
	}
	_ang_tri += 2.0f;
	if (_ang_tri > 360) {
		_ang_tri -= 360;
	}

	glutPostRedisplay(); //Tell GLUT that the display has changed

	//Tell GLUT to call update again in 25 milliseconds
	glutTimerFunc(25, update, 0);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {

	case 27: ///Escape key
		exit(0);
		break;

	case 'd': //case for day effect
		day_night = 0;
		rain = 0;
		glutPostRedisplay();
		break;

	case 'n': //case for night effect
		day_night = 1;
		rain = 0;
		glutPostRedisplay();
		break;

	case 'r': //case for rain effect
		rain = 1;
		glutPostRedisplay();
		break;

	default:  //case for default
		break;
	}
}

int main(int argc, char** argv)
{
	//Initialize GLUT

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1331, 641);
	glutInitWindowPosition(0, 0);

	//Create the window
	glutCreateWindow("An urban scenario design");
	initRendering();

	//Set handler functions
	glutDisplayFunc(drawScene);

	glutReshapeFunc(handleResize);//add a resize method

	glutKeyboardFunc(keyboard);//add a keyboard method

	glutTimerFunc(25, update, 0); //Add a timer

	glutMainLoop();

	return 0;
}