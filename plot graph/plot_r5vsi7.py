import csv
import matplotlib.pyplot as plt
import numpy as np

def read_file_r5(file_name):
    with open(file_name, 'r') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        next(csv_reader)
        size = []
        t1 = []
        t2 = []
        t4 = []
        t8 = []
        t16 = []
        for line in csv_reader:
            size.append(int(line[0]))
            t1.append(float(line[1]))
            t2.append(float(line[2]))
            t4.append(float(line[3]))
            t8.append(float(line[4]))
            t16.append(float(line[5]))
    return size, t1, t2, t4, t8, t16

def read_file_i7(file_name):
    with open(file_name, 'r') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        next(csv_reader)
        size = []
        t1 = []
        t2 = []
        t4 = []
        t8 = []
        for line in csv_reader:
            size.append(int(line[0]))
            t1.append(float(line[1]))
            t2.append(float(line[2]))
            t4.append(float(line[3]))
            t8.append(float(line[4]))
    return size, t1, t2, t4, t8


def plot_graph(title):

    datar5 = read_file_r5('time-r5.csv')
    datai7 = read_file_i7('time-i7.csv')
    size = datar5[0]
    ax = plt.gca()
    plt.title(title)
    ax.set_xscale('log', basex=2)
    ax.set_xticks(size)
    ax.set_yticks(np.arange(0, 8000, 250))
    plt.grid(True, alpha=0.5)

    for t in range(len(datar5[1:])):
        plt.plot(size, datar5[t+1], marker='o', markersize=2, linestyle='-', linewidth=1.5, label="R5 {}-thread".format(2**t))

    for t in range(len(datai7[1:])):
        plt.plot(size, datai7[t+1], marker='o', markersize=2, linestyle='--', linewidth=1.5, label="i7 {}-thread".format(2**t))

    plt.xlabel('Array size (element)', fontsize=13, labelpad=5)
    plt.ylabel('Sorting time (ms)', fontsize=13, labelpad=5)
    plt.legend(loc=0, fontsize=12)
    plt.show()

graph_title = 'Ryzen 5 1600 Vs i7 6700k'
plot_graph(graph_title)