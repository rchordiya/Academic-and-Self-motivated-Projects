from Media import Media

class Periodic(Media):
	def __init__(self,a,b,c,d,e,f,g,h,i,j,k,l):
		Media.__init__(self,a,b,c,i)
		self.author=d
		self.description=e
		self.publisher=f
		self.publishing_history=g
		self.series=h
		self.related_titles=j
		self.other_forms_of_title=k
		self.govt_doc_number=l
		
	def display(self):
		print("<------------------------------ Record From Periodic --------------------------------------------->\n");
		print("Call Number: "+self.call_number+"\n");
		print("Title: "+self.title+"\n");
		print("Subjects: "+self.subject+"\n");
		print("Author: "+self.author+"\n");
		print("Description: "+self.description+"\n");
		print("Publisher: "+self.publisher+"\n");
		print("Publishing History: "+self.publishing_history+"\n");		
		print("Series: "+self.series+"\n");
		print("Notes: "+self.notes+"\n");
		print("Related Titles: "+self.related_titles+"\n");
		print("Other forms of title: "+self.other_forms_of_title+"\n")
		print("Govt Doc Number: "+self.govt_doc_number+"\n")
		
	def compare_other(self,x):
		if (self.description.find(x) != -1):
			return True
		elif(self.series.find(x) !=-1):
			return True
		elif(self.notes.find(x) !=-1):
			return True
		elif(self.related_titles.find(x) !=-1):
			return True
		else:
			return False
	
 
  
