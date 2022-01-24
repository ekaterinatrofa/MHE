import csv

reader = csv.reader(open("res_.txt"), delimiter=",")
time_l_20 = []
for x in reader:
    # if 0 <=float(x[2].strip()) <= 0.5:
    if x[2].strip() == '0':
        time_l_20.append(x)

sorted_20 = sorted(time_l_20, key=lambda x: x[1])

for x in sorted_20:
    print(x)

reader = csv.reader(open("res_.txt"), delimiter=",")
time_avg_seq = []

for x in reader:
    time_avg_seq.append(x)

print()
print("max average time in sequential GA")

print(max(time_avg_seq, key=lambda x: x[1]))

print("-------------------------------------------------------------------")
print("parallel")
reader = csv.reader(open("res_parallel.txt"), delimiter=",")
time_l_20_parallel = []

for x in reader:
    # if 0 <=float(x[2].strip()) <= 0.5:
    if x[2].strip() == '0':
        time_l_20_parallel.append(x)

sorted_20_parallel = sorted(time_l_20_parallel, key=lambda x: x[1])

for x in sorted_20_parallel:
    print(x)

reader = csv.reader(open("res_parallel.txt"), delimiter=",")

time_avg_parallel = []
for x in reader:
    time_avg_parallel.append(x)

print()
print("max average time in parallel GA")
print(max(time_avg_parallel, key=lambda x: x[1]))


