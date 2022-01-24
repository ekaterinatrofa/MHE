import csv

reader = csv.reader(open("res.txt"), delimiter=",")
time_l_20 = []
time_l_10 = []
time_l_5 = []
for x in reader:
    # print(x[0].split(';'))
    # print(x[0].split(';'))
    # 0 <=float(x[2].strip()) <= 1.0

    if x[0].split(';')[0] == 'inputfile = ssp_20.txt' and x[2].strip() == '0':
        time_l_20.append(x)
    if x[0].split(';')[0] == 'inputfile = ssp_10.txt' and x[2].strip() == '0':
        time_l_10.append(x)
    if x[0].split(';')[0] == 'inputfile = ssp_5.txt' and 0 <=float(x[2].strip()) <= 1.0:
        time_l_5.append(x)

sorted_20 = sorted(time_l_20, key=lambda x: x[1])
sorted_10 = sorted(time_l_10, key=lambda x: x[1])
sorted_5 = sorted(time_l_5, key=lambda x: x[1])
# # print(sortedl)
for x in sorted_20:
    print(x)

print("-----------")

for x in sorted_10:
    print(x)

print("-----------")

for x in sorted_5:
    print(x)