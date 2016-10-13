from Media import Media

class Book(Media):
	def __init__(self,a,b,c,d,e,f,g,h,i,j):
		Media.__init__(self,a,b,c,j)
		self.author=d
		self.description=e
		self.publisher=f
		self.city=g
		self.year=h
		self.series=i
		
	def display(self):
		print("<------------------------------ Record From Book --------------------------------------------->\n");
		print("Call Number: "+self.call_number+"\n");
		print("Title: "+self.title+"\n");
		print("Subjects: "+self.subject+"\n");
		print("Author: "+self.author+"\n");
		print("Description: "+self.description+"\n");
		print("Publisher: "+self.publisher+"\n");
		print("City: "+self.city+"\n");
		print("Year: "+self.year+"\n");
		print("Series: "+self.series+"\n");
		print("Notes: "+self.notes+"\n");
		
	def compare_other(self,x):
		if (self.description.find(x) != -1):
			return True
		elif(self.year.find(x) !=-1):
			return True
		elif(self.notes.find(x) !=-1):
			return True
		else:
			return False
	
 
  
