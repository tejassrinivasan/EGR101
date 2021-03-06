# Generated by Django 3.0.4 on 2020-04-20 19:04

from django.db import migrations, models
import django.utils.timezone


class Migration(migrations.Migration):

    dependencies = [
        ('tickets', '0003_info_pub_date'),
    ]

    operations = [
        migrations.CreateModel(
            name='Ticket',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('room', models.CharField(max_length=100, verbose_name='Room Number')),
                ('request', models.CharField(choices=[('Supplies - Whiteboard', 'Supplies - Whiteboard'), ('Supplies - Blackboard', 'Supplies - Blackboard'), ('Temperature - Too Hot', 'Temperature - Too Hot'), ('Temperature - Too Cold', 'Temperature - Too Cold'), ('General', 'General')], max_length=100, verbose_name='Request Type')),
                ('pub_date', models.DateTimeField(blank=True, default=django.utils.timezone.now, verbose_name='time')),
                ('status', models.CharField(choices=[('Not Started', 'Not Started'), ('In Progress', 'In Progress'), ('Completed', 'Completed')], max_length=100, verbose_name='Status')),
            ],
        ),
        migrations.DeleteModel(
            name='Info',
        ),
    ]
