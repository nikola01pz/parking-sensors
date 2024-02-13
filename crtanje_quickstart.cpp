// #define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>

#include "GL/glut.h"
#include "../../sndfile.hh"


#include <math.h>
#include <iostream>

#include <windows.h>
#include <mmsystem.h>
#include <io.h>


#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <cstdlib>
#include <minmax.h>


using namespace std;
void display(void);
void reshape(int, int);
void idle();

int zvuk = 0;
int sensors[2][8] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int sensorSideActive[2][2] = { 0, 0, 0, 0 };

// user defined functions
void sensorDisplay();
void drawFR1();
void drawFR2();
void drawFR3();
void drawFL1();
void drawFL2();
void drawFL3();
void drawBR1();
void drawBR2();
void drawBR3();
void drawBL1();
void drawBL2();
void drawBL3();

void readSensors(unsigned char, int, int);
void selectBackgroundColor(int r, int g, int b);
void makeSound(int proximity);
void playSoundIntensity();
void makeSoundRight(int proximity);
void makeSoundLeft(int proximity);
void makeSoundFront(int proximity);
void makeSoundBack(int proximity);

unsigned char* loadPPM(const char* filename, int& width, int& height) {
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}
	retval_fgets = fgets(buf[0], BUFSIZE, fp);
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width = atoi(buf[1]);
	height = atoi(buf[2]);
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}
	return rawData;
}

void initGL()
{
	glEnable(GL_TEXTURE_2D); // enable texture mapping
	glShadeModel(GL_SMOOTH); // enable smooth shading
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // get clear background (black color)
	glClearDepth(1.0f); // color depth buffer
	glDepthFunc(GL_LEQUAL); // configuration of depth testing
	//enable additional options regarding: perspective correction, anti-aliasing, etc
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

void loadTexture()
{
	GLuint texture[1]; // declaring space for one texture
	int twidth, theight; // declaring variable for width and height of an image
	unsigned char* tdata; // declaring pixel data
	// loading image data from specific file:
	tdata = loadPPM("..\\auto3.ppm", twidth, theight);
	if (tdata == NULL) return; // check if image data is loaded
	// generating a texture to show the image
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE,
		tdata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

int main(int argc, char** argv) {
	/* 1) INITIALIZATION */
	// initialize GLUT
	glutInit(&argc, argv);
	// set window position and size
	glutInitWindowPosition(545, 180);
	glutInitWindowSize(720, 720);
	// set the combination of predefined values for display mode
	// set color space (Red, Green, Blue - RGB)
	// alocate depth buffer
	// set the size of the buffer (double)
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	// create window
	glutCreateWindow("Screen");
	/* 2) REGISTRATION OF CALLBACK FUNCTION */
	// function called when new window need to be drawn
	glutDisplayFunc(display);
	// function called when window changes the size
	glutReshapeFunc(reshape);
	// function called when nothing else is executing and CPU is free
	glutIdleFunc(idle);
	// function called when keyboard key is pressed
	glutKeyboardFunc(readSensors); // custom function 'readSensors' can	be implemented separately
	loadTexture();
	initGL();
	/* 3) START GLUT PROCESSING CYCLE */
	glutMainLoop();
	return 0;
}

void readSensors(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'q':
		PlaySound(NULL, NULL, 0);
		sensors[0][1] = 0;
		sensors[0][2] = 0;
		sensors[0][3] = 0;
		sensorSideActive[0][0] = 0;
		display();
		break;
	case 'w':
		sensors[0][1] = 1;
		sensors[0][2] = 0;
		sensors[0][3] = 0;
		sensorSideActive[0][0] = 1;
		display();
		break;
	case 'e':
		sensors[0][1] = 1;
		sensors[0][2] = 1;
		sensors[0][3] = 0;
		sensorSideActive[0][0] = 1;
		display();
		break;
	case 'r':
		sensors[0][1] = 1;
		sensors[0][2] = 1;
		sensors[0][3] = 1;
		sensorSideActive[0][0] = 1;
		display();
		break;
	case 't':
		sensors[0][4] = 1;
		sensors[0][5] = 1;
		sensors[0][6] = 1;
		sensorSideActive[0][1] = 1;
		display();
		break;
	case 'z':
		sensors[0][4] = 0;
		sensors[0][5] = 1;
		sensors[0][6] = 1;
		sensorSideActive[0][1] = 1;
		display();
		break;
	case 'u':
		sensors[0][4] = 0;
		sensors[0][5] = 0;
		sensors[0][6] = 1;
		sensorSideActive[0][1] = 1;
		display();
		break;
	case 'i':
		PlaySound(NULL, NULL, 0);
		sensors[0][4] = 0;
		sensors[0][5] = 0;
		sensors[0][6] = 0;
		sensorSideActive[0][1] = 0;
		display();
		break;
	case 'a':
		PlaySound(NULL, NULL, 0);
		sensors[1][1] = 0;
		sensors[1][2] = 0;
		sensors[1][3] = 0;
		sensorSideActive[1][0] = 0;
		display();
		break;
	case 's':
		sensors[1][1] = 1;
		sensors[1][2] = 0;
		sensors[1][3] = 0;
		sensorSideActive[1][0] = 1;
		display();
		break;
	case 'd':
		sensors[1][1] = 1;
		sensors[1][2] = 1;
		sensors[1][3] = 0;
		sensorSideActive[1][0] = 1;
		display();
		break;
	case 'f':
		sensors[1][1] = 1;
		sensors[1][2] = 1;
		sensors[1][3] = 1;
		sensorSideActive[1][0] = 1;
		display();
		break;
	case 'g':
		sensors[1][4] = 1;
		sensors[1][5] = 1;
		sensors[1][6] = 1;
		sensorSideActive[1][1] = 1;
		display();
		break;
	case 'h':
		sensors[1][4] = 0;
		sensors[1][5] = 1;
		sensors[1][6] = 1;
		sensorSideActive[1][1] = 1;
		display();
		break;
	case 'j':
		sensors[1][4] = 0;
		sensors[1][5] = 0;
		sensors[1][6] = 1;
		sensorSideActive[1][1] = 1;
		display();
		break;
	case 'k':
		PlaySound(NULL, NULL, 0);
		sensors[1][4] = 0;
		sensors[1][5] = 0;
		sensors[1][6] = 0;
		sensorSideActive[1][1] = 0;
		display();
		break;
	case 'c':
		PlaySound(NULL, NULL, 0);
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				sensors[i][j] = 0;
			}
		}
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				sensorSideActive[i][j] = 0;
			}
		}
		display();
	}
	printf("Activation matrix:\n");
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			printf("%d\t", sensors[i][j]);
		}
		printf("\n");
	}

	printf("Sensor sides active:\n");
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			printf("%d\t", sensorSideActive[i][j]);
		}
		printf("\n");
	}
}

void display() {
	cerr << "display callback" << endl;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_QUADS);
	selectBackgroundColor(1, 1, 1);
	sensorDisplay();
	glEnd();
	glutSwapBuffers();
}

void reshape(int width, int height)
{
	cerr << "reshape callback" << endl;
	// specify the desired rectangle
	glViewport(0, 0, width, height);
	// switch to matrix projection
	glMatrixMode(GL_PROJECTION);
	// clean projection matrix
	glLoadIdentity();
	// set camera view (orthographic projection with 4x4 unit square canvas)
	glOrtho(-2, 2, -2, 2, 2, -2);
	// swith back to matrix
	glMatrixMode(GL_MODELVIEW);
}

void idle()
{
	// here comes the code which will be executed when program state is idle
}

void selectBackgroundColor(int r, int g, int b)
{
	// drawing a background white rectangle
	glColor3f(r, g, b); // choosing a color
	glTexCoord2f(0, 1); glVertex3f(-2, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(2, -1, 0);
	glTexCoord2f(1, 0); glVertex3f(2, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(-2, 1, 0);
}

void sensorDisplay()
{
	if (sensors[0][3] == 1)
	{
		drawFR1();
		drawFR2();
		drawFR3();
	}
	else if (sensors[0][2] == 1)
	{
		drawFR1();
		drawFR2();
	}
	else if (sensors[0][1] == 1)
	{
		drawFR1();
	}

	if (sensors[1][3] == 1)
	{
		drawFL1();
		drawFL2();
		drawFL3();
	}
	else if (sensors[1][2] == 1)
	{
		drawFL1();
		drawFL2();
	}
	else if (sensors[1][1] == 1)
	{
		drawFL1();
	}

	if (sensors[0][4] == 1)
	{
		drawBR1();
		drawBR2();
		drawBR3();
	}
	else if (sensors[0][5] == 1)
	{
		drawBR1();
		drawBR2();
	}
	else if (sensors[0][6] == 1)
	{
		drawBR1();
	}

	if (sensors[1][4] == 1)
	{
		drawBL1();
		drawBL2();
		drawBL3();
	}
	else if (sensors[1][5] == 1)
	{
		drawBL1();
		drawBL2();
	}
	else if (sensors[1][6] == 1)
	{
		drawBL1();
	}
	playSoundIntensity();
}

void playSoundIntensity() {
	PlaySound(NULL, NULL, 0);
	if (sensors[0][3] || sensors[1][3] || sensors[0][4] || sensors[1][4]) {
		makeSound(3);
	}
	else if (sensors[0][2] || sensors[1][2] || sensors[0][5] || sensors[1][5]) {
		makeSound(2);
	}
	else if (sensors[0][1] || sensors[1][1] || sensors[0][6] || sensors[1][6]) {
		makeSound(1);
	}
}

void makeSound(int proximity)
{
	if (sensorSideActive[0][0] && sensorSideActive[1][0])
	{
		makeSoundFront(proximity);
	}
	else if (sensorSideActive[0][1] && sensorSideActive[1][1])
	{
		makeSoundBack(proximity);
	}
	else if (sensorSideActive[0][0] || sensorSideActive[0][1])
	{
		makeSoundRight(proximity);
	}
	else if (sensorSideActive[1][0] || sensorSideActive[1][1])
	{
		makeSoundLeft(proximity);
	}
}

void makeSoundRight(int proximity)
{
	switch (proximity)
	{
	case 1:
		PlaySound("C:/stuff/faks/diplomski/okapp/project/ParkingSensorsWindows/GenerateSound/GenerateSound/ParkingSensorsAudio/R1.wav", NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
		Sleep(250);
		break;
	case 2:
		PlaySound("C:/stuff/faks/diplomski/okapp/project/ParkingSensorsWindows/GenerateSound/GenerateSound/ParkingSensorsAudio/R2.wav", NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
		Sleep(250);
		break;
	case 3:
		PlaySound("C:/stuff/faks/diplomski/okapp/project/ParkingSensorsWindows/GenerateSound/GenerateSound/ParkingSensorsAudio/R3.wav", NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
		Sleep(250);
		break;
	}
}
void makeSoundLeft(int proximity)
{
	switch (proximity)
	{
	case 1:
		PlaySound("C:/stuff/faks/diplomski/okapp/project/ParkingSensorsWindows/GenerateSound/GenerateSound/ParkingSensorsAudio/L1.wav", NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
		Sleep(250);
		break;
	case 2:
		PlaySound("C:/stuff/faks/diplomski/okapp/project/ParkingSensorsWindows/GenerateSound/GenerateSound/ParkingSensorsAudio/L2.wav", NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
		Sleep(250);
		break;
	case 3:
		PlaySound("C:/stuff/faks/diplomski/okapp/project/ParkingSensorsWindows/GenerateSound/GenerateSound/ParkingSensorsAudio/L3.wav", NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
		Sleep(250);
		break;
	}
}
void makeSoundFront(int proximity)
{
	switch (proximity)
	{
	case 1:
		PlaySound("C:/stuff/faks/diplomski/okapp/project/ParkingSensorsWindows/GenerateSound/GenerateSound/ParkingSensorsAudio/F1.wav", NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
		Sleep(250);
		break;
	case 2:
		PlaySound("C:/stuff/faks/diplomski/okapp/project/ParkingSensorsWindows/GenerateSound/GenerateSound/ParkingSensorsAudio/F2.wav", NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
		Sleep(250);
		break;
	case 3:
		PlaySound("C:/stuff/faks/diplomski/okapp/project/ParkingSensorsWindows/GenerateSound/GenerateSound/ParkingSensorsAudio/F3.wav", NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
		Sleep(250);
		break;
	}
}
void makeSoundBack(int proximity)
{
	switch (proximity)
	{
	case 1:
		PlaySound("C:/stuff/faks/diplomski/okapp/project/ParkingSensorsWindows/GenerateSound/GenerateSound/ParkingSensorsAudio/B1.wav", NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
		Sleep(250);
		break;
	case 2:
		PlaySound("C:/stuff/faks/diplomski/okapp/project/ParkingSensorsWindows/GenerateSound/GenerateSound/ParkingSensorsAudio/B2.wav", NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
		Sleep(250);
		break;
	case 3:
		PlaySound("C:/stuff/faks/diplomski/okapp/project/ParkingSensorsWindows/GenerateSound/GenerateSound/ParkingSensorsAudio/B3.wav", NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
		Sleep(250);
		break;
	}
}

void drawFR1()
{
	glColor3f(1.0, 0.50, 0.50);
	glVertex3f(-1.48f, 0.21f, 0.0f);
	glVertex3f(-1.40f, 0.16f, 0.0f);
	glVertex3f(-1.14f, 0.55f, 0.0f);
	glVertex3f(-1.22f, 0.60f, 0.0f);
}
void drawFR2()
{
	glColor3f(1.0, 0.35, 0.35);
	glVertex3f(-1.35f, 0.20f, 0.00f);
	glVertex3f(-1.28f, 0.16f, 0.0f);
	glVertex3f(-1.09f, 0.46f, 0.00f);
	glVertex3f(-1.15f, 0.50f, 0.0f);
}
void drawFR3()
{
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-1.22f, 0.20f, 0.0f);
	glVertex3f(-1.15f, 0.16f, 0.0f);
	glVertex3f(-1.03f, 0.36f, 0.0f);
	glVertex3f(-1.1f, 0.40f, 0.0f);
}
void drawFL1()
{
	glColor3f(1.0, 0.50, 0.50);
	glVertex3f(-1.48f, -0.21f, 0.0f);
	glVertex3f(-1.40f, -0.16f, 0.0f);
	glVertex3f(-1.14f, -0.55f, 0.0f);
	glVertex3f(-1.22f, -0.60f, 0.0f);
}
void drawFL2()
{
	glColor3f(1.0, 0.35, 0.35);
	glVertex3f(-1.35f, -0.20f, 0.00f);
	glVertex3f(-1.28f, -0.16f, 0.0f);
	glVertex3f(-1.09f, -0.46f, 0.00f);
	glVertex3f(-1.15f, -0.50f, 0.0f);
}
void drawFL3()
{
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-1.22f, -0.20f, 0.0f);
	glVertex3f(-1.15f, -0.16f, 0.0f);
	glVertex3f(-1.03f, -0.36f, 0.0f);
	glVertex3f(-1.1f, -0.40f, 0.0f);
}
void drawBR1()
{
	glColor3f(1.0, 0.50, 0.50);
	glVertex3f(1.48f, 0.21f, 0.0f);
	glVertex3f(1.40f, 0.16f, 0.0f);
	glVertex3f(1.14f, 0.55f, 0.0f);
	glVertex3f(1.22f, 0.60f, 0.0f);
}
void drawBR2()
{
	glColor3f(1.0, 0.35, 0.35);
	glVertex3f(1.35f, 0.20f, 0.00f);
	glVertex3f(1.28f, 0.16f, 0.0f);
	glVertex3f(1.09f, 0.46f, 0.00f);
	glVertex3f(1.15f, 0.50f, 0.0f);
}
void drawBR3()
{
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(1.22f, 0.20f, 0.0f);
	glVertex3f(1.15f, 0.16f, 0.0f);
	glVertex3f(1.03f, 0.36f, 0.0f);
	glVertex3f(1.1f, 0.40f, 0.0f);
}
void drawBL1()
{
	glColor3f(1.0, 0.50, 0.50);
	glVertex3f(1.48f, -0.21f, 0.0f);
	glVertex3f(1.40f, -0.16f, 0.0f);
	glVertex3f(1.14f, -0.55f, 0.0f);
	glVertex3f(1.22f, -0.60f, 0.0f);
}
void drawBL2()
{
	glColor3f(1.0, 0.35, 0.35);
	glVertex3f(1.35f, -0.20f, 0.00f);
	glVertex3f(1.28f, -0.16f, 0.0f);
	glVertex3f(1.09f, -0.46f, 0.00f);
	glVertex3f(1.15f, -0.50f, 0.0f);
}
void drawBL3()
{
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(1.22f, -0.20f, 0.0f);
	glVertex3f(1.15f, -0.16f, 0.0f);
	glVertex3f(1.03f, -0.36f, 0.0f);
	glVertex3f(1.1f, -0.40f, 0.0f);
}