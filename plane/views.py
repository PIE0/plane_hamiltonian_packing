from django.shortcuts import render
from django.http import HttpResponse
from plane.cpp_runner import run_cpp
import json

def plane_serve(request):
    context = {
        'latest_question_list': [1, 2, 4],
    }
    return render(request, 'plane.html', context)

def run(request):
    if request.method != 'POST':
        return
    data = json.loads(request.body.decode("utf-8"))
    algs = ['rnd_rot']
    cmp = 'sum'
    points = data['points']
    run_cpp(cmp, algs, points)
    return HttpResponse('{}', content_type="application/json")