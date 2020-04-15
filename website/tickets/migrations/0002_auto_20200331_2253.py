# Generated by Django 3.0.4 on 2020-04-01 05:53

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('tickets', '0001_initial'),
    ]

    operations = [
        migrations.CreateModel(
            name='Info',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('room', models.CharField(max_length=100, verbose_name='Room Number')),
                ('request', models.CharField(max_length=100, verbose_name='Request Type')),
                ('status', models.CharField(max_length=100, verbose_name='Status')),
            ],
        ),
        migrations.DeleteModel(
            name='Room',
        ),
    ]
