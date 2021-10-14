//be name khoda :)
#include "graph_utils.h"
#include <algorithm>
using namespace std;

const double PRECISION = 30000;
const int ITERATIONS = 10000;
int cnt_sort_compares;

Path rotate_points(vector <Point> &points, double t) {
  vector <int> p_index;
  for(int i = 0; i < points.size(); i++)
    p_index.push_back(i);
  sort(p_index.begin(), p_index.end(), [&](int i, int j) {
    cnt_sort_compares++;
    Point c = RotateCCW(points[i], t);
    Point d = RotateCCW(points[j], t);
    return c.x < d.x;
  });
  return Path(points, p_index);
}

pair <Path, Path> solve_rot_sort(vector <Point> points, cmp_func compare) {
  Path best_ph1, best_ph2;
  pair <int, int> best_answer = {100000000, 10000000};

  int iter = ITERATIONS;
  while(iter--) {
    auto random_angle = [](){return distr(eng) % (int)(2*PRECISION) / PRECISION * M_PI;};
    Path ph1 = rotate_points(points, random_angle());
    Path ph2 = rotate_points(points, random_angle());
    pair <int, int> check_answer = check_cross_free_packing_paths(ph1, ph2, false);
    if(compare(check_answer, best_answer))
      best_ph1 = ph1, best_ph2 = ph2, best_answer = check_answer;
    if(best_answer == pair<int,int>{0, 0})
      break;
  }
  return {best_ph1, best_ph2};
}

/*
12
0 0
1 0
0 1
1 1

2 3
3 2
4 2
-1 4

-100 250
-98 -231
245 153
213 -138
*/