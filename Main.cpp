#include <iostream>
#include <windows.h> 
#include <gl/Gl.h>
#include <gl/glut.h>

using namespace std;

// Window Width and Height
const int w = 1280, h = 720;

void myDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex2i(w * 0.95, h);
	glVertex2i(w * 0.95, 0);
	glEnd();

	glFlush();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Mirror Reflection Game");
	glutDisplayFunc(myDisplay);
	glClearColor(1, 1, 1, 0);
	glColor3f(0, 0, 0);
	glPointSize(8.0);
	glLineWidth(4.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, w, 0.0, h);
	glutMainLoop();
}