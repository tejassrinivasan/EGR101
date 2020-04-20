from django.contrib import admin

from .models import Ticket


class InfoAdmin(admin.ModelAdmin):
    fieldsets = [
        (None,               {'fields': ['room']}),
        (None,               {'fields': ['request']}),
        (None,               {'fields': ['status']}),
        ('Date information', {'fields': ['pub_date'], 'classes': ['collapse']}),
    ]
    list_display = ('room', 'request', 'status')
    list_filter = ['pub_date']
    search_fields = ['request']

admin.site.register(Ticket, InfoAdmin)