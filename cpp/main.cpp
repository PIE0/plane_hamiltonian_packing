// be name khoda
#include "utils/graph_utils.h"
#include "utils/utils.cpp"
#include "utils/get_graph_input.cpp"
#include "algorithms/rnd_rot.cpp"
#include "algorithms/four_perm.cpp"
#include "algorithms/zigzag.cpp"
#include "algorithms/genetic/genetic.cpp"
#include "test_gen.cpp"
#include "argument_parser.cpp"
#include <assert.h>
#include <map>
using namespace std;

map <string, sol_func> ALGORITHMS = {
  {"rnd_rot", solve_rot_sort},
  {"four_perm", four_backtrack_solve},
  {"zigzag", find_two_disjoint_plane_paths},
  {"genetic", run_genetic},
};

map <string, eval_func> COMPARES = {
  {"sum", [](tuple <int, int, int> a) {
    return get<0>(a) + get<1>(a) + get<2>(a);
  }},
  {"weighted_sum", [](tuple <int, int, int> a) {
    return 5*get<0>(a) + 10*get<1>(a) + get<2>(a);
  }},
};

void write(string output_file, Path p1, Path p2, tuple <int, int, int> answer, int fitness) {
  path_output(p1);
  path_output(p2);
  cout << get<0>(answer) << " " << get<1>(answer) << " " << get<2>(answer) << ' ' << fitness << endl;
}

void run_sols(string input_file, string output_file, vector <pair<string, sol_func>> &sols, eval_func &evaluation) {
  vector <Point> points=read_input(input_file, false);
  freopen(output_file.c_str(), "w", stdout);
  map <Point, int> count;
  for(auto point : points)
    count[point] = 1;
  cerr << "Number of points: " << points.size() << endl;
  for(auto sol : sols) {
    auto [p1, p2] = sol.second(points, evaluation);
    for(auto point : p1.points)
      count[point]++;
    for(auto point : p2.points)
      count[point]++;
    auto answer = check_cross_free_packing_paths(p1, p2, false);
    cerr << sol.first << "'s answer: " << answer << " --> " << evaluation(answer) << endl;
    write(output_file, p1, p2, answer, evaluation(answer));
  }
  for(auto [k, v] : count) {
    // if(v != 2*sols.size()+1)
    //   cerr << k << ' ' << v << endl;
    assert(v == 2*sols.size()+1);
  }
  cerr << "END OF SOLUTION RUNS------------------" << endl;
}

// sample args: batch gen 10 5 20
// sample args: single gen 20 300
// sample args: batch run 10 input_folder output_folder sum rnd_rot four_perm
// sample args: single run sum rnd_rot four_perm
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
    vector <pair<string, sol_func>> sols;
    for(auto algo : args.algos) {
      assert(ALGORITHMS.find(algo) != ALGORITHMS.end());
      sols.push_back({algo, ALGORITHMS[algo]});
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