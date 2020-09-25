////////////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <complex>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>
////////////////////////////////////////////////////////////////////////////////

typedef std::complex<double> Point;
typedef std::vector<Point> Polygon;
double right_most_x = INT_MIN;

// Return true iff [a,b] intersects [c,d], and store the intersection in ans
bool intersect_segment(const Point &a, const Point &b, const Point &c, const Point &d, Point &ans) {
	// ab, cd are parallel
	if ((d.imag() - c.imag()) * (b.real() - a.real()) == (b.imag() - a.imag()) * (d.real() - c.real())) {
		// ab, cd are conllinear
		if ((b.imag() - a.imag()) * (c.real() - a.real()) == (c.imag() - a.imag()) * (b.real() - a.real())) {
			return false;
		} else return false;
	} else {
		double t1 = (c.real() * (d.imag() - c.imag()) + a.imag() * (d.real() - c.real()) - c.imag() * (d.real() - c.real()) - a.real() * (d.imag() - c.imag()))
		/ ((b.real() - a.real()) * (d.imag() - c.imag()) - (d.real() - c.real()) * (b.imag() - a.imag()));
		double t2 = (a.real() * (b.imag() - a.imag()) + c.imag() * (b.real() - a.real()) - a.imag() * (b.real() - a.real()) - c.real() * (b.imag() - a.imag()))
		/ ((d.real() - c.real()) * (b.imag() - a.imag()) - (b.real() - a.real()) * (d.imag() - c.imag()));

		if (t1 >= 0.0 && t1 <= 1.0 && t2 >= 0.0 && t2 <= 1.0) {
				ans = {a.real() + t1 * (b.real() - a.real()), a.imag() + t1 * (b.imag() - a.imag())};
		} else return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////

bool is_inside(const Polygon &poly, const Point &query) {
	Point outside(right_most_x + 1, query.imag());
	int intersect_count = 0;

	// 1. Compute bounding box and set coordinate of a point outside the polygon
	for (int i = 0; i < poly.size(); i++) {
		Point ans;
		// 2. Cast a ray from the query point to the 'outside' point, count number of intersections
		if (intersect_segment(poly[i], poly[i == poly.size() - 1 ? 0 : i + 1], query, outside, ans)) {
			intersect_count++;
		}
	}

	return intersect_count % 2 != 0;
}

////////////////////////////////////////////////////////////////////////////////

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

Polygon load_obj(const std::string &filename) {
	std::ifstream in(filename);
	std::string line;
	std::vector<Point> ret;
	while (std::getline(in, line)) {
		if (line.rfind("v", 0) == 0) {
			std::istringstream iss(line);
			char v, zero;
			double x, y;
			iss >> v >> x >> y >> zero;
			ret.push_back(Point(x, y));
		}
	}

	return ret;
}

void save_xyz(const std::string &filename, const std::vector<Point> &points) {
	std::ofstream out(filename);
	if (!out.is_open()) {
		throw std::runtime_error("failed to open file " + filename);
	}
	out << std::fixed;
	for (const auto &p : points) {
		out << p.real() << " " << p.imag() << " " << 0 << "\n";
	}
	out << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char * argv[]) {
	if (argc <= 3) {
		std::cerr << "Usage: " << argv[0] << " points.xyz poly.obj result.xyz" << std::endl;
	}
	std::vector<Point> points = load_xyz(argv[1]);
	Polygon poly = load_obj(argv[2]);

	for (Point p : poly) {
		if (p.real() > right_most_x) {
			right_most_x = p.real();
		}
	}

	//***** TEST START *******
	// Point ans;
	// std::cout << intersect_segment(Point(1, 3), Point(6, 3), Point(2, 2), Point(4, 1), ans) << std::endl;
	//*****TEST START ********

	std::vector<Point> result;
	for (size_t i = 0; i < points.size(); ++i) {
		if (is_inside(poly, points[i])) {
			result.push_back(points[i]);
		}
	}
	save_xyz(argv[3], result);
	return 0;
}
