import matplotlib.pyplot as plt

def obtainInfo(filename, p1, m1, p2, m2):
    fin=open(filename, "r")
    fin.readline()
    fin.readline()
    fin.readline()
    raw_data=fin.readline().strip().split('\t')
    for x in raw_data:
        p1.append(float(x))
    fin.readline()
    fin.readline()
    raw_data=fin.readline().strip().split('\t')
    for x in raw_data:
        m1.append(float(x))
    fin.readline()
    fin.readline()
    fin.readline()
    raw_data=fin.readline().strip().split('\t')
    for x in raw_data:
        p2.append(float(x))
    fin.readline()
    fin.readline()
    raw_data=fin.readline().strip().split('\t')
    for x in raw_data:
        m2.append(float(x))
    fin.close()

resDir="/home/hadoop/comparison/statistic/0103/"

bfResPath=resDir+"bfRes"
pfResPath=resDir+"pfRes"
dufoResPath=resDir+"dufoRes"
runResPath=resDir+"runRes"
uedfResPath=resDir+"uedfRes"

coreNum=[2*(i+1) for i in range(16)]

bpc=[]
bmc=[]
ppc=[]
pmc=[]
dpc=[]
dmc=[]
rpc=[]
rmc=[]
upc=[]
umc=[]

utval=[5*(i+11) for i in range(10)]

bpu=[]
bmu=[]
ppu=[]
pmu=[]
dpu=[]
dmu=[]
rpu=[]
rmu=[]
upu=[]
umu=[]

obtainInfo(bfResPath, bpc, bmc, bpu, bmu)
assert(len(bpc)==len(coreNum))
assert(len(bmc)==len(coreNum))
assert(len(bpu)==len(utval))
assert(len(bmu)==len(utval))
obtainInfo(pfResPath, ppc, pmc, ppu, pmu)
assert(len(ppc)==len(coreNum))
assert(len(pmc)==len(coreNum))
assert(len(ppu)==len(utval))
assert(len(pmu)==len(utval))
obtainInfo(dufoResPath, dpc, dmc, dpu, dmu)
assert(len(dpc)==len(coreNum))
assert(len(dmc)==len(coreNum))
assert(len(dpu)==len(utval))
assert(len(dmu)==len(utval))
obtainInfo(runResPath, rpc, rmc, rpu, rmu)
assert(len(rpc)==len(coreNum))
assert(len(rmc)==len(coreNum))
assert(len(rpu)==len(utval))
assert(len(rmu)==len(utval))
obtainInfo(uedfResPath, upc, umc, upu, umu)
assert(len(upc)==len(coreNum))
assert(len(umc)==len(coreNum))
assert(len(upu)==len(utval))
assert(len(umu)==len(utval))

plt.subplot(2,2,1)
plt.title("coreNum preemption")
plt.plot(coreNum, bpc, label="bf")
plt.plot(coreNum, ppc, label="pf")
plt.plot(coreNum, dpc, label="dufo")
plt.plot(coreNum, rpc, label="run")
plt.plot(coreNum, upc, label="uedf")
plt.legend(loc="upper left", frameon=False)

plt.subplot(2,2,2)
plt.title("coreNum migration")
plt.plot(coreNum, bmc, label="bf")
plt.plot(coreNum, pmc, label="pf")
plt.plot(coreNum, dmc, label="dufo")
plt.plot(coreNum, rmc, label="run")
plt.plot(coreNum, umc, label="uedf")
plt.legend(loc="upper left", frameon=False)

plt.subplot(2,2,3)
plt.title("utilization preemption")
plt.plot(utval, bpu, label="bf")
plt.plot(utval, ppu, label="pf")
plt.plot(utval, dpu, label="dufo")
plt.plot(utval, rpu, label="run")
plt.plot(utval, upu, label="uedf")
plt.legend(loc="upper left", frameon=False)

plt.subplot(2,2,4)
plt.title("utilization migration")
plt.plot(utval, bmu, label="bf")
plt.plot(utval, pmu, label="pf")
plt.plot(utval, dmu, label="dufo")
plt.plot(utval, rmu, label="run")
plt.plot(utval, umu, label="uedf")
plt.legend(loc="upper left", frameon=False)

plt.show()
