import os

tsDir="/home/hadoop/comparison/taskset1/"
fileList=os.listdir(tsDir)
oDir="/home/hadoop/comparison/pf/output/"
lDir="/home/hadoop/comparison/pf/log/"
#fileList.remove(".")
#fileList.remove("..")

exe="./pfAlgo"

fout=open("go.sh", "w")

for fname in fileList:
    ofname=fname+".out"
    lfname=fname+".log"
    cmd=exe+" "+tsDir+fname+" "+oDir+ofname+" "+lDir+lfname+"\n"
    fout.write(cmd)


fout.flush()
fout.close()
