#include <windows.h> 
#include <gl/Gl.h>
#include <vector>
#include <cmath>

using namespace std;

class Point {
public:
	GLdouble x, y;
	Point() : x(0), y(0) {}
	Point(GLdouble a, GLdouble b) : x(a), y(b) {}
};

class Vector {
public:
	GLdouble x, y;
	Vector() : x(0), y(0) {}
	Vector(GLdouble a, GLdouble b) : x(a), y(b) {}
	Vector(Point p1, Point p2) : x(p1.x - p2.x), y(p1.y - p2.y) {}
	Vector perp() {
		return Vector(-y, x);
	}
	GLdouble dot(Vector v) {
		return x * v.x + y * v.y;
	}
	GLdouble length() {
		return sqrt(x * x + y * y);
	}
	Vector normal() {
		GLdouble l = length();
		return l != 0 ? Vector(x / l, y / l) : Vector();
	}
	Vector operator*(GLdouble num) {
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
	Vector normal;
	GLdouble distance, t;
	bool type = 1, goal = 0;
	Mirror(Point a) : p1(a), p2(a) {}
	Mirror(Point a, Point b) : p1(a), p2(b), type(0) {}
};

class Light {
public:
	vector<Point> points;
	bool won = false;
	Light(GLdouble width, GLdouble height) {
		points.push_back(Point(width * 0.95, -height));
		points.push_back(Point(width * 0.95, 2 * height));
	}
	bool calcClosestReflection(vector<Mirror> mirrors) {
		Point p1 = points[points.size() - 2], *p2 = &points[points.size() - 1];
		Vector b(p1, *p2), bPerp = b.perp();
		vector<Mirror> intersecting;
		for (Mirror m : mirrors) {
			Vector d = Vector(m.p1, m.p2);
			Vector dPerp = d.perp();
			Vector c(m.p1, p1);
			GLdouble t = -c.dot(dPerp) / b.dot(dPerp);
			GLdouble u = c.dot(bPerp) / d.dot(bPerp);
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
		if (closest.goal) {
			won = true;
			return false;
		}
		*p2 = b * closest.t + p1;
		points.push_back(b - closest.normal * 2 * b.dot(closest.normal) + *p2);
		return true;
	}
};