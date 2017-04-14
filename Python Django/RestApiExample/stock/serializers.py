from rest_framework import serializers

from .models import Stockmarket

class StockmarketSerializer(serializers.ModelSerializer):
    class Meta:
        model= Stockmarket
        #fields=('ticker','volume')
        #to send all
        fields='__all__'

