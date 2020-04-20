from django.db import models
from django.utils import timezone
import datetime
from django.utils.timezone import now


# Create your models here.

class Ticket(models.Model):
    room = models.CharField(max_length=100, verbose_name="Room Number")
    request = models.CharField(max_length=100, verbose_name="Request Type", choices=[('Supplies - Whiteboard', 'Supplies - Whiteboard'), 
    	('Supplies - Blackboard', 'Supplies - Blackboard'), ('Temperature - Too Hot', 'Temperature - Too Hot'),
    	('Temperature - Too Cold', 'Temperature - Too Cold'), ('General', 'General'),])
    pub_date = models.DateTimeField(default=now, blank=True, verbose_name="time")
    status = models.CharField(max_length=100, verbose_name="Status", choices=[('Not Started', 'Not Started'), ('In Progress', 'In Progress'),
    	('Completed', 'Completed')])

