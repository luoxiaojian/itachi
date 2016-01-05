import os

tsDir="/home/hadoop/comparison/tasksets/"
fileList=os.listdir(tsDir)
oDir="/home/hadoop/comparison/bf/output/"
lDir="/home/hadoop/comparison/bf/log/"
#fileList.remove(".")
#fileList.remove("..")

exe="./bfAlgo"

fout=open("go.sh", "w")

for fname in fileList:
    ofname=fname+".out"
    lfname=fname+".log"
    cmd=exe+" "+tsDir+fname+" "+oDir+ofname+" "+lDir+lfname+"\n"
    fout.write(cmd)


fout.flush()
fout.close()
