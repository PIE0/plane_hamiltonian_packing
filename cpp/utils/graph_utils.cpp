#include "graph_utils.h"
#include <iostream>
using namespace std;

int cnt_intersect_check = 0;
Point::Point() {}
Point::Point(double x, double y) : x(x), y(y) {}
Point::Point(const Point &p) : x(p.x), y(p.y) {}
Point Point::operator + (const Point &p) const { return Point(x+p.x, y+p.y); }
Point Point::operator - (const Point &p) const { return Point(x-p.x, y-p.y); }
Point Point::operator * (double c) const { return Point(x*c, y*c); }
Point Point::operator / (double c) const { return Point(x/c, y/c); }
bool Point::operator < (const Point &p) const { return x < p.x or (x == p.x and y < p.y); }
bool Point::operator == (const Point &p) const { return x == p.x and y == p.y; }
bool Point::operator != (const Point &p) const { return x != p.x or y != p.y; }
double Point::operator*(Point q) const { return x * q.y - y * q.x; }

ostream& operator<<(ostream &os, const Point &p) { return os << setprecision(2) << fixed << "(" << p.x << "," << p.y << ")"; }
template <typename T>
ostream& operator<<(ostream &os, const vector <T> &v) {
  for(auto u : v)
      os << u << ' ';
  return os;
}
template ostream& operator<< <int>(ostream &os, const vector <int> &v);
void points_output(vector <Point> points) {
  for(auto p : points)
    cout << p.x << ' ' << p.y << endl;
}
ostream& operator <<(ostream& os, const pair <Point, Point> &p) {
    return os << "{" << p.first << "," << p.second << "}";
}

Path::Path() {points.clear();}
Path::Path(vector <Point> points) : points(points) {}
Path::Path(vector <Point> points, vector <int> p_index) {
  for(auto ind : p_index)
    this->points.push_back(points[ind]);
}
void Path::add(Point p) {points.push_back(p);}

ostream& operator <<(ostream &os, const Path &p) {return os << p.points;}
void path_output(Path path) {
  points_output(path.points);
}

int Path::count_crossings(bool circuit_break=true) {
  int crossings = 0;
  auto find_intersect = [&](Point &a, Point &b, Point &c, Point &d) {
    // cerr << "Intersect: " << a << ' ' << b << ' ' << c << ' ' << d << endl;
    crossings++;
  };
  auto is_between = [](Point a, Point b, Point c) {
    return lines_parallel(a, b, b, c) and dist2(a, c) < max(dist2(a, b), dist2(b, c));
  };
  for(int i=0; i < points.size()-1; i++) {
      for(int j = i + 2; j < points.size()-1; j++) {
          cnt_intersect_check++;
          // if(cnt % 10000 == 0)
            // cerr << cnt << endl;
          if(SegmentsIntersect(points[i], points[i+1], points[j], points[j+1])) {
              find_intersect(points[i], points[i+1], points[j], points[j+1]);
              if(circuit_break)
                return 1;
          }
      }
    if(i < points.size()-2 and is_between(points[i], points[i+1], points[i+2])) {
      find_intersect(points[i], points[i+1], points[i+1], points[i+2]);
      if(circuit_break)
        return 1;
    }
  }
  return crossings;
}

double dot(Point p, Point q)     { return p.x*q.x+p.y*q.y; }
double dist2(Point p, Point q)   { return dot(p-q,p-q); }
double cross(Point p, Point q)   { return p.x*q.y-p.y*q.x; }
Point RotateCCW90(Point p)   { return Point(-p.y,p.x); }
Point RotateCW90(Point p)    { return Point(p.y,-p.x); }
Point RotateCCW(Point p, double t) {return Point(p.x*cos(t)-p.y*sin(t), p.x*sin(t)+p.y*cos(t)); }

// determine if lines from a to b and c to d are parallel or collinear
bool lines_parallel(Point a, Point b, Point c, Point d) { 
  return abs(cross(b-a, c-d)) == 0; 
}

bool lines_collinear(Point a, Point b, Point c, Point d) { 
  return lines_parallel(a, b, c, d) 
    && abs((a - b) * (a - c)) < EPS 
    && abs((c - d) * (c - a)) < EPS;
}

// determine if line segment from a to b intersects with 
// line segment from c to d
bool SegmentsIntersect(Point a, Point b, Point c, Point d) {
  if (lines_collinear(a, b, c, d)) {
    if (dist2(a, c) == 0 || dist2(a, d) == 0 || dist2(b, c) == 0 || dist2(b, d) == 0)
      return true;
    if (dot(c-a, c-b) > 0 && dot(d-a, d-b) > 0 && dot(c-b, d-b) > 0)
      return false;
    return true;
  }
  if (cross(d-a, b-a) * cross(c-a, b-a) > 0) return false;
  if (cross(a-c, d-c) * cross(b-c, d-c) > 0) return false;
  return true;
}

Point line_intersect(Point L11, Point L12, Point L21, Point L22) {
  if(lines_collinear(L11, L12, L21, L22)) {
    return L11;
  }
  double d1 = (L21-L11) * (L12-L11);
  double d2 = (L22-L11) * (L12-L11);
  return (L22 * d1 - L21 * d2) / (d1-d2);
}

int count_repeated_edge(Path p1, Path p2, bool circuit_break) {
  int cnt = 0;
  for(int i = 0; i < p1.points.size()-1; i++)
    for(int j = 0; j < p2.points.size()-1; j++)
      if((p1.points[i] == p2.points[j] and p1.points[i+1] == p2.points[j+1]) or 
                (p1.points[i] == p2.points[j+1] and p1.points[i+1] == p2.points[j])) {
        if(circuit_break) 
          return 1;
        cnt++;
      }
  return cnt;
}

int count_paths_intersections(Path p1, Path p2) {
  int crossings = 0;
  auto find_intersect = [&](Point &a, Point &b, Point &c, Point &d) {
    // cerr << "Two Paths Intersect: " << a << ' ' << b << ' ' << c << ' ' << d << endl;
    crossings++;
  };
  auto is_between = [](Point a, Point b, Point c) {
    return lines_parallel(a, b, b, c) and dist2(a, c) < max(dist2(a, b), dist2(b, c));
  };
  for(int i=0; i < p1.points.size()-1; i++) {
      for(int j = 0; j < p2.points.size()-1; j++) {
        Point &a = p1.points[i], &b = p1.points[i+1], &c = p2.points[j], &d = p2.points[j+1];
        if((a == c and b == d) or (a == d and b == c))
          continue;
        if(a == c or a == d or b == c or b == d) { // Check if two segments have not common subsegment
          if(not lines_parallel(a, b, c, d))  
            continue;
          Point common=a, other1=b, other2=c;
          if(a == c)
            other2 = d;
          else if (a != d) {
            common = b;
            other1 = a;
            if(b == c)
              other2 = d;
          }
          if(dist2(other1, other2) < max(dist2(common, other1), dist2(common, other2)))
            find_intersect(a, b, c, d);
          continue;
        }
        if(SegmentsIntersect(a, b, c, d))
            find_intersect(a, b, c, d);
      }
  }
  return crossings;
}

tuple<int, int, int> check_cross_free_packing_paths(Path p1, Path p2, bool circuit_break) {
  int repeated_edge = count_repeated_edge(p1, p2, circuit_break);
  if(circuit_break and repeated_edge != 0)
    return {0, 1, 0};
  int crossings1 = p1.count_crossings(circuit_break);
  if(circuit_break and crossings1 != 0)
    return {1, 0, 0};
  int crossings2 = p2.count_crossings(circuit_break);
  return {crossings1 + crossings2, repeated_edge, count_paths_intersections(p1, p2)};
}

float get_angle_ABC(Point a, Point b, Point c) {
    if(a == b or b == c or a == c)
      return 0;
    Point ab = { b.x - a.x, b.y - a.y };
    Point cb = { b.x - c.x, b.y - c.y };
    float dot = (ab.x * cb.x + ab.y * cb.y); // dot product
    float cross = (ab.x * cb.y - ab.y * cb.x); // cross product
    float alpha = fabs(atan2(cross, dot));
    return alpha * 180. / PIE;
}

vector <Point> find_convex_hull(vector <Point> points) {
  sort(points.begin(), points.end());
  vector<Point> up, dn;
  auto area2 = [](Point a, Point b, Point c) {
    return a*b + b*c + c*a;
  };
  for (int i = 0; i < points.size(); i++) {
    while (up.size() > 1 && area2(up[up.size()-2], up.back(), points[i]) >= 0) 
      up.pop_back();
    while (dn.size() > 1 && area2(dn[dn.size()-2], dn.back(), points[i]) <= 0)
      dn.pop_back();
    up.push_back(points[i]);
    dn.push_back(points[i]);
  }
  vector <Point> convex_hull=dn;
  for (int i = (int) up.size() - 2; i >= 1; i--)
    convex_hull.push_back(up[i]);
  return convex_hull;
}