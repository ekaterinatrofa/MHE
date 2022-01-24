import os
import re
# import pandas as pd
# import numpy as np
from collections import defaultdict
from statistics import mean


# iterations = [10, 100]
iterations = [10, 100, 1000, 10000, 100000, 1000000]
temp_foo = ["fast", "logarithmic"]
temp = [5.0, 10.0, 50.0, 500.0, 1000.0, 2000.0]

d = defaultdict(list)
cwd = os.getcwd()

for iteration in iterations:
    for repeat in range(25):
        for temperature_function in temp_foo:
            for temperature in temp:
                cmdName = "\"" + cwd + "\\bin\\Debug\\sa.exe\" " + "--iterations " + str(iteration) + "--tempfunction " \
                          + temperature_function + "--temp " + str(temperature)
                # print(cmdName)
                result = os.popen(cmdName)
                output = result.read()
                # print(output)
                goal = re.findall("=.*", output)
                goal = re.findall("[0-9.]+", goal[0])
                # print(goal)
                experiment = "--iterations " + str(iteration) + " --tempfunction " + temperature_function + " --temp " + str(temperature)
                # print(experiment)
                d[experiment].append([float(goal[0])])

result = []
for k, v in d.items():
    mean_goal = mean([x[0] for x in v])
    # print(k, mean_goal)
    result.append([k, mean_goal])

result.sort(key=lambda x: x[1])

# print(result)

for x in result:
    print(x)
with open("res_sa.txt", 'w') as outfile:
    outfile.write('\n'.join(' , '.join(map(str, row)) for row in result))



