import subprocess
import sys
import numpy as np
from matplotlib import pyplot as plt

def getPageFaults(method="lru", pSize=20, vSize=60, sSize=60, testcase="req1.dat"):
	try:
		output = subprocess.Popen(["./" + method, str(vSize), str(pSize), str(sSize), "./testcases/" + testcase], stdout=subprocess.PIPE).communicate()[0]
		pageFaults = output.split()[-1]
		return int(pageFaults)
	except Exception as e:
		print(f"Error occurred in getPageFaults for method {method}, pSize {pSize}, vSize {vSize}, sSize {sSize}, testcase {testcase}: {e}")
		return 0
	
def plot(vSize=60, testcase="req1.dat"):
	policies = ["lru", "random", "fifo"]
	for method in policies:
		x = np.arange(1, vSize + 1, 1)
		pageFaults = []
		# frames in range of [1, vSize]
		for i in x:
			pf = getPageFaults(method=method, pSize=i, vSize=vSize, testcase=testcase)
			if (pf == 0):
				pageFaults.append(pageFaults[-1])
			else:
				pageFaults.append(pf)
		plt.plot(x, pageFaults, label=method)

	plt.title(f"File: {testcase} \n # of logical pages: {vSize}") 
	plt.ylabel("Number of page faults")
	plt.xlabel("Number of Frames")
	plt.legend()
	plt.savefig("./images/" + testcase.split(".")[0] + ".jpg")
	plt.clf()


def main():
	plot(60, "req1.dat")
	plot(60, "req2.dat")
	plot(30, "req3.dat")
	plot(100, "req4.dat")
	plot(100, "req5.dat")

if __name__ == "__main__":
	main()

