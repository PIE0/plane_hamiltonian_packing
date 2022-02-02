#pragma once
#include <vector>
#include <math.h>
#include <iomanip>
#include <map>
#include <algorithm>
using namespace std;

class Point { 
public:
  double x, y; 
  Point();
  Point(double x, double y);
  Point(const Point &p);
  Point operator + (const Point &p) const;
  Point operator - (const Point &p) const;
  Point operator * (double c) const;
  Point operator / (double c) const;
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
const double PIE = acos(-1);
const int INF_COORDINATE = (1<<15);
const double INF_DISTANCE = 1e18;

using sol_func = pair<Path, Path> (*)(vector<Point>, bool (*)(pair<int, int>, pair<int, int>));
ostream& operator<<(ostream &os, const Point &p);
ostream& operator <<(ostream& os, const pair <Point, Point> &p);
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
Point line_intersect(Point L11, Point L12, Point L21, Point L22);

int count_repeated_edge(Path p1, Path p2, bool circuit_break);
pair<int, int> check_cross_free_packing_paths(Path, Path, bool=true);

float get_angle_ABC(Point a, Point b, Point c);
vector <Point> find_convex_hull(vector <Point> points);