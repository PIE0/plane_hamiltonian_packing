#pragma once
#include <vector>
#include <math.h>
#include <iomanip>
#include <map>
using namespace std;

class Point { 
public:
  double x, y; 
  Point();
  Point(double x, double y);
  Point(const Point &p);
  Point operator + (const Point &p) const;
  Point operator - (const Point &p) const;
  Point operator * (long long c) const;
  bool operator < (const Point &p) const;
  bool operator == (const Point &p) const;
  double operator * (Point q) const;
};

class Path {
public:
  vector <Point> points;
  Path();
  Path(vector <Point> points, vector <int> p_index);
  Path(vector <Point>);
  void add(Point p);
  int count_crossings(bool);
};

const double EPS = 1e-12;

using sol_func = pair<Path, Path> (*)(vector<Point>, bool (*)(pair<int, int>, pair<int, int>));
ostream& operator<<(ostream &os, const Point &p);
template <typename T>
ostream& operator<<(ostream &os, const vector <T> &v);
ostream& operator<<(ostream &os, const Path &p);
void points_output(vector <Point> points);
void path_output(Path path);

double dot(Point, Point);   
double dist2(Point, Point);
double cross(Point, Point);
Point RotateCCW90(Point p);
Point RotateCW90(Point p);
Point RotateCCW(Point p, double t);

bool lines_parallel(Point, Point, Point, Point);
bool lines_collinear(Point, Point, Point, Point);
bool SegmentsIntersect(Point, Point, Point, Point);

int count_repeated_edge(Path p1, Path p2, bool circuit_break);
pair<int, int> check_cross_free_packing_paths(Path, Path, bool=true);

vector <Point> sort_points_on_view(vector <Point> ps, Point p);