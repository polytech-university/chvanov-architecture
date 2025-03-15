from django.shortcuts import render

from django.conf import settings

import rest_framework
from rest_framework import viewsets
from .models import large_language_model
from .serializers import large_language_model_Serializer


class large_language_model_ViewSet(viewsets.ModelViewSet):
    queryset = large_language_model.objects.all()
    serializer_class = large_language_model_Serializer
