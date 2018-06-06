import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
import numpy as np

def process(filename):
    seq = []
    uni = []
    with open(filename, "r") as fp:
        line = fp.readline()
        while line:
            if (line.startswith("Sequential Access")):
                times = []
                for i in range(7):
                    line = fp.readline()
                    time = float(line.split()[-1][:-2])
                    if (time == 0):
                        time = 1e-7
                    times.append(time)
                seq.append(times)
            elif (line.startswith("Accessing Elements with A Uniform Distribution")):
                times = []
                for i in range(7):
                    line = fp.readline()
                    time = float(line.split()[-1][:-2])
                    times.append(time)
                uni.append(times)
            line = fp.readline()
    return np.array(seq), np.array(uni)

seq, uni = process("benchmarks_new2.txt")
plt.figure()
plt.plot(2**np.arange(0, 25), uni[:,0], '-o', label='Perfectly Balanced Tree')
plt.plot(2**np.arange(0, 25), uni[:,1], '-o', label='Weight-Balanced Tree')
plt.plot(2**np.arange(0, 25), uni[:,2], '-o', label='Splay Tree')
plt.plot(2**np.arange(0, 25), uni[:,3], '-o', label='Multi-Splay Tree')
plt.plot(2**np.arange(0, 25), uni[:,4], '-o', label='std::set')
plt.plot(2**np.arange(0, 25), uni[:,5], '-o', label='Sorted Array')
plt.plot(2**np.arange(0, 25), uni[:,6], '-o', label='std::unordered_set')
plt.title("Runtime of 2^18 Uniform Queries")
plt.xlabel('Number of Elements')
plt.ylabel('Milleseconds')
plt.legend()
plt.savefig("uniform.png")
plt.close()

plt.figure()
plt.plot(2**np.arange(7, 25), seq[7:25,0], '-o', label='Perfectly Balanced Tree')
plt.plot(2**np.arange(7, 25), seq[7:25,1], '-o', label='Weight-Balanced Tree')
plt.plot(2**np.arange(7, 25), seq[7:25,2], '-o', label='Splay Tree')
plt.plot(2**np.arange(7, 25), seq[7:25,3], '-o', label='Multi-Splay Tree')
plt.plot(2**np.arange(7, 25), seq[7:25,4], '-o', label='std::set')
plt.plot(2**np.arange(7, 25), seq[7:25,5], '-o', label='Sorted Array')
plt.plot(2**np.arange(7, 25), seq[7:25,6], '-o', label='std::unordered_set')
plt.title("Runtime of Sequential Queries on All Elements")
plt.xlabel('Number of Elements')
plt.ylabel('Milleseconds')
plt.legend()
# plt.xticks(range(7, 25))
plt.savefig("sequential.png")
plt.close()
