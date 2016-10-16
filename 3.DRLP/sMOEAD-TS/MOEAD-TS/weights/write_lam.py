fin = open("weight_200_2.data",'w')
num = 200
for i in range(num):
	a1 = float(i)/(num -1)
	a2 = 1.0 - a1
	fin.write(str(a1)+' '+str(a2)+'\n')
fin.close()