import threading
import time
def square(numbers):
    print("Calculate square numbers")
    for n in numbers:
        time.sleep(0.2)
        print('square',n*n)
def cube(numbers):
    print("Calculate cube numbers")
    for n in numbers:
        time.sleep(0.2)
        print('cube',n*n*n)
numbers=[1,2,3,4,5]

t=time.time()
t1=threading.Thread(target=square,args=(numbers,))
t2=threading.Thread(target=cube,args=(numbers,))

t1.start()
t2.start()

t1.join()
t2.join()
print(time.time()-t)
