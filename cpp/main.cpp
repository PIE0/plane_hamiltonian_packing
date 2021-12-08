// be name khoda
#include "utils/graph_utils.h"
#include "utils/utils.cpp"
#include "utils/get_graph_input.cpp"
#include "algorithms/rnd_rot.cpp"
#include "algorithms/four_perm.cpp"
#include "test_gen.cpp"
#include "argument_parser.cpp"
#include <assert.h>
#include <map>
using namespace std;

map <string, sol_func> ALGORITHMS = {
  {"rnd_rot", solve_rot_sort},
  {"four_perm", four_backtrack_solve},
};

map <string, cmp_func> COMPARES = {
  {"sum", [](pair <int, int> a, pair <int, int> b) {
    return a.first + a.second < b.first + b.second;
  }}
};

void write(string output_file, Path p1, Path p2) {
  path_output(p1);
  path_output(p2);
  auto answer = check_cross_free_packing_paths(p1, p2, false);
  cout << answer.first << ' ' << answer.second << endl;
}

void run_sols(string input_file, string output_file, vector <sol_func> &sols, cmp_func &cmp) {
  vector <Point> points=read_input(input_file, false);
  freopen(output_file.c_str(), "w", stdout);
  for(auto sol : sols) {
    auto [p1, p2] = sol(points, cmp);
    write(output_file, p1, p2);
  }
}

// sample args: batch gen 10 5 20
// sample args: single gen 20 300
// sample args: batch - 10 input_folder output_folder sum rnd_rot four_perm
// sample args: single - sum rnd_rot four_perm
// l and r must be at least 4
int main(int argc, char *argv[]) {
  assert(argc >= 5); // not enough arguments to run
  vector <string> args_str;
  for(int i = 1; i < argc; i++)
    args_str.push_back(string(argv[i]));
  auto args = parse_args(args_str);
  if(args.gen_test)
    generate(args.l, args.r, args.test_count);
  else {
    assert(COMPARES.find(args.compare_method) != COMPARES.end());
    auto cmp = COMPARES[args.compare_method];
    vector <sol_func> sols;
    for(auto algo : args.algos) {
      assert(ALGORITHMS.find(algo) != ALGORITHMS.end());
      sols.push_back(ALGORITHMS[algo]);
    }
    if(args.batch) {
      for(int i = 1; i <= args.test_count; i++) {
        cerr << "Running " << i << "th test..." << endl;
        string input_file  = args.input_folder+"/input"+to_string(i)+".txt";
        string output_file = args.output_folder+"/output"+to_string(i)+".txt";
        run_sols(input_file, output_file, sols, cmp);
      }
    } else
      run_sols(args.input_file, args.output_file, sols, cmp);
  }
  cerr << "\nTime to run: " << 1000 * clock() / CLOCKS_PER_SEC << endl;
}