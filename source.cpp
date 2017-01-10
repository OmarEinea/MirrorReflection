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
Point *clicked = NULL;

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

	for (Mirror m : mirrors) {
		// Draw Mirrors Lines
		glColor3f(0, 0, 1);
		glBegin(GL_LINES);
		glVertex2i(m.p1.x, m.p1.y);
		glVertex2i(m.p2.x, m.p2.y);
		glEnd();
		// Draw Mirrors Handles
		glBegin(GL_POINTS);
		glColor3f(0, 0.5, 1);
		glVertex2i(m.p1.x, m.p1.y);
		glVertex2i(m.p2.x, m.p2.y);
		glEnd();
	}


	glutSwapBuffers();
	glFlush();
}

void myMouse(int button, int state, int x, int y) {
	Point mouse(x, h - y);
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN && (mirrors.size() == 0 || !clicked)) {
			for (auto& m : mirrors) {
				if (Vector(m.p1, mouse).length() < 10) clicked = &m.p1;
				if (Vector(m.p2, mouse).length() < 10) clicked = &m.p2;
			}
			if(!clicked) {
				mirrors.push_back(Mirror(mouse));
				clicked = &mirrors.back().p1;
			}
		} else
			clicked = NULL;
	glutPostRedisplay();
}

void myMovedMouse(int x, int y) {
	if(clicked) {
		clicked->x = x;
		clicked->y = h - y;
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
	glPointSize(10);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(4);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glutMainLoop();
}