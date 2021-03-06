import logging
import os
import subprocess

PROJECT_PATH = os.path.abspath(os.path.dirname(__name__))
logger = logging.getLogger(__name__)

def extract_path(f, point_count):
    points = []
    for _ in range(point_count):
        points.append(list(map(int, f.readline().split())))
    return points

def run_cpp(algs, points, weights):
    # Write to input.txt
    cpp_path = os.path.join(PROJECT_PATH, 'cpp')
    input_path = os.path.join(cpp_path, 'input.txt')
    with open(input_path.__str__(), 'w') as f:
        f.write(str(len(points)) + '\n')
        for p in points:
            f.write(str(p[0]) + ' ' + str(p[1]) + '\n')
    # Run cpp code
    os.chdir(cpp_path)
    print(weights)
    try:
        subprocess.run('./a.out single run {} {} {} {}'.format(*weights, ' '.join(algs)).split(), timeout=30)
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
            if(len(answer) == 4):
                ret[alg] = [path1, path2]
                ret['fitness'] = answer
    return ret
