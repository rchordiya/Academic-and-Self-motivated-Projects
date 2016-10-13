from Media import Media

class Video(Media):
	def __init__(self,a,b,c,d,e,f,g,h):
		Media.__init__(self,a,b,c,f)
		self.description=d
		self.distributor=e
		self.series=g
		self.label=h
		
		
	def display(self):
		print("<------------------------------ Record From Video --------------------------------------------->\n");
		print("Call Number: "+self.call_number+"\n");
		print("Title: "+self.title+"\n");
		print("Subjects: "+self.subject+"\n");
		print("Description: "+self.description+"\n");
		print("Distributor: "+self.distributor+"\n");
		print("Notes: "+self.notes+"\n");
		print("Series: "+self.series+"\n");
		print("Label: "+self.label+"\n");
		
	def compare_other(self,x):
		if (self.description.find(x) != -1):
			return True
		elif(self.distributor.find(x) !=-1):
			return True
		elif(self.notes.find(x) !=-1):
			return True
		else:
			return False
	
 
  
