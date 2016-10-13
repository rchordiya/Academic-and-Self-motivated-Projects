from Media import Media
from Book import Book
from Film import Film
from Periodic import Periodic
from Video import Video
class SearchEngine:
	global CardCatalog
	CardCatalog=list()
	def __init__(self):
		#CardCatalog=list()
		book_var = open('book.txt','r')
		line = book_var.readline()
		while line:
			bookObj=line.split('|')
			book_to_store=Book(bookObj[0],bookObj[1],bookObj[2],bookObj[3],bookObj[4],bookObj[5],bookObj[6],bookObj[7],bookObj[8],bookObj[9])
			CardCatalog.append(book_to_store)
			line = book_var.readline()			
		book_var.close()
		
		film_var2 = open('film.txt','r')
		line2 = film_var2.readline()
		while line2:
			filmObj=line2.split('|')
			film_to_store=Film(filmObj[0],filmObj[1],filmObj[2],filmObj[3],filmObj[4],filmObj[5])
			CardCatalog.append(film_to_store)
			line2 = film_var2.readline()			
		film_var2.close()
		
		periodic_var3 = open('periodic.txt','r')
		line3 = periodic_var3.readline()
		while line3:
			pObj=line3.split('|')
			periodic_to_store=Periodic(pObj[0],pObj[1],pObj[2],pObj[3],pObj[4],pObj[5],pObj[6],pObj[7],pObj[8],pObj[9],pObj[10],pObj[11])
			CardCatalog.append(periodic_to_store)
			line3 = periodic_var3.readline()			
		periodic_var3.close()
		
		video_var4 = open('video.txt','r')
		line4 = video_var4.readline()
		while line4:
			vObj=line4.split('|')
			video_to_store=Video(vObj[0],vObj[1],vObj[2],vObj[3],vObj[4],vObj[5],vObj[6],vObj[7])
			CardCatalog.append(video_to_store)
			line4 = video_var4.readline()			
		video_var4.close()	
	
	
		
	def search_by_call_number(self,srch_str):		
		size=len(CardCatalog)
		result=list()
		for letter in range(0,size):
			if (CardCatalog[letter].compare_call_number(srch_str)):
				result.append(CardCatalog[letter]);
		return result
		
   
   
    
	def search_by_title(self,srch_str):		
		size=len(CardCatalog)
		result=list()
		for letter in range(0,size):
			if (CardCatalog[letter].compare_title(srch_str)):
				result.append(CardCatalog[letter]);
		return result

	def search_by_subjects(self,srch_str):		
		size=len(CardCatalog)
		result=list()
		for letter in range(0,size):
			if (CardCatalog[letter].compare_subject(srch_str)):
				result.append(CardCatalog[letter]);
		return result
		
	def search_by_other(self,srch_str):		
		size=len(CardCatalog)
		result=list()
		for letter in range(0,size):
			if (CardCatalog[letter].compare_other(srch_str)):
				result.append(CardCatalog[letter]);
		return result
		
	
	
