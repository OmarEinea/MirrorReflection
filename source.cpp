#include <iostream>
#include <windows.h> 
#include <gl/Gl.h>
#include <gl/glut.h>
#include <vector>
#include "classes.h"

using namespace std;

// Window Width and Height
const int w = 1280, h = 720;
vector<Mirror> mirrors;

void myDisplay(void) {
	Light light(w, h);
	while (light.calcClosestReflection(mirrors)) {}

	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_LINE_STRIP);
	// Draw Light Source
	glColor3f(1, 0, 0);
	for (Point p : light.points)
		glVertex2i(p.x, p.y);
	glEnd();

	glBegin(GL_LINES);
	// Draw Mirrors
	glColor3f(0, 0, 1);
	for (Mirror m : mirrors) {
		glVertex2i(m.p1.x, m.p1.y);
		glVertex2i(m.p2.x, m.p2.y);
	}
	glEnd();

	glutSwapBuffers();
	glFlush();
}

void myMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN && (mirrors.size() == 0 || mirrors.back().released))
			mirrors.push_back(Mirror(Point(x, h - y)));
		else
			mirrors.back().released = true;
	glutPostRedisplay();
}

void myMovedMouse(int x, int y) {
	if (mirrors.size() > 0 && !mirrors.back().released) {
		mirrors.back().p2.x = x;
		mirrors.back().p2.y = h - y;
	}
	glutPostRedisplay();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Mirror Reflection Game");
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMovedMouse);
	glClearColor(1, 1, 1, 0);
	glColor3f(0, 0, 0);
	glPointSize(8.0);
	glLineWidth(4.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glutMainLoop();
}