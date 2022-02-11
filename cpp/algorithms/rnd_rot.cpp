#include "../utils/graph_utils.h"
#include <algorithm>
using namespace std;

namespace rnd_rot_algorithm {
  static const double PRECISION = 30000;
  static const int ITERATIONS = 10000;
  static int cnt_sort_compares;
}
using namespace rnd_rot_algorithm;

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

pair <Path, Path> solve_rot_sort(vector <Point> points, eval_func evaluation) {
  Path best_ph1, best_ph2;
  tuple <int, int, int> best_answer = make_tuple(-1, 1000000, 1000000);

  int iter = ITERATIONS;
  while(iter--) {
    auto random_angle = [](){return distr(eng) % (int)(2*PRECISION) / PRECISION * M_PI;};
    Path ph1 = rotate_points(points, random_angle());
    Path ph2 = rotate_points(points, random_angle());
    tuple <int, int, int> check_answer = check_cross_free_packing_paths(ph1, ph2, false);
    if(get<0>(best_answer) == -1 or evaluation(check_answer) < evaluation(best_answer))
      best_ph1 = ph1, best_ph2 = ph2, best_answer = check_answer;
    if(best_answer == make_tuple(0, 0, 0))
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