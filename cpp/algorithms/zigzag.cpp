//be name khoda :)
#include "../utils/graph_utils.h"
using namespace std;

pair <Point, Point> find_left_bridge(vector <Point> L, vector <Point> R, map <Point, bool> &is_left,
                                     Point center, Point seperation_line_end) {
  vector <Point> all(L.begin(), L.end());
  all.insert(all.end(), R.begin(), R.end());
  vector <Point> convex_hull = find_convex_hull(all);
  cerr << "Convex hull: " << convex_hull << endl;
  pair <Point, Point> left_bridge;
  double left_bridge_dist=INF_DISTANCE;
  for(int i = 0; i < convex_hull.size(); i++) {
    Point &p1 = convex_hull[i], &p2 = convex_hull[(i+1)%convex_hull.size()];
    if(is_left[p1] != is_left[p2]) {
      Point intersect = line_intersect(p1, p2, center, seperation_line_end);
      cerr << "Candid Bridge: " << p1 << " ----- " << p2 << " : " << intersect << endl;
      double dis = dist2(intersect, center);
      if(dis < left_bridge_dist) {
        left_bridge_dist = dis;
        left_bridge = {p1, p2};
      }
    }
  }
  assert(left_bridge_dist != INF_DISTANCE);
  return left_bridge;
}

Path find_zigzag_path(vector <Point> points) {
  cerr << "All: " << points << endl;
  Path best_ph1(points), best_ph2(points);
  sort(points.begin(), points.end(), [](Point a, Point b){return a.y < b.y or (a.y == b.y and a.x < b.x);});
  Point center = points[0];
  points.erase(points.begin());
  Point INF_POINT(-INF_COORDINATE, center.y);
  sort(points.begin(), points.end(), [&](Point a, Point b){
    return get_angle_ABC(a, center, INF_POINT) < get_angle_ABC(b, center, INF_POINT) or 
      (get_angle_ABC(a, center, INF_POINT) == get_angle_ABC(b, center, INF_POINT) and 
        dist2(center, a) < dist2(center, b));
    });
  vector <Point> L(points.begin(), points.begin()+points.size()/2), R(points.begin()+points.size()/2, points.end());
  cerr << "Center: " << center << endl;
  cerr << "L: " << L << endl;
  cerr << "R: " << R << endl;

  map <Point, bool> is_left;
  for(auto p : L)
    is_left[p] = true;
  Point seperation_line_end = center + (L.back() - center)*INF_COORDINATE;
  Path result;
  result.add(center);
  while(L.size() or R.size()) {
    if(L.size() == 0) {
      result.add(R[0]);
      R.erase(R.begin());
      continue;
    } else if(R.size() == 0) {
      result.add(L[0]);
      L.erase(L.begin());
      continue;
    }
    auto [p1, p2] = find_left_bridge(L, R, is_left, center, seperation_line_end);
    if(!is_left[p1])
      swap(p1, p2);
    if(result.points.back() == center or is_left[result.points.back()]) {
      R.erase(find(R.begin(), R.end(), p2));
      result.add(p2);
    } else {
      L.erase(find(L.begin(), L.end(), p1));
      result.add(p1);
    }
  }
  cerr << "result: " << result << endl;
  return result;
}

pair <Path, Path> find_two_disjoint_plane_paths(vector <Point> points, cmp_func compare) {
  return {find_zigzag_path(points), Path(points)};
}