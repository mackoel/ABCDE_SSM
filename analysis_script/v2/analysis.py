import csv
import math
import os
from dataclasses import dataclass
from typing import Dict, Optional
import matplotlib.pyplot as plt
from scipy.stats import mannwhitneyu
import numpy as np

import errno
import sys

def set_param(data, params):
    for i in range(0, len(params["lbound"])):
        a = (params["hbound"][i] + params["lbound"][i]) / 2.0
        b = (params["hbound"][i] - params["lbound"][i]) / 2.0
        data[i + 1] = a + b * math.tanh(float(data[i + 1])) #i + 1 because 0 in x is element number
    return data


def _bounds(data, number_params, params, notUse=False):  # for just convert type without bounds convert
    if number_params >= len(params["lbound"]):
        if number_params == params["delta_index"]:
            for i, _ in enumerate(data):
                a = (5.0 + 0.0) / 2.0
                b = (5.0 - 0.0) / 2.0
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

def _is_string_without_definition_iter(list):
    return bool(len(list) > 3)


def plot_graphics_evolution_param(file_name, params, isBounds = False):
    res_data = []
    skip_index = 6  # we skip element number, because there is no need to look for matches here
    step = 3
    folder = os.path.dirname(file_name)

    imdir = os.path.join(folder, "images")

    try:
        os.makedirs(imdir)
    except OSError as exc:  # Python ≥ 2.5
        if exc.errno == errno.EEXIST and os.path.isdir(imdir):
            pass
        # possibly handle other errno cases here, otherwise finally:
        else:
            raise

    count_iter = 0
    count_start_iter = 0 #maybe 0 i dont know
    with open(file_name) as file:
        for line in file:
            line_list = line.split()
            if _is_string_without_definition_iter(line_list):
                num_list = [num for num in line_list[skip_index:len(line_list):step]]
            else:
                num_list = "iteration = " + line_list[2]
                if count_iter > int(line_list[2]):
                    count_start_iter = count_iter
                count_iter = int(line_list[2])

            res_data.append(num_list)
    count_iter += 2 + count_start_iter # 2 because we start with -1 and after that we have 1, but when start iter is over we start with 0
    optimize_param = [[] for _ in range(params["num_log_param"])]

    for iter_line in res_data:
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


def plot_graphics_evolution_param_best(file_name, params, isBounds=False):
    res_data = []
    best_error = 10000.0
    skip_index = 6  # we skip element number, because there is no need to look for matches here
    step = 3
    folder = os.path.dirname(file_name)
    imdir = os.path.join(folder, "images")

    try:
        os.makedirs(imdir)
    except OSError as exc:  # Python ≥ 2.5
        if exc.errno == errno.EEXIST and os.path.isdir(imdir):
            pass
        # possibly handle other errno cases here, otherwise finally:
        else:
            raise

    error_index = params["delta_index"] - 1

    param_values = None
    with open(file_name) as file:
        for line in file:
            line_list = line.split()
            if _is_string_without_definition_iter(line_list):
                num_list = [num for num in line_list[skip_index:len(line_list):step]]
            else:
                best_error = 10000.0
                if param_values is not None:
                    res_data.append(param_values)
                continue
            error = float(num_list[error_index])
            best_error = float(best_error)
            if error <= best_error:
                param_values = num_list
                best_error = num_list[error_index]

    if len(res_data) != params["count_iter"]:
        print("ERROR")
        print(len(res_data))
        print(params["count_iter"])
    optimize_param = [[] for _ in range(params["num_log_param"])]

    for iter_line in res_data:
        ind = 0
        for res in iter_line:
            optimize_param[ind].append(res)
            ind = ind + 1

    number_param = 0
    for param in optimize_param:
        fig = plt.figure()
        data_for_graphics = param
        if isBounds:
            _bounds(data_for_graphics, number_param, params)
            filename = os.path.join(folder, "images", "params[" + str(number_param) + "]_best_bounds.pdf")
        else:
            _bounds(data_for_graphics, number_param, params, True)
            filename = os.path.join(folder, "images", "params[" + str(number_param) + "]_best.pdf")
        plt.plot(data_for_graphics)
        plt.ylabel("value")
        plt.xlabel("iter")
        fig.savefig(filename)

        number_param += 1


def read_logs(l_r_file, l_d_file, l_i_file):
    start_take_param = 3  # because we need element number from log result
    step = 3
    res_data = []
    count_iter = 0
    with open(l_r_file) as f:
        for line in f:
            line_list = line.split()
            if _is_string_without_definition_iter(line_list):
                num_list = [num for num in line_list[start_take_param:len(line_list):step]]
            else:
                num_list = "iteration = " + line_list[2]
                count_iter = int(line_list[2])
            res_data.append(num_list)
    count_iter += 2
    d_data = []
    with open(l_d_file) as f:
        for line in f:
            if (line.startswith('file')):
                d_data.append(line)
    i = 0
    while i < len(d_data):
        if "/home/" in d_data[i]:
            del d_data[i]
        else:
            i += 1
    i_data = []
    with open(l_i_file) as f:
        for line in f:
            i_data.append(line)
    return (res_data, d_data, i_data, count_iter)

@dataclass
class DeepReader:
    params: Dict
    def parse_deep_header(self, _str):
        l_list = _str.split(" ")
        header = []
        target = []
        for l in l_list:
            h = l.split(":")
            if (len(h) == 2):
                header.append(h[0])
        for h in header:
            if "target" in h:
                target.append(h)

        return target


    def parse_deep(self, _str):
        l_list = _str.split(" ")
        res = []
        for l in l_list:
            h = l.split(":")
            if (len(h) == 2) and "target" in h[0]:
                res.append(h[1])
        return res


    def create_header(self, folder, deep_header):
        filename = folder + "/" + "table.txt"
        deep_output = self.parse_deep_header(deep_header)
        with open(filename, "w") as csvfile:
            writer = csv.writer(csvfile, delimiter=";")
            tab = ["element_number"]
            for i in range(self.params["count_param"]):
                tab.append("accept_param[" + str(i) + "]")
            tab.append("w")
            tab.append("error")
            tab.append("delta")
            for i in range(self.params["count_param"]):
                tab.append("accept_param_bounds[" + str(i) + "]")
            for i in range(self.params["count_param"]):
                tab.append("curr_param[" + str(i) + "]")
            for i in range(self.params["count_param"]):
                tab.append("curr_param_bounds[" + str(i) + "]")
            tab.append("iter")
            tab.append("element_number")
            tab.append("seed")
        #  tab.append("deepoutput")
            tab.append("original_alpha")
            tab.append("alpha")
            tab.append("accept")
            for target in deep_output:
                # print(target)
                tab.append(target)
        # tab = ["element_number", "n", "l", "l1", "l2", "l3", "l4", "w", "error", "delta", "n_b", "l_b", "l1_b", "l2_b", "l3_b", "l4_b",  "n_deep", "l_deep", "l1_deep", "l2_deep", "l3_deep", "l4_deep", "n_deep_b", "l_deep_b", "l1_deep_b", "l2_deep_b", "l3_deep_b", "l4_deep_b",  "iter", "element_number", "seed", "deepoutput", "original_alpha", "alpha", "accept"]
            writer.writerow(tab)


    def create_table(self, folder, x, x_b, x_deep, x_deep_b, iteration, element_number, curr_seed, d, o_alpha, alpha, accept):
        filename = folder + "/" + "table.txt"
        with open(filename, "a") as csvfile:
            writer = csv.writer(csvfile, delimiter=";")
            deep_res = self.parse_deep(d)
            #tab = [x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7], x[8], x[9], int(x_b[1]), int(x_b[2]), x_b[3], x_b[4],
            #       x_b[5], x_b[6], x_deep[1], x_deep[2], x_deep[3], x_deep[4], x_deep[5], x_deep[6], int(x_deep_b[1]),
            #       int(x_deep_b[2]), x_deep_b[3], x_deep_b[4], x_deep_b[5], x_deep_b[6], float(iteration),
            #       float(element_number), float(curr_seed), o_alpha, alpha, accept]
            tab = [element_number]
            for i in range(self.params["count_param"]):
                tab.append(x[i + 1])
            tab.append(x[self.params["count_param"] + 1])
            tab.append(x[self.params["count_param"] + 2])
            tab.append(x[self.params["count_param"] + 3])

            for i in range(self.params["count_param"]):
                tab.append(x_b[i + 1])

            for i in range(self.params["count_param"]):
                tab.append(x_deep[i + 1])

            for i in range(self.params["count_param"]):
                tab.append(x_deep_b[i + 1])

            for xx in [iteration, element_number, curr_seed, o_alpha, alpha, accept]:
                tab.append(xx)

            for res in deep_res:
                tab.append(res)
            writer.writerow(tab)

def plot_table(folder, params, l_r_file, x_data, d_data, i_data):
    deep_reader = DeepReader(params=params)
    deep_reader.create_header(folder, d_data[0])
    find = False
    k = 0
    accept_deep_output = []
    chain_data = []
    best_error = 20.0
    param_values = None
    error_index = params["delta_index"] - 1
    start_take_param = 3  # we skip element number, because there is no need to look for matches here
    step = 3
    best_deep_output = []
    with open(l_r_file) as f:
        for line in f:
            line_list = line.split()
            if _is_string_without_definition_iter(line_list):
                num_list = [num for num in line_list[start_take_param:len(line_list):step]]
            else:
                best_error = 10000.0
                if param_values is not None:
                    if param_values not in chain_data:
                        chain_data.append(param_values)
                continue
            new_error = float(num_list[error_index])
            best_error = float(best_error)
            if new_error <= best_error:
                param_values = num_list
                best_error = new_error

    list_seed = [-1 for _ in range(0, params["count_iter"] * params["count_theta"])]

    for x in x_data:
        if "iteration = " in x:
            iteration = x.split()[2]
        else:
            element_number = x[0]
            for d in d_data:  # in deep logs parallels output - so search by ids first
                if "iteration = " not in d:
                    continue
                curr_iter = d.split("iteration = ", 1)[1].split(" ")[0]
                curr_element_number = d.split("element number = ", 1)[1].split(" ")[0]
                curr_seed = d.split("seed = ", 1)[1].split("\n")[0]  # итерации run_a и run повторяются, надо внимательнее выбирать

                сurr_deep_output = d
                if float(element_number) == float(curr_element_number) and float(iteration) == float(
                        curr_iter) and curr_seed not in list_seed:
                    list_seed.append(curr_seed)
                    find = True
                    break
            for i in range(k, len(i_data)):
                if ("INIT" in i_data[i] or "RUN" in i_data[i] or "RUN_APPROXIMATE" in i_data[i] or "iteration = " in
                        i_data[i] or "element number = " in i_data[i] or "delta = " in i_data[i] or "error = " in
                        i_data[i]):  # delete
                    i += 1
                else:
                    check_param = [curr_element_number, i_data[i]]

                    for ii in range(params["count_param"]):
                        check_param.append(i_data[i + ii + 1])

                    i += params["delta_index"]
                    if "original alpha = " in i_data[i]:
                        o_alpha = i_data[i].split("original alpha = ", 1)[1].rstrip()
                        alpha = i_data[i + 1].split("alpha = ", 1)[1].rstrip()
                        accept = i_data[i + 2].rstrip()
                        i += 3

                    else:
                        o_alpha = "init"
                        alpha = "init"
                        accept = "accept"
                    if accept == "accept" or accept == "accept alpha":
                        accept_deep_output.append(сurr_deep_output)
                    for c in chain_data:
                        if c == x:
                            best_deep_output.append(сurr_deep_output)
                            break
                    k = i
                    break

            if find == False:
                x_param = []
                for i in range(0, len(x)):
                    x_param.append("null")
                print(x_param)
                deep_reader.create_table(folder,
                                         x_param,
                                         set_param(x, params),
                                         iteration,
                                         element_number,
                                         -1, "null", "null", "null", "null")
            else:
                x_param = []
                for i in range(0, len(x) - 3):
                    x_param.append(x[i])
                deep_reader.create_table(folder, x, set_param(x_param, params),
                                         check_param, set_param(check_param, params),
                                         iteration, element_number,
                                         curr_seed, сurr_deep_output.rstrip(),
                                         o_alpha, alpha, accept)
                find = False
    return accept_deep_output, best_deep_output


def plot_error(folder, params, cdo, d_data, _slice=0, mode=0):
    error_training = []
    error_valid = []
    # for x in x_data:
    #     if "iteration = " in x:
    #         iteration = x.split()[2]
    #     else:
    #         element_number = x[0]
    #         for d in d_data:
    #             curr_iter = d.split("iteration = ",1)[1].split(" ")[0]
    #             curr_element_number = d.split("element number = ", 1)[1].split(" ")[0]
    #             if float(element_number) == float(curr_element_number) and float(iteration) == float(curr_iter):
    #                 if d in cdo[_slice:]:#optional
    #                    error_training.append(float(d.split("target[1]:", 1)[1].split(" ")[0]) / size_training)
    #                    error_valid.append(float(d.split("target[21]:", 1)[1].split(" ")[0]) / size_valid)
    #                    break
    for d in cdo[_slice:]:

        if "target[7]" not in d or "target[21]" not in d:
            continue
        training_value = math.sqrt(float(d.split("target[7]:", 1)[1].split()[0]) / (2.0 * params["size_training"]))
        valid_value = math.sqrt(float(d.split("target[21]:", 1)[1].split()[0]) / (2.0 * params["size_valid"]))
        if (training_value < 100 and valid_value < 100):
            error_training.append(training_value)
            error_valid.append(valid_value)

    print(len(error_valid))
    print(len(error_training))
    print("MAX = ", max(error_training))
    print("MAX2 = ", max(error_valid))
    # error_training.remove(max(error_training))#if was deep error
    #  error_valid.remove(max(error_valid))

    f = plt.figure(figsize=(20, 10))
    max_max = min(max(max(error_training), max(error_valid)), 200.0)
    min_min = max(min(min(error_training), min(error_valid)), 0.0)
    print(max_max)
    bins_arr = np.arange(min_min, max_max, 3)
    plt.hist(error_training, bins=bins_arr, alpha=0.5, label='error_training', rwidth=1)
    plt.hist(error_valid, bins=bins_arr, alpha=0.5, label='error_valid', rwidth=1)
    plt.legend(loc='upper right')

    plt.axvline(np.mean(error_training), color='k', linestyle='dashed', linewidth=1)

    coord_train = np.median(error_training)
    min_ylim, max_ylim = plt.ylim()
    plt.text(coord_train * 1.1, max_ylim * 0.9, 'Median: {:.2f}'.format(coord_train))

    coord_valid = np.median(error_valid)
    plt.axvline(coord_valid, color='g', linestyle='dashed', linewidth=1)

    min_xlim, max_xlim = plt.xlim()
    plt.text(coord_valid * 1.1, max_ylim * 0.8, 'Median: {:.2f}'.format(coord_valid))

    mwu_s, mwu_p = mannwhitneyu(error_training, error_valid)
    if (mwu_p > 0.05):
        plt.text(max_xlim * 0.8, (min_ylim + max_ylim) * 0.5,
                 'Validated (P={:.5f}'.format(mwu_p) + '>0.05, U={:.2f})'.format(mwu_s))
    else:
        plt.text(max_xlim * 0.8, (min_ylim + max_ylim) * 0.5,
                 'Invalid (P={:.5f}'.format(mwu_p) + '<0.05, U={:.2f})'.format(mwu_s))

    plt.xlabel('Root mean square error')
    plt.ylabel('Frequency')
    if d_data == 0:
        d_data = [0]
    if len(cdo) != len(d_data):
        if mode == 0:
            filename = folder + "/" + "error_dist_accept" + "_slice_" + str(_slice) + ".pdf"
            plt.title(
                'Used ' + str(len(error_valid)) + ' accepted from last ' + str((params["count_iter"] - _slice)) + ' iterations')
        else:
            filename = folder + "/" + "error_dist_best" + "_slice_" + str(_slice) + ".pdf"
            plt.title('Used ' + str(len(error_valid)) + ' best from last ' + str((params["count_iter"] - _slice)) + ' iterations')
    else:
        filename = folder + "/" + "error_dist_all" + "_slice_" + str(_slice) + ".pdf"
        plt.title('Used ' + str(len(error_valid)) + ' all from last ' + str((params["count_iter"] - _slice)) + ' iterations')

   # plt.show()
    f.savefig(filename)

def get_params_abcdeini(ini_fm):
    params = {}
    with open(ini_fm, "r") as fm:
        for line in fm:
            if (line.startswith("[default_settings]")):
                break
            if (line.startswith('count_iter')):
                params['count_theta'] = int(line.split('=')[1])
            if (line.startswith('lbound')):
                params['lbound'] = [float() for _ in line.split('=')[1].split(';')]
            if (line.startswith('hbound')):
                params['hbound'] = [float() for _ in line.split('=')[1].split(';')]

    params['count_iter'] = None
    params['num_log_param'] = len(params['hbound']) + 3
    params['count_param'] = len(params['hbound'])
    params['delta_index'] = len(params['hbound']) + 2
    params["size_training"] = 1200
    params["size_valid"] = 300
    return(params)

if __name__ == "__main__":
  #  folder_paths = ["data/09_03/v3/with_snp", "data/09_03/v4/with_snp", "data/09_03/v4/without_snp"]
    ini_pp = sys.argv[1]
    folder = os.path.dirname(ini_pp)
#    folder_paths = ["data/09_03/v4/with_snp"]
    #1600 - 1333 and 333
    log_result_name = "log_result.txt"
    log_iter_name = "log_iteration.txt"
    log_deeps_name = "log_deeps"
    # params = {"num_log_param": 7,  #length of lbound + 1
              # "count_theta": 8,
              # "lbound": [1, 10, 0.005, 0.005],
              # "hbound": [7, 20, 1, 1],
              # "size_training": 1216,
              # "size_valid": 284,
              # "count_param": 4, #length of lbound
              # "delta_index": 6, #length of lbound + 2
              # "count_iter": None}
    params = get_params_abcdeini(os.path.abspath(os.path.join(folder_paths[0], ini_pp)))
    print(params)

    log_result = os.path.abspath(os.path.join(folder, log_result_name))
    log_deeps = os.path.abspath(os.path.join(folder, log_deeps_name))
    log_iter = os.path.abspath(os.path.join(folder, log_iter_name))
     #  plot_graphics_evolution_param(log_result, params)

    result_data, deep_data, iter_data, count_iter = read_logs(log_result, log_deeps, log_iter)
    params["count_iter"] = count_iter

    count_iter = plot_graphics_evolution_param(log_result, params, isBounds=True)

      #  plot_graphics_evolution_param_best(log_result, params)
     #   plot_graphics_evolution_param_best(log_result, params, isBounds=True)

    accept_deep_output, best_deep_output = plot_table(folder,
                                                          params,
                                                          log_result,
                                                          result_data,
                                                          deep_data,
                                                          iter_data)
    print("all")
    plot_error(folder, params, deep_data, deep_data, 0)
    print("accept")

    plot_error(folder, params, accept_deep_output, deep_data, 1)
    for i in range(0, params["count_iter"], 10):
        print("best = ", i)
        plot_error(folder, params, best_deep_output, deep_data, i, 1)
