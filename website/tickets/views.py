from django.shortcuts import render
from django.http import HttpResponse
from django.views.generic import ListView
from .models import Ticket
from django_tables2 import SingleTableView
from .tables import RequestsTable
import json
from django.http import StreamingHttpResponse

class IndexListView(SingleTableView):
    model = Ticket
    table_class = RequestsTable
    template_name = 'tickets/index.html'

def about(request):
    context = {"about_page": "active"} # new info here
    return render(request, 'tickets/about.html', context)

def contact(request):
    context = {"contact_page": "active"} # new info here
    return render(request, 'tickets/contact.html', context)


def main_page(request):
    context = {"contact_page": "active"}
    if request.method=='GET':
        newTicket = Ticket(room = request.GET['room'], request = request.GET['request'], status = request.GET['status']) 
        newTicket.save()
    return render(request, 'tickets/contact.html', context)

