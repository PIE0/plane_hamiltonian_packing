# Compile code with this command
g++ --std=c++17 main.cpp graph_utils.cpp
# Example Runs
./a.out batch gen 10 5 20
./a.out single gen 20 300
./a.out batch - 12 good_tests good_tests sum rnd_rot four_perm
./a.out single - sum rnd_rot four_perm