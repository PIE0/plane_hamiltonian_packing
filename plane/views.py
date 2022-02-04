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
    points = data['points']
    algorithm = [data.get('algorithm') or 'zigzag'] # FIXME
    evaluation = data.get('evaluation') or 'weighted_sum' # FIXME
    # print("input: ", points)
    ret = run_cpp(evaluation, algorithm, points)
    print("output: ", ret)
    return JsonResponse(ret)
