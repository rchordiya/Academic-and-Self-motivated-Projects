from django.shortcuts import get_object_or_404
from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework import status
from .models import Stockmarket
from .serializers import StockmarketSerializer


# Create your views here.
#stocks/AMZN
class StockList(APIView):

    def get(self,request): #return all the data from database
        stocks=Stockmarket.objects.all()
        #serialize them i.e convert them into JSON
        serializer=StockmarketSerializer(stocks,many=True)
        return Response(serializer.data)

    def post(self): # let create a new stock
        pass
