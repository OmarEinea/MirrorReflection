#include <vector>
#include <cmath>

using namespace std;

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
	Light(int width, int height) {
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