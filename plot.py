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
plt.plot(uni[:,0], '-o', label='Perfectly Balanced Tree')
plt.plot(uni[:,1], '-o', label='Weight-Balanced Tree')
plt.plot(uni[:,2], '-o', label='Splay Tree')
plt.plot(uni[:,3], '-o', label='Multi-Splay Tree')
plt.plot(uni[:,4], '-o', label='std::set')
plt.plot(uni[:,5], '-o', label='Sorted Array')
plt.plot(uni[:,6], '-o', label='std::unordered_set')
plt.title("Runtime of 2^18 Uniform Queries")
plt.xlabel('Log2 of Number of Total Elements')
plt.ylabel('Milleseconds')
plt.legend()
plt.savefig("uniform.png")
plt.close()

plt.figure()
plt.plot(np.log2(seq[:,0]), '-o', label='Perfectly Balanced Tree')
plt.plot(np.log2(seq[:,1]), '-o', label='Weight-Balanced Tree')
plt.plot(np.log2(seq[:,2]), '-o', label='Splay Tree')
plt.plot(np.log2(seq[:,3]), '-o', label='Multi-Splay Tree')
plt.plot(np.log2(seq[:,4]), '-o', label='std::set')
plt.plot(np.log2(seq[:,5]), '-o', label='Sorted Array')
plt.plot(np.log2(seq[:,6]), '-o', label='std::unordered_set')
plt.title("Runtime of Sequential Queries on All Elements")
plt.xlabel('Log2 of Number of Elements')
plt.ylabel('Log2 of Milleseconds')
plt.legend()
plt.savefig("sequential.png")
plt.close()