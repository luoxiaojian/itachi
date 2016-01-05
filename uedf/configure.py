import os

tsDir="/home/hadoop/comparison/sample/"
fileList=os.listdir(tsDir)
oDir="/home/hadoop/comparison/uedf/log/"
#fileList.remove(".")
#fileList.remove("..")

exe="./algo"

fout=open("go.sh", "w")

for fname in fileList:
    ofname=fname+".log"
    cmd=exe+" "+tsDir+fname+" "+oDir+ofname+"\n"
    fout.write(cmd)


fout.flush()
fout.close()
