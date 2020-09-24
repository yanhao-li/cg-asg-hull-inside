////////////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <complex>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>
#include <limits.h>
#include <math.h>
////////////////////////////////////////////////////////////////////////////////

typedef std::complex<double> Point;
typedef std::vector<Point> Polygon;

// the determinant of u and v
double inline det(const Point &u, const Point &v) {
	return u.real()*v.imag() - v.real()*u.imag();
} 

bool inline salientAngle(const Point &a, const Point &b, const Point &c) {
	// a => first point
	// b => mid point
	// c => new point
	Point ac(c.real()-a.real(), c.imag()-a.imag());
	Point ab(b.real()-a.real(), b.imag()-a.imag());

	return det(ab, ac) > 0;
}

// get the distance between two points
double inline dist(const Point &p1, const Point &p2) {
	return sqrt(pow(p2.real() - p1.real(), 2) + pow(p2.imag() - p1.imag(), 2)); 
}

struct Compare {
	Point p0; // lowest point of the poly
	bool operator ()(const Point &p1, const Point &p2) {
		Point p0p1(p1.real() - p0.real(), p1.imag() - p0.imag());
		Point p0p2(p2.real() - p0.real(), p2.imag() - p0.imag());
		double d = det(p0p1, p0p2);
		if (d == 0) {
			return dist(p0, p1) < dist(p0, p2);
		}

		return d > 0;
	}
};


std::vector<Point> load_xyz(const std::string &filename) {
	std::vector<Point> points;
	std::ifstream in(filename);

	int total_points;
	in >> total_points;

	double x, y, z;
	while (in >> x >> y >> z)
	{
		points.push_back(Point(x, y));
	}

	return points;
}

////////////////////////////////////////////////////////////////////////////////

Polygon convex_hull(std::vector<Point> &points) {
	Compare order;
	// find p0
	Point p0 = points[0];
	for (Point point : points) {
		if (point.imag() < p0.imag()) {
			p0 = point;
		} else if (point.imag() == p0.imag() && point.real() < p0.real()) {
			p0 = point;
		}
	}

	order.p0 = p0;
	std::sort(points.begin(), points.end(), order);

	// std::vector<Point> temp = points;
	// points.clear();

	// If several points have same polar angle, only keep the farthest one
	// points.push_back(temp[0]);
	// for (int i = 1; i < temp.size(); i++) {
	// 	while (i < temp.size() - 1 && getPolarAngle(p0, temp[i]) == getPolarAngle(p0, temp[i + 1])) {
	// 		i++;
	// 	}
	// 	points.push_back(temp[i]);
	// }

	Polygon hull;
	hull.push_back(points[0]);
	hull.push_back(points[1]);
	hull.push_back(points[2]);

	for (int i = 3; i < points.size(); i++) {
		while (!salientAngle(*(hull.rbegin() + 1), hull.back(), points[i])) {
			hull.pop_back();
		}

		hull.push_back(points[i]);
	}
	return hull;
}

////////////////////////////////////////////////////////////////////////////////

void save_obj(const std::string &filename, Polygon &poly) {
	std::ofstream out(filename);
	if (!out.is_open()) {
		throw std::runtime_error("failed to open file " + filename);
	}
	out << std::fixed;
	for (const auto &v : poly) {
		out << "v " << v.real() << ' ' << v.imag() << " 0\n";
	}
	// for (size_t i = 0; i < poly.size(); ++i) {
	// 	out << "l " << i+1 << ' ' << 1+(i+1)%poly.size() << "\n";
	// }
	out << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char * argv[]) {
	if (argc <= 2) {
		std::cerr << "Usage: " << argv[0] << " points.xyz output.obj" << std::endl;
	}

	std::string input_file = argv[1];
	std::string output_file = argv[2];

	std::vector<Point> points = load_xyz(input_file);

	Polygon hull = convex_hull(points);
	save_obj(output_file, hull);
	return 0;
}
