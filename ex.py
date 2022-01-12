import os
import re
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.collections import EventCollection

statistics = {
    "tabu": [],
    "hill_climb": [],
    "brute_force": []
}

time_brut = []
time_climb = []
time_tabu = []

size_brut = []
size_climb = []
size_tabu = []

result_brut = []
result_climb = []
result_tabu = []

time_result_brut = []
time_result_climb = []
time_result_tabu = []

kkk = ["ssp_5.txt", "ssp_10.txt", "ssp_20.txt"]

for method_name in statistics:
    for file in kkk:
        head, _ = os.path.splitext(file)
        problem_size = int(head.split('_')[1])
    # for problem_size in range(5, 15):
        for repeat in range(1, 6):
            # cmdName = "C:\\Users\\PC\\Documents\\3^ rok\\MHE1\\subset_sum_problem\\bin\\Debug\\subset_sum_problem.exe " \
            #           "--size " + str(problem_size) + " --method " + method_name + " --iterations 100 --tabu_size 20 "

            cmdName = "C:\\Users\\PC\\Documents\\3^ rok\\MHE1\\subset_sum_problem\\bin\\Debug\\subset_sum_problem.exe " \
                      " --inputfile .\\bin\\Debug\\" + file + " --method " + method_name + " --outputfile .\\bin\\Debug\\"+ method_name + file + " --iterations 100 --tabu_size 20"
            print(cmdName)
            result = os.popen(cmdName)
            output = result.read()
            print(output)
            calcTime = re.findall("dt.*", output)
            # print(calcTime)
            if len(calcTime) > 0:
                calcTime = re.findall("[0-9.]+", calcTime[0])
                result_val = re.findall("[0-9.]+", re.findall("result.*", output)[0])
                # print(float(result_val[0]))
                statistics[method_name].append([problem_size, float(result_val[0]), float(calcTime[0])])

# print(statistics)
with open("result.plt", "a") as gnuplotFile:
    gnuplotFile.write("set term png\n")
    gnuplotFile.write("set output \"result.png\"\n")
    gnuplotFile.write("plot ")
    for method_name in statistics:
        # print(method_name)
        summary = statistics[method_name]
        # print(summary)
        per_size = {}
        for values in summary:
            if per_size.get(values[0]) is None:
                per_size[values[0]] = [[values[1], values[2]]]
            else:
                per_size[values[0]].append([values[1], values[2]])
        # print(per_size)
        for s in per_size:
            combined = np.mean(per_size[s], axis=0)
            with open("result_" + method_name + ".txt", "a") as myfile:
                myfile.write(
                    method_name + "    " + str(s) + "    " + str(combined[0]) + "    " + str(combined[1]) + "\n")
                if method_name == 'brute_force':
                    size_brut.append(int(s))
                    time_brut.append(float(combined[1]))
                    # result_brut.append(float(combined[0]))
                    if s > 4:
                        result_brut.append(float(combined[0]))
                        time_result_brut.append(float(combined[1]))

                elif method_name == 'hill_climb':
                    size_climb.append(int(s))
                    time_climb.append(float(combined[1]))
                    # result_climb.append(float(combined[0]))
                    if s > 4:
                        result_climb.append(float(combined[0]))
                        time_result_climb.append(float(combined[1]))

                elif method_name == 'tabu':
                    size_tabu.append(int(s))
                    time_tabu.append(float(combined[1]))
                    # result_tabu.append(float(combined[0]))
                    if s > 4:
                        result_tabu.append(float(combined[0]))
                        time_result_tabu.append(float(combined[1]))


        gnuplotFile.write("'result_" + method_name + ".txt' u 1:2 w lines, ")

    gnuplotFile.write("\n")

xdata1 = size_brut[:]
ydata1 = time_brut[:]

data1 = result_brut[:]
dataTime1 = time_result_brut[:]

xdata2 = size_climb[:]
ydata2 = time_climb[:]

data2 = result_climb[:]
dataTime2 = time_result_climb[:]

xdata4 = size_tabu[:]
ydata4 = time_tabu[:]
# print(ydata4)
data4 = result_tabu[:]
dataTime4 = time_result_tabu[:]
print(data4)


xdata1.sort()
ydata1.sort()
data1.sort()
dataTime1.sort()
xdata2.sort()
ydata2.sort()
data2.sort()
dataTime2.sort()
xdata4.sort()
ydata4.sort()
data4.sort()
dataTime4.sort()

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)

ax.plot(xdata1, ydata1, color='tab:blue', label='Bruteforce')
ax.plot(xdata2, ydata2, color='tab:green', label='Climbing')
ax.plot(xdata4, ydata4, color='tab:red', label='Tabu')

xevents1 = EventCollection(xdata1, color='tab:blue', linelength=0.05, orientation='vertical')
xevents2 = EventCollection(xdata2, color='tab:green', linelength=0.05, orientation='vertical')
xevents4 = EventCollection(xdata4, color='tab:red', linelength=0.05, orientation='vertical')

yevents1 = EventCollection(ydata1, color='tab:blue', linelength=0.05, orientation='vertical')
yevents2 = EventCollection(ydata2, color='tab:green', linelength=0.05, orientation='vertical')
yevents4 = EventCollection(ydata4, color='tab:red', linelength=0.05, orientation='vertical')

ax.add_collection(xevents1)
ax.add_collection(xevents2)
ax.add_collection(xevents4)
ax.add_collection(yevents1)
ax.add_collection(yevents2)
ax.add_collection(yevents4)

ax.set_xlim([min(xdata2), max(xdata2)])
ax.set_ylim([0, 1])
ax.legend()
ax.set_title('Graph of estimated time of generated solution')
plt.savefig("Time-Size")
plt.show()
plt.close()


# fig_r = plt.figure()
# ax = fig_r.add_subplot(1, 1, 1)
#
# ax.plot(xdata1, data1, color='tab:blue', label='Bruteforce')
# ax.plot(xdata2, data2, color='tab:green', label='Climbing')
# ax.plot(xdata4, data4, color='tab:red', label='Tabu')
#
# xevents1 = EventCollection(xdata1, color='tab:blue', linelength=0.05, orientation='vertical')
# xevents2 = EventCollection(xdata2, color='tab:green', linelength=0.05, orientation='vertical')
# xevents4 = EventCollection(xdata4, color='tab:red', linelength=0.05, orientation='vertical')
#
# yevents1 = EventCollection(data1, color='tab:blue', linelength=0.05, orientation='vertical')
# yevents2 = EventCollection(data2, color='tab:green', linelength=0.05, orientation='vertical')
# yevents4 = EventCollection(data4, color='tab:red', linelength=0.05, orientation='vertical')
#
# ax.add_collection(xevents1)
# ax.add_collection(xevents2)
# ax.add_collection(xevents4)
# ax.add_collection(yevents1)
# ax.add_collection(yevents2)
# ax.add_collection(yevents4)
#
# ax.set_xlim([min(xdata2), max(xdata2)])
# ax.set_ylim([0, 1])
# ax.legend()
# ax.set_title('Graph of estimated time of generated solution')
# plt.savefig("Time-Size")
# plt.show()
# plt.close()
#
index = np.arange(len(dataTime1))
print(data1)
print(dataTime1)
print("------")
data1 = np.around(data1, decimals=4)
df = pd.DataFrame({'BFresult': data1, 'time': dataTime1}, index=index)

ax = df.plot.bar(x='BFresult', rot=0)
plt.savefig("BFresult")
plt.show()
plt.close()


index = np.arange(len(dataTime2))
print(data2)
print(dataTime2)
print("------")
data2 = np.around(data2, decimals=4)
df = pd.DataFrame({'HCresult': data2, 'time': dataTime2}, index=index)

ax = df.plot.bar(x='HCresult', rot=0)
plt.savefig("HCresult")
plt.show()
plt.close()


index = np.arange(len(dataTime4))
print(data4)
print(dataTime4)
print("------")
data4 = np.around(data4, decimals=4)
df = pd.DataFrame({'TSresult': data4, 'time': dataTime4}, index=index)

ax = df.plot.bar(x='TSresult', rot=0)
plt.savefig("TSresult")
plt.show()
plt.close()


# result = os.popen("gnuplot result.plt")
# output = result.read()
