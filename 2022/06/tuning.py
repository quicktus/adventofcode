import os;l=open(os.path.join(os.getcwd(),"input.txt"),"r").read();x,v,s=len(l),14,"\nfirst {}age sequence found after {} chars";print(s.format("pack",[len(set(l[i-4:i]))for i in range(4,x)].index(4)+4),s.format("mess",[len(set(l[i-v:i]))for i in range(v,x)].index(14)+v))