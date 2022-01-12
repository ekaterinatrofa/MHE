import os
import re
# import pandas as pd
# import numpy as np
from collections import defaultdict
from statistics import mean

mutation_probability = [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]
crossover_probability = [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]
pop_size = [10, 20, 40, 60]
# pop_size = [10]

select = ["tournament_selection", "roulette_selection"]
cross = ["one", "two"]

files = ["ssp_20.txt"]
# files = ["ssp_5.txt"]

method = "ga"
d = defaultdict(list)
cwd = os.getcwd()
for repeat in range(25):
    for file in files:
        print(file)
        for mutation_p in mutation_probability:
            # print(mutation_p)
            for crossover_p in crossover_probability:
                # print(crossover_p)
                for selection in select:
                    # print(selection)
                    for crossover in cross:
                        # print(crossover)
                        for population in pop_size:
                            cmdName = "\""+cwd+"\\bin\\Debug\\subset_sum_problem.exe\" " \
                                      "--population_size " + str(population) + " --method " + method + \
                                      " --mutation_prob " + str(mutation_p) + " --crossover_prob " + str(crossover_p) + \
                                      " --selection " + selection + " --crossover " + crossover + " --inputfile .\\bin\\Debug\\" + file
                            print(cmdName)
                            result = os.popen(cmdName)
                            output = result.read()
                            print(output)
                            goal = re.findall("goal.*", output)
                            goal = re.findall("[0-9.]+", goal[0])
                            # print(goal)
                            calcTime = re.findall("dt.*", output)
                            calcTime = re.findall("[0-9.]+", calcTime[0])
                            # print(calcTime)
                            experiment = "inputfile = " + file + "; selection = " + selection + "; crossover = " + crossover + "; population_size = " + str(
                                population) + "; mutation_prob = " + str(mutation_p) + "; crossover_prob = " + str(
                                crossover_p)
                            # print(experiment)
                            d[experiment].append([int(goal[0]), float(calcTime[0])])

result = []
for k, v in d.items():
    mean_time = mean([x[1] for x in v])
    mean_goal = mean([x[0] for x in v])
    # print(k , mean_time , mean_goal)
    result.append([k, mean_time, mean_goal])

result.sort(key=lambda x: x[2])
with open("res_parallel.txt", 'w') as outfile:
    outfile.write('\n'.join(' , '.join(map(str, row)) for row in result))

