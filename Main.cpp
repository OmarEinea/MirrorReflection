#include <iostream>
#include <windows.h> 
#include <gl/Gl.h>
#include <gl/glut.h>

using namespace std;

const int screenWidth = 1280;
const int screenHeight = 720;

void myDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Mirror Reflection Game");
	glutDisplayFunc(myDisplay);
	glClearColor(1, 1, 1, 0);
	glColor3f(0, 0, 0);
	glPointSize(8.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, screenWidth, 0.0, screenHeight);
	glutMainLoop();
}