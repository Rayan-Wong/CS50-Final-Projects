# Generated by Django 4.2.3 on 2023-07-29 12:59

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('blog', '0002_user_background_color'),
    ]

    operations = [
        migrations.AlterField(
            model_name='user',
            name='background_color',
            field=models.CharField(blank=True, max_length=50, null=True),
        ),
    ]