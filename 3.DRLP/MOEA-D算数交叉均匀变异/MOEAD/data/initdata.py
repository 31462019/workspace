import random
room = 30
fin = open("example3.30_5_5",'w')
all_width = 0.0
fin.write("width\n")
for x in range(room):
	a = float(int(random.uniform(4,15)*10))/10
	all_width += a
	if x != room-1:
		fin.write(str(a)+' ')
	else:
		fin.write(str(a)+"\n")
fin.write("flow\n")
for x in xrange(room):
	for y in xrange(room):
		if random.random() >0.4:
			a = 0
		elif random.random() >0.7:
			a = random.randint(0,20000)
		else:
			a = random.randint(0,2000)
		fin.write(str(a) + " ")
	fin.write("\n")

fin.write("limit_row\n")
for x in xrange(3):
	a = float(int((all_width+9.5)/2*10))/10
	if x != 2:
		fin.write(str(a) + ' ')
	else:
		fin.write(str(a) + "\n")
fin.write("limit_wid\n")
fin.write("3 3.5 3\n")
fin.write("corridor\n3.5\n")
fin.write("height\n6\n")
fin.write("adj\n")

for x in xrange(room):
	for y in xrange(room):
		tmp = random.random()
		if(tmp<0.1):
			a = 0
		elif(tmp<0.25):
			a = 1
		elif(tmp<0.45):
			a = 2
		elif(tmp<0.65):
			a = 3
		elif(tmp<0.85):
			a = 4
		else:
			a = 5
		fin.write(str(a) + " ")
	fin.write("\n")
fin.close()
