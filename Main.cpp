#include <iostream>
#include <windows.h> 
#include <gl/Gl.h>
#include <gl/glut.h>
#include <vector>

using namespace std;
// Window Width and Height
const int w = 1280, h = 720;

class Point {
public:
	double x, y;
	Point() : x(0), y(0) {}
	Point(double a, double b) : x(a), y(b) {}
};

class Mirror {
public:
	Point p1, p2;
	bool released;
	Mirror(Point a) : p1(a), p2(a), released(0) {}
	Mirror(Point a, Point b) : p1(a), p2(b), released(1) {}
};

class Light {
public:
	vector<Point> points;
	Light() {
		points.push_back(Point(w * 0.95, -h));
		points.push_back(Point(w * 0.95, 2 * h));
	}
};

vector<Mirror> mirrors;
Light light;

void myDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_LINES);
	// Draw Light Source
	glColor3f(1, 0, 0);
	for (Point p : light.points)
		glVertex2i(p.x, p.y);
	// Draw Mirrors
	glColor3f(0, 0, 1);
	for (Mirror m : mirrors) {
		glVertex2i(m.p1.x, m.p1.y);
		glVertex2i(m.p2.x, m.p2.y);
	}
	glEnd();

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