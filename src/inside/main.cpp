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

double inline det(const Point &u, const Point &v) {
	// TODO
	return 0;
}

// Return true iff [a,b] intersects [c,d], and store the intersection in ans
bool intersect_segment(const Point &a, const Point &b, const Point &c, const Point &d, Point &ans) {
	double deno = (a.real() - b.real()) * (c.imag()-d.imag()) - (a.imag() - b.imag()) * (c.real() - d.real());
	if (deno == 0) return false;
	double axby_aybx = a.real() * b.imag() - a.imag() * b.real();
	double cxdy_cydx = c.real() * d.imag() - c.imag() * d.real();
	ans.real(
		(axby_aybx * (c.real() - d.real()) - (a.real() - b.real()) * cxdy_cydx)
		/ deno
	);
	ans.imag(
		(axby_aybx * (c.imag() - d.imag()) - (a.imag() - b.imag()) * cxdy_cydx)
		/ deno
	);
	return true;
}

////////////////////////////////////////////////////////////////////////////////

bool is_inside(const Polygon &poly, const Point &query) {
	// 1. Compute bounding box and set coordinate of a point outside the polygon
	// TODO
	Point outside(right_most_x + 1, query.imag());
	// 2. Cast a ray from the query point to the 'outside' point, count number of intersections
	// TODO
	return true;
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
	// TODO
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char * argv[]) {
	if (argc <= 3) {
		std::cerr << "Usage: " << argv[0] << " points.xyz poly.obj result.xyz" << std::endl;
	}
	std::vector<Point> points = load_xyz(argv[1]);
	Polygon poly = load_obj(argv[2]);
	std::vector<Point> result;
	for (size_t i = 0; i < points.size(); ++i) {
		if (is_inside(poly, points[i])) {
			result.push_back(points[i]);
		}
	}
	save_xyz(argv[3], result);
	return 0;
}
