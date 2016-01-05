import os

tsDir="/home/hadoop/comparison/taskset1/"
fileList=os.listdir(tsDir)
oDir="/home/hadoop/comparison/run/log/"
#fileList.remove(".")
#fileList.remove("..")

exe="./run"

fout=open("go.sh", "w")

for fname in fileList:
    ofname=fname+".log"
    cmd=exe+" "+tsDir+fname+" "+oDir+ofname+"\n"
    fout.write(cmd)


fout.flush()
fout.close()
