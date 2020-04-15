from django.shortcuts import render
from django.http import HttpResponse
from django.views.generic import ListView
from .models import Info
from django_tables2 import SingleTableView
from .tables import RequestsTable
import json
from django.http import StreamingHttpResponse

class IndexListView(SingleTableView):
    model = Info
    table_class = RequestsTable
    template_name = 'tickets/index.html'

def about(request):
    context = {"about_page": "active"} # new info here
    return render(request, 'tickets/about.html', context)

def contact(request):
    context = {"contact_page": "active"} # new info here
    return render(request, 'tickets/contact.html', context)


def main_page(request):
    if request.method=='POST':
            received_json_data=json.loads(request.body)
            return StreamingHttpResponse('it was post request: '+str(received_json_data))
    return StreamingHttpResponse('it was GET request')

