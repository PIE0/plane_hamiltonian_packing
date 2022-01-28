//be name khoda :)
#include "utils/graph_utils.h"
#include <iostream>
#include <assert.h>
using namespace std;

const int N = 47;
const int K = 100;

void generate(int l, int r, int number_of_tests) {
  for(int test_num=1; test_num <= number_of_tests; test_num++) {
    cerr << "Generating " << test_num << "th test..." << endl;
    string file_name = "gen_tests/input"+to_string(test_num)+".txt";
    freopen(file_name.c_str(), "w", stdout);
    assert(l >= 4);
    assert(r >= l);
    int n = distr(eng) % (r-l+1) + l;
    vector <Point> points;
    for(int i = 0; i < n; i++) 
      points.push_back(Point(distr(eng) % K - K/2, distr(eng)%K - K/2));
    cout << n << endl;
    assert(points.size() == n);
    points_output(points);
  }
}