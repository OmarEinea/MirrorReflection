#include <iostream>
#include <windows.h> 
#include <gl/Gl.h>
#include <gl/glut.h>
#include <vector>
#include <cmath>

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
	Vector() : x(0), y(0) {}
	Vector(double a, double b) : x(a), y(b) {}
	Vector(Point p1, Point p2) : x(p1.x - p2.x), y(p1.y - p2.y) {}
	Vector perp() {
		return Vector(-y, x);
	}
	double dot(Vector v) {
		return x * v.x + y * v.y;
	}
	double length() {
		return sqrt(x * x + y * y);
	}
	Vector normal() {
		double l = length();
		return l != 0 ? Vector(x / l, y / l) : Vector();
	}
	Vector operator*(double num) {
		return Vector(x * num, y * num);
	}
	Vector operator-(Vector v) {
		return Vector(x - v.x, y - v.y);
	}
	Point operator+(Point p) {
		return Point(p.x + x, p.y + y);
	}
};

class Mirror {
public:
	Point p1, p2;
	bool released;
	double distance, t;
	Vector normal;
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
	bool calcClosestReflection(vector<Mirror> mirrors) {
		Point p1 = points[points.size() - 2], *p2 = &points[points.size() - 1];
		Vector b(p1, *p2), bPerp = b.perp();
		vector<Mirror> intersecting;
		for (int i = 0; i < mirrors.size(); i++) {
			Mirror m = mirrors[i];
			Vector d = Vector(m.p1, m.p2);
			Vector dPerp = d.perp();
			Vector c(m.p1, p1);
			double t = -c.dot(dPerp) / b.dot(dPerp);
			double u = c.dot(bPerp) / d.dot(bPerp);
			if (b.dot(dPerp) != 0 && 0 <= t && t <= 1 && 0 <= u && u <= 1) {
				m.distance = Vector(p1, b * -t + p1).length();
				if (m.distance > 1) {
					m.t = -t;
					m.normal = d.normal();
					intersecting.push_back(m);
				}
			}
		}
		if (intersecting.size() == 0)
			return false;
		Mirror closest = intersecting.front();
		for (Mirror m : intersecting)
			if (m.distance < closest.distance)
				closest = m;
		*p2 = b * closest.t + p1;
		points.push_back(b - closest.normal * 2 * b.dot(closest.normal) + *p2);
		return true;
	}
};

vector<Mirror> mirrors;

void myDisplay(void) {
	Light light;
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