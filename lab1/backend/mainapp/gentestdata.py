import random
from .models import *
from django.db import transaction
from faker import Faker


fk = Faker()

def gentestdata():
    with transaction.atomic():
        for _ in range(1000):
            llm = large_language_model()
            llm.model_name = random.choice(("GPT-4", "BERT", "T5", "RoBERTa", "XLNet", "ERNIE", "Megatron", "EleutherAI" "GPT-Neo", "OpenAI" "Codex", "LaMDA"))
            llm.opensource = fk.boolean(chance_of_getting_true=25)
            llm.reasoning = fk.boolean(chance_of_getting_true=5)
            
            llm.param_number_billions = round(fk.random.random() * 1000)
            llm.context_window = random.choice((10_000, 64_000, 120_000, 200_000, 1_000_000))
            llm.cost = round(fk.random.random() * 10, 3)

            llm.MMLU = round(min(max(random.gauss(86, 10), 0), 100), 2)
            llm.TruthfulQA = round(min(max(random.gauss(86, 10), 0), 100), 2)
            llm.Perplexity = round(min(max(random.gauss(86, 10), 0), 100), 2)
            
            llm.save()

    print('Successfully generated data')