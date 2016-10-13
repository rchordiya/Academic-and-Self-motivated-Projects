from Media import Media

class Film(Media):
	def __init__(self,a,b,c,d,e,f):
		Media.__init__(self,a,b,c,e)
		self.director=d
		self.year=f
		
		
	def display(self):
		print("<------------------------------ Record From Film --------------------------------------------->\n");
		print("Call Number: "+self.call_number+"\n");
		print("Title: "+self.title+"\n");
		print("Subjects: "+self.subject+"\n");
		print("Director: "+self.director+"\n");
		print("Notes: "+self.notes+"\n");
		print("Year: "+self.year+"\n");
		
	def compare_other(self,x):
		if (self.director.find(x) != -1):
			return True
		elif(self.year.find(x) !=-1):
			return True
		elif(self.notes.find(x) !=-1):
			return True
		else:
			return False
	
 
  
