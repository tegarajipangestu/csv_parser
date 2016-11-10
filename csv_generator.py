import csv
import random
from faker import Faker
from datetime import datetime


l=Faker('en_GB')
g=open("1.csv","w")
w=csv.writer(g)
w.writerow(('id','name','address','college','company','dob','age'))
for i in range(20):
    w.writerow((i+1,"Some name","Some address",random.choice(['psg','sona','amirta','anna university']),random.choice(['CTS','INFY','HTC']),(random.randrange(1950,1995,1),random.randrange(1,13,1),random.randrange(1,32,1)),random.choice(range(0,100))))
