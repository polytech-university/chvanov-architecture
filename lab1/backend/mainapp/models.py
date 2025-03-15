from django.db import models


class large_language_model(models.Model):
    model_name = models.CharField(null=True, max_length=100)
    opensource = models.BooleanField(null=True)
    reasoning = models.BooleanField(null=True)

    param_number_billions = models.FloatField(null=True)
    context_window = models.IntegerField(null=True)
    ndow = models.IntegerField(null=True)
    cost = models.FloatField(null=True)
    
    MMLU = models.FloatField(null=True)
    TruthfulQA = models.FloatField(null=True)
    Perplexity = models.FloatField(null=True)