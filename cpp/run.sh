# Compile code with this command
g++ --std=c++17 main.cpp utils/graph_utils.cpp -pthread
# Example Runs
./a.out batch gen 10 5 20
./a.out single gen 20 300
./a.out batch run 12 good_tests good_tests sum rnd_rot four_perm
./a.out single run sum rnd_rot four_perm