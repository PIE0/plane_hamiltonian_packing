#include "../utils/graph_utils.h"
#include <algorithm>
#include <assert.h>
using namespace std;

namespace four_perm_algorithm {
	vector <vector <int>> v;
	tuple <int, int, int> answer;
	Path best_path1, best_path2;
}
using namespace four_perm_algorithm;

pair <vector <int>, vector <int>> make_paths() {
	vector <int> p1_inds, p2_inds;
	int ind = 0;
	for(auto u : v) {
		int p2_indexes[2][4] = {{1, 3, 0, 2}, {2, 0, 3, 1}};
		for(int i = 0; i < 4; i++) {
			p1_inds.push_back(ind + u[i]);
			p2_inds.push_back(ind + u[p2_indexes[(bool)u.back()][i]]);
		}
		ind += 4;
	}
	return {p1_inds, p2_inds};
}

void solve(int now, vector <Point> &points, eval_func &evaluation) {
	if(answer == make_tuple(0, 0, 0))
		return;
	if(now+4 > points.size()) {
		auto [p1_inds, p2_inds] = make_paths();
		Path path1 = Path(points, p1_inds), path2 = Path(points, p2_inds);
		tuple <int, int, int> new_answer = check_cross_free_packing_paths(path1, path2, false);
		if(get<0>(answer) == -1 or evaluation(new_answer) < evaluation(answer)) {
			answer = new_answer;
			best_path1=path1;
			best_path2=path2;
		}
		return;
	}
	vector <int> per = {0, 1, 2, 3};
	do {
		// cerr << now << ' ' << per << endl;
		auto go = [&](int path_case) {
			per.push_back(path_case);
			v.push_back(per);
			solve(now+4, points, evaluation);
			v.pop_back();
			per.pop_back();
		};
		go(0);
		go(1);
	} while(next_permutation(per.begin(), per.end()));
}

// For better result, number of points must be divisible by 4
// At most about 15 points
pair <Path, Path> four_backtrack_solve(vector <Point> points, eval_func evaluation) { 
	assert(points.size() <= 15);
	answer = make_tuple(-1, 10000000, 10000000);
	sort(points.begin(), points.end());
	solve(0, points, evaluation);
	int n_last = (points.size()/4)*4;
	for(; n_last < points.size(); n_last++) {
		best_path1.add(points[n_last]);
		best_path2.add(points[n_last]);
	}
	return {best_path1, best_path2};
}