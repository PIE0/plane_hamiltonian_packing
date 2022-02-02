#include "graph_utils.h"
#include <assert.h>
#include "utils.cpp"
using namespace std;

Point get_slope(Point s) {
  assert((int)s.x == s.x and (int)s.y == s.y);
  int g = int_gcd(abs(s.x), abs(s.y));
  if(s.x < 0 and s.y < 0)
    g *= -1;
  s.x /= g; s.y /= g;
  if((s.x < 0 and s.y > 0) or (s.x==0 and s.y<0)) {
    s.x *= -1;
    s.y *= -1;
  }
  return s;
}

bool check_colinear_points(vector <Point> points) {
  for(int i = 0; i < points.size(); i++) {
    map <Point, int> mark;
    for(int j = 0; j < points.size(); j++) {
      if(i == j)
        continue;
      Point shib = get_slope(points[i]-points[j]);
      if(mark.find(shib) != mark.end())
        if(lines_parallel(points[mark[shib]], points[i], points[j], points[i])) {
          cerr << "Found three colinear points" << points[i] << ' ' << points[j] << ' ' << points[mark[shib]] << endl;
          return true;
        } else 
          assert(false);
      mark[shib] = j;
    }
  }
  return false;
}

vector <Point> read_input(string input_file, bool check_colinear) {
  freopen(input_file.c_str(), "r", stdin);
  int n;
  cin >> n;
  vector <Point> points(n, Point());
	for(int i = 0; i < n; i++) {
		cin >> points[i].x >> points[i].y;
    assert(abs(points[i].x) < INF_COORDINATE);
    assert(abs(points[i].y) < INF_COORDINATE);
  }
  if(check_colinear)
    check_colinear_points(points);
  return points;
}