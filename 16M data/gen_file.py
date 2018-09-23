import random
import csv
import matplotlib.pyplot as plt

def create_random(length, min_num, max_num):
    l = []
    for i in range(0, length):
        l.append(random.randint(min_num, max_num))

    if length > 200:
        r = l[0:200]
        plt.plot(list(range(len(r))), r, 'o', markersize=2)
        plt.show()
    else:
        plt.plot(list(range(len(l))), l, 'o', markersize=2)
        plt.show()
    return(l)

def create_decrease(length, start, max_step,chance=60):
    l = [start]
    for i in range(0, length):
        c = random.randint(0, 100)
        if c <= chance:
            k = random.randint(0, max_step)
            l.append( int(l[-1] - k) )
        else:
            k = random.randint(0, chance)
            l.append( int(l[-1] + k) )

    if length > 200:
        r = l[0:200]
        plt.plot(list(range(len(r))), r, 'o', markersize=2)
        plt.show()
    else:
        plt.plot(list(range(len(l))), l, 'o', markersize=2)
        plt.show()
    return(l)

def create_increase(length, start, max_step,chance=60):
    l = [start]
    for i in range(0, length):
        c = random.randint(0, 100)
        if c <= chance:
            k = random.randint(0, max_step)
            l.append( int(l[-1] + k) )
        else:
            k = random.randint(0, chance)
            l.append( int(l[-1] - k) )

    if length > 200:
        r = l[0:200]
        plt.plot(list(range(len(r))), r, 'o', markersize=2)
        plt.show()
    else:
        plt.plot(list(range(len(l))), l, 'o', markersize=2)
        plt.show()
    return(l)

def write_file(name, l):
    file = open('{}.txt'.format(name), 'w')
    for i in l:
        file.write("{} ".format(i))

# 2**20 = 1048576 
# 2*22 = 4194304
length = 2**24
dec = create_decrease(length, 1000000, 100, 75)
write_file("decrease_int", dec)
inc = create_increase(length, -10000000, 100, 75)
write_file("increase_int", inc)
ran = create_random(length, -90000000, 90000000)
write_file("random_int", ran)




