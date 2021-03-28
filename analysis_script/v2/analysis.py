import csv
import math
import os

import matplotlib.pyplot as plt
from scipy.stats import mannwhitneyu
import numpy as np


def set_param(data, params):
    for i in range(0, len(params["lbound"])):
        a = (params["hbound"][i] + params["lbound"][i]) / 2.0
        b = (params["hbound"][i] - params["lbound"]) / 2.0
        data[i + 1] = a + b * math.tanh(float(data[i + 1])) #i + 1 because 0 in x is element number
    return data


def _bounds(data, number_params, params, notUse=False):  # for just convert type without bounds convert
    if number_params >= len(params["lbound"]):
        if number_params == params["delta_index"]:
            for i, _ in enumerate(data):
                a = (5.0 + 0.0)/2.0
                b = (5.0 - 0.0)/2.0
                if notUse:
                    data[i] = float(data[i])
                else:
                   data[i] = a + b * math.tanh(float(data[i]))
        else:
            for i, _ in enumerate(data):
                data[i] = float(data[i])
        return data
    else:
        for i, _ in enumerate(data):
            a = (params["hbound"][number_params] + params["lbound"][number_params])/2.0
            b = (params["hbound"][number_params] - params["lbound"][number_params])/2.0
            if notUse:
                data[i] = float(data[i])
            else:
                data[i] = a + b * math.tanh(float(data[i]))
    return data

def _is_string_with_definition_iter(list):
    return bool(len(list) > 3)


def plot_graphics_evolution_param(file_name, params, isBounds = False):
    iter_data = []
    skip_index = 6  # we skip element number, because there is no need to look for matches here
    step = 3
    folder = os.path.dirname(file_name)
    count_iter = 0
    count_start_iter = 0
    with open(file_name) as file:
        for line in file:
            line_list = line.split()
            if _is_string_with_definition_iter(line_list):
                num_list = [num for num in line_list[skip_index:len(line_list):step]]
            else:
                num_list = "iteration = " + line_list[2]
                if count_iter > int(line_list[2]):
                    count_start_iter = count_iter
                count_iter = int(line_list[2])

            iter_data.append(num_list)
    count_iter += 2 + count_start_iter # 2 because we start with -1 and after that we have 1, but when start iter is over we start with 0
    optimize_param = [[] for _ in range(params["num_log_param"])]

    for iter_line in iter_data:
        if not "iteration = " in iter_line:
            ind = 0
            for res in iter_line:
                optimize_param[ind].append(res)
                ind = ind + 1
    number_param = 0
    for param in optimize_param:
        len_param = len(param)
        for theta in range(0, params["count_theta"]):
            fig = plt.figure()
            data_for_graphics = [float(x) for x in param[theta:len_param:params["count_theta"]]]
            if isBounds:
                _bounds(data_for_graphics, number_param, params)
                filename = os.path.join(folder, "images", "params[" + str(number_param) + "]" +
                                        "individ[" + str(theta) + "]_bounds.pdf")
            else:
                filename = os.path.join(folder, "images", "params[" + str(number_param) + "]" +
                                        "individ[" + str(theta) + "].pdf")
            plt.plot(data_for_graphics)
            fig.savefig(filename)
            
        number_param += 1

    return count_iter


if __name__ == "__main__":
    folder_paths = ["data/09_03/v3/with_snp", "data/09_03/v4/with_snp", "data/09_03/v4/without_snp"]
    log_result_name = "log_result.txt"
    log_iter_name = "log_iteration.txt"
    log_deeps_name = "log_deeps"
    params = {"num_log_param": 9,
              "count_theta": 12,
              "lbound": [1, 10, 0.005, 0.005, 0.005, 0.005],
              "hbound": [7, 20, 10, 1, 1, 10],
              "size_training": 1000,
              "size_valid": 666,
              "count_param": 6,
              "delta_index": 8}
    for folder in folder_paths:
        log_result = os.path.abspath(os.path.join(folder, log_result_name))
        log_deeps = os.path.abspath(os.path.join(folder, log_deeps_name))
        log_iter = os.path.abspath(os.path.join(folder, log_iter_name))

        plot_graphics_evolution_param(log_result, params)
        plot_graphics_evolution_param(log_result, params, isBounds=True)
