#!/usr/bin/python
import sys
import matplotlib.pyplot as plt
from subprocess import call

f = open("xxx")
rbound = int(1)
while True:
    line = f.readline().strip()
    if not line: break
    [x, y] = line.split()
    x = int(x)
    y = int(y)
    plt.bar([x], [y], [1])

plt.title("days between first visit and first buy, delete 0, 1, 2");
plt.xlabel("day")
plt.ylabel("sell amount")
savepath="./tmp.png"
plt.savefig(savepath)
#call("/usr/bin/shotwell", savepath)

