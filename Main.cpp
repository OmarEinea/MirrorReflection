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

class Vector {
public:
	double x, y;
	Vector(double a, double b) : x(a), y(b) {}
	Vector(Point p1, Point p2) : x(p1.x - p2.x), y(p1.y - p2.y) {}
	Vector perp() {
		return Vector(-y, x);
	}
	double dot(Vector v) {
		return x * v.x + y * v.y;
	}
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
	bool intersects(Mirror m) {
		Vector d = Vector(m.p1, m.p2);
		Vector dPerp = d.perp();
		for (int i = 1; i < points.size(); i++) {
			Vector b(points[i - 1], points[i]);
			Vector c(m.p1, points[i - 1]), bPerp = b.perp();
			if (b.dot(dPerp) != 0) {
				double t = - c.dot(dPerp) / b.dot(dPerp);
				double u = c.dot(bPerp) / d.dot(bPerp);
				return 0 <= t && t <= 1 && 0 <= u && u <= 1;
			}
		}
		return false;
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