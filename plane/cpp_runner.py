import os
import logging
import subprocess

PROJECT_PATH = os.path.abspath(os.path.dirname(__name__))
logger = logging.getLogger(__name__)

def extract_path(f, point_count):
    points = []
    for _ in range(point_count):
        points.append(list(map(int, f.readline().split())))
    return points

def run_cpp(cmp, algs, points):
    # Write to input.txt
    cpp_path = os.path.join(PROJECT_PATH, 'cpp')
    input_path = os.path.join(cpp_path, 'input.txt')
    with open(input_path.__str__(), 'w') as f:
        f.write(str(len(points)) + '\n')
        for p in points:
            f.write(str(p[0]) + ' ' + str(p[1]) + '\n')
    # Run cpp code
    os.chdir(cpp_path)
    try:
        subprocess.run('./a.out single - {} {}'.format(cmp, ' '.join(algs)).split(), timeout=3)
    except Exception as e:
        logger.error(e)
    # Read return value
    output_path = os.path.join(cpp_path, 'output.txt')
    ret = {}
    with open(output_path.__str__(), 'r') as f:
        for alg in algs:
            path1 = extract_path(f, len(points))
            path2 = extract_path(f, len(points))
            answer = list(map(int, f.readline().split()))
            if(len(answer) == 2):
                ret[alg] = [path1, path2, answer]
    return ret