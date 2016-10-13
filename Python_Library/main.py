from SearchEngine import SearchEngine
from Media import Media

def main():
	track=True
	results = list()
	search_obj=SearchEngine()
	while track:
		print("Enter your choice\n");
		print("-------------------------------------\n");
		print("1. Search by call number\n");
		print("2. Search by title\n");
		print("3. Search by subject\n");
		print("4. Search Other\n");
		print("5.To exit\n");
		print("---------------------------------------\n");
		results.clear()
		n=int(input())
		if n==1:
			print("Enter the call number\n");
			srch_str=input()
			results=search_obj.search_by_call_number(srch_str)
			for letter in results:
				letter.display()
				
			                
         
		elif n==2:
			print("Enter the title\n");
			srch_str=input()
			results=search_obj.search_by_title(srch_str)
			for letter in results:
				letter.display()
				
			
         
		elif n==3:
			print("Enter the subject\n");
			srch_str=input()
			results=search_obj.search_by_subjects(srch_str)
			for letter in results:
				letter.display()
				
			
         
		elif n==4:
			print("Enter to search by other\n");
			srch_str=input()
			results=search_obj.search_by_other(srch_str)
			for letter in results:
				letter.display()
				
			
		else :   		
			track=False				   		
		print("---------------------------------------------------------");      
		print("Total Number of reports found ----------> ",len(results))
		print("\n")
		   	   

if __name__ == "__main__": main()
