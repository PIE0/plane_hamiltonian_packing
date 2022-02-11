import json

from cpp_config import ALGORITHMS, EVALUATIONS
from django.http import HttpResponse, JsonResponse
from django.shortcuts import render

from plane.cpp_runner import run_cpp


def plane_serve(request):
    context = {
        'latest_question_list': [1, 2, 4],
    }
    return render(request, 'plane.html', context)

def run(request):
    if request.method != 'POST':
        return
    data = json.loads(request.body.decode("utf-8"))
    algorithm = [data.get('algorithm') or 'zigzag'] # FIXME
    points = data['points']
    weights = data['weights']
    # print("input: ", points)
    ret = run_cpp(algorithm, points, weights)
    # print("output: ", ret)
    return JsonResponse(ret)
