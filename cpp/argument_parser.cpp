#include <string>
#include <vector>
#include <assert.h>
using namespace std;

struct Arguments {
  bool batch, gen_test;
  int test_count, l, r;
  vector <string> algos;
  string input_folder, output_folder, compare_method;
  string input_file="input.txt";
  string output_file="output.txt";
};

Arguments parse_args(vector <string> args_str) {
  assert(args_str[0] == "batch" or args_str[0] == "single");
  assert(args_str[1] == "gen" or args_str[1] == "-");
  Arguments args;
  args.batch = (args_str[0]=="batch");
  args.gen_test = (args_str[1]=="gen");
  if(args.gen_test) {
    args.test_count = (args.batch ? stoi(args_str[2]) : 1);
    args.l = stoi(args_str[2+args.batch]);
    args.r = stoi(args_str[3+args.batch]);
  } else {
    args.test_count = (args.batch ? stoi(args_str[2]) : 1);
    args.input_folder = (args.batch ? args_str[3] : "-");
    args.output_folder = (args.batch ? args_str[4] : "-");

    int next_one = (args.batch ? 5 : 2);
    args.compare_method = args_str[next_one++];
    while(next_one < args_str.size())
      args.algos.push_back(args_str[next_one++]);
  }
  return args;
}