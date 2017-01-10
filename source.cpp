#include <iostream>
#include <windows.h> 
#include <gl/Gl.h>
#include <gl/glut.h>
#include <vector>
#include <ctime>
#include "classes.h"

using namespace std;

// Window Width and Height
const GLint w = 1280, h = 720;
GLint level = 2;
vector<Mirror> mirrors;
Point *clicked = NULL;

GLvoid generateObstacles(GLint level) {
	mirrors.clear();
	Mirror goal = Mirror(Point(2, 0), Point(2, h));
	goal.goal = true;
	mirrors.push_back(goal);
	// Obstacle's x position, window height & y position
	GLdouble x, wh, wy;
	GLint i, walls = level * 5;
	while (walls > 0) {
		x = rand() % w * 0.85 + w * 0.05;
		for (i = 0; i < mirrors.size(); i++)
			if (abs(mirrors[i].p1.x - x) < 25)
				break;
		if (i != mirrors.size())
			continue;
		wh = rand() % (h / 3) + h / 8;
		wy = rand() % (h * 2 / 3);

		mirrors.push_back(Mirror(Point(x, 0), Point(x, wy)));
		mirrors.push_back(Mirror(Point(x, wy + wh), Point(x, h)));
		walls--;
	}
	glutPostRedisplay();
}

GLvoid myDisplay(GLvoid) {
	Light light(w, h);
	while (light.calcClosestReflection(mirrors)) {}
	if (light.won && !clicked) {
		generateObstacles(++level);
		light.won = false;
	}
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_LINE_STRIP);
	// Draw Light Source
	glColor3f(1, 0, 0);
	for (Point p : light.points)
		glVertex2i(p.x, p.y);
	glEnd();

	for (Mirror m : mirrors) {
		// Draw Mirrors or Obstacles Lines
		glColor3f(0, m.goal, m.type);
		glBegin(GL_LINES);
		glVertex2i(m.p1.x, m.p1.y);
		glVertex2i(m.p2.x, m.p2.y);
		glEnd();
		// If Mirrors Draw their Handles
		if(m.type) {
			glBegin(GL_POINTS);
			glColor3f(0, 0.5, 1);
			glVertex2i(m.p1.x, m.p1.y);
			glVertex2i(m.p2.x, m.p2.y);
			glEnd();
		}
	}

	glutSwapBuffers();
	glFlush();
}

GLvoid myMouse(GLint button, GLint state, GLint x, GLint y) {
	Point mouse(x, h - y);
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN && !clicked) {
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

GLvoid myMovedMouse(GLint x, GLint y) {
	if(clicked) {
		clicked->x = x;
		clicked->y = h - y;
	}
	glutPostRedisplay();
}	

GLvoid main(GLint argc, char** argv) {
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
	srand(time(0));
	generateObstacles(level);
	glutMainLoop();
}