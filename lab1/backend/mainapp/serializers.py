from rest_framework import serializers


from .models import large_language_model


class large_language_model_Serializer(serializers.ModelSerializer):
   
    class Meta:
        model = large_language_model
        fields = '__all__'