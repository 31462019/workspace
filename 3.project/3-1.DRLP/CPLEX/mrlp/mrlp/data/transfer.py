fin = open('example3.10_2_2','r')
fout = open('cplex3.10_2_2','w')
for i in fin:
	tmp = i
	tmp = tmp.strip()
	line = tmp.replace(' ',',')
	fout.write(line+'\n')
fin.close()
fout.close()