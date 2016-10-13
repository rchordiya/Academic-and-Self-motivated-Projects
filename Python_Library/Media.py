class Media:
	def __init__(self,a,b,c,d):
		self.call_number=a
		self.title=b
		self.subject=c
		self.notes=d
	
	def compare_call_number(self,x):
		found=self.call_number.find(x)
		if(found==-1):
			return False
		else:
			return True
			
	def compare_title(self,x):
		found=self.title.find(x)
		if(found==-1):
			return False
		else:
			return True
			
	def compare_subject(self,x):
		found=self.subject.find(x)
		if(found==-1):
			return False
		else:
			return True
	def compare_other(self,x):
		pass
		
	def display():
		pass
	
	
