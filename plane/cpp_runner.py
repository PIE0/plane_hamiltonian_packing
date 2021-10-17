import os

PROJECT_PATH = os.path.abspath(os.path.dirname(__name__))



def run_cpp(cmp, algs, points):
    cpp_path = os.path.join(PROJECT_PATH, 'cpp')
    input_path = os.path.join(cpp_path, 'input.txt')
    with open(input_path.__str__(), 'w') as f:
        f.write(str(len(points)) + '\n')
        for p in points:
            f.write(str(p[0]) + ' ' + str(p[1]) + '\n')
    os.chdir(cpp_path)
    os.system('g++ --std=c++17 graph_utils.cpp main.cpp single - {} {}'.format(cmp, ' '.join(algs)))
    
    output_path = os.path.join(cpp_path, 'output.txt')
    with open(output_path.__str__(), 'r') as f:
        line = f.readline()
        points
    # os.popen('echo Returned output')