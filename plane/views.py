from django.shortcuts import render
from django.http import HttpResponse

def plane_serve(request):
    context = {
        'latest_question_list': [1, 2, 4],
    }
    return render(request, 'plane.html', context)
