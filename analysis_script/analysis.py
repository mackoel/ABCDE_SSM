import matplotlib.pyplot as plt
import csv
import math
from scipy.stats import mannwhitneyu
import numpy as np

count_process = 1
processes = ["without snp", "with snp"]
log_result = "log_result.txt"
log_iter = "log_iteration.txt"
log_deeps = "log_deeps"
`num_log_param = 
count_thetha = 12
lbound=[1,10,0.005,0.005,0.005,0.005]
hbound=[7,20,10,1,1,10]
size_training = 526
size_valid = 132
count_param = 6
f = []
count_iter = 121
delta_index = 8
def set_param(x):
    for i in range(0, len(lbound)):
        a = (hbound[i] + lbound[i])/2.0
        b = (hbound[i] - lbound[i])/2.0
        x[i+1] = a + b * math.sin(float(x[i+1])) #i + 1 because 0 in x is element number             
    return x

def bounds(one, k):
    i = 0
    if k >= len(lbound):
        if k == delta_index:
            for data in one:
                a = (5.0 + 0.0)/2.0
                b = (5.0 - 0.0)/2.0
                one[i] = a + b * math.sin(float(one[i]))
                i += 1
        return one
    for data in one:
        a = (hbound[k] + lbound[k])/2.0
        b = (hbound[k] - lbound[k])/2.0
        one[i] = a + b * math.sin(float(one[i]))
        i += 1
    return one

def parse_deep_header(_str):
    l_list = _str.split(" ")
    header = []
    target = []
    for l in l_list:
        h = l.split(":") 
        if(len(h) == 2):
            header.append(h[0])
    for h in header:
        if "target" in h:
            target.append(h)
            
    return target

def parse_deep(_str):
    l_list = _str.split(" ")
    res = []
    for l in l_list:
        h = l.split(":") 
        if(len(h) == 2) and "target" in h[0]:
            res.append(h[1])
    return res
    
def create_header(folder, deep_header):
    filename = folder + "/" + "table.txt"

    deep_output = parse_deep_header(deep_header)
    with open(filename, "a") as csvfile:
       writer = csv.writer(csvfile,  delimiter=";")
       tab = ["element_number"]
       for i in range(count_param):
           tab.append("accept_param[" + str(i) + "]")
       tab.append("w")
       tab.append("error")
       tab.append("delta")
       for i in range(count_param):
           tab.append("accept_param_bounds[" + str(i) + "]")
       for i in range(count_param):
           tab.append("curr_param[" + str(i) + "]")   
       for i in range(count_param):
           tab.append("curr_param_bounds[" + str(i) + "]")
       tab.append("iter")
       tab.append("element_number")
       tab.append("seed")
     #  tab.append("deepoutput")
       tab.append("original_alpha")
       tab.append("alpha")
       tab.append("accept")
       for target in deep_output:
           tab.append(target)
       #tab = ["element_number", "n", "l", "l1", "l2", "l3", "l4", "w", "error", "delta", "n_b", "l_b", "l1_b", "l2_b", "l3_b", "l4_b",  "n_deep", "l_deep", "l1_deep", "l2_deep", "l3_deep", "l4_deep", "n_deep_b", "l_deep_b", "l1_deep_b", "l2_deep_b", "l3_deep_b", "l4_deep_b",  "iter", "element_number", "seed", "deepoutput", "original_alpha", "alpha", "accept"]
       writer.writerow(tab)
       
def create_table(folder, x, x_b, x_deep, x_deep_b, iteration, element_number, curr_seed, d, o_alpha, alpha, accept):
    filename = folder + "/" + "table.txt"
    with open(filename, "a") as csvfile:
       writer = csv.writer(csvfile,  delimiter=";")
       deep_res = parse_deep(d)
       tab = [x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7] ,x[8], x[9], int(x_b[1]), int(x_b[2]), x_b[3], x_b[4], x_b[5], x_b[6], x_deep[1], x_deep[2], x_deep[3], x_deep[4], x_deep[5], x_deep[6],  int(x_deep_b[1]), int(x_deep_b[2]), x_deep_b[3], x_deep_b[4], x_deep_b[5], x_deep_b[6], float(iteration), float(element_number), float(curr_seed), o_alpha, alpha, accept]
       for res in deep_res:
           tab.append(res)
       writer.writerow(tab)
       
def plot_graph_evolution_param(folder, file, isBounds = False):
    x_data = [] 
    start_take_param = 6#we skip element number, because there is no need to look for matches here
    with open(file) as f:
        for line in f:
            word_list = line.split()
            if(len(word_list) > 3):
                num_list = [num for num in word_list[start_take_param:len(word_list):3]]
            else:
                num_list = "iteration = " + word_list[2]
            x_data.append(num_list)
    param = []
    for num_param in range(0,num_log_param):
       param.append([])

    for x in x_data:
        if not "iteration = " in x:
            k = 0
            for res in x:
                param[k].append(res)
                k = k + 1
    number_param = 0
    for p in param:
        len_p = len(p)
        for thetha in range(0, count_thetha):
            fig = plt.figure()
            one = [float(x) for x in p[thetha:len_p:count_thetha]]
            if(isBounds == True):
                one = bounds(one, number_param)
                filename = folder + "/" + "param[" +str(number_param) +"]" + "_" + "individ[" + str(thetha) + "]" +"_"+ "bounds.pdf"
            else: 
                filename = folder + "/" + "param[" +str(number_param) +"]" + "_" + "individ[" + str(thetha) + "]" ".pdf"
                
            plt.plot(one)
            plt.show()
            fig.savefig(filename)
            
        number_param += 1
        
def plot_graph_evolution_param_iter(folder, file, isBounds = False):
    x_data = [] 
    start_take_param = 6#we skip element number, because there is no need to look for matches here
    with open(file) as f:
        for line in f:
            word_list = line.split()
            if(len(word_list) > 3):
                num_list = [num for num in word_list[start_take_param:len(word_list):3]]
            else:
                num_list = "iteration = " + word_list[2]
            x_data.append(num_list)
    param = []
    for num_param in range(0, count_iter+1):
       param.append([])

    k = -1
    for x in x_data:
        if "iteration = " in x:
            k += 1
            continue
        for res in x:
            param[k].append(res)
    number_param = 0#its iter here
    k = 0
    for p in param:
        len_p = len(p)
        for thetha in range(0, num_log_param):
            fig = plt.figure()
            one = [float(x) for x in p[thetha:len_p:num_log_param]]
            if(isBounds == True):
                one = bounds(one, number_param)
                filename = folder + "/" + "iter[" +str(number_param) +"]" + "_" + "param[" + str(thetha) + "]" +"_"+ "bounds.pdf"
            else: 
                filename = folder + "/" + "iter[" +str(number_param) +"]" + "_" + "param[" + str(thetha) + "]" ".pdf"
                
            plt.hist(one)
            plt.show()
            fig.savefig(filename)
            
        number_param += 1

def plot_graph_evolution_param_best(folder, file, isBounds = False):
    x_data = [] 
    best_error = 2.0
    append_num_list = ["iteration = "]
    error_index = 7
    start_take_param = 6#we skip element number, because there is no need to look for matches here
    with open(file) as f:
        for line in f:
            word_list = line.split()
            if(len(word_list) > 3):
                num_list = [num for num in word_list[start_take_param:len(word_list):3]]
            else:
                best_error = 2.0
                if("iteration = " not in append_num_list[0]):
                    x_data.append(append_num_list[0])
                continue
            new_error = float(num_list[error_index])
            best_error = float(best_error)
            if(new_error <= best_error):
                append_num_list[0] = num_list
                best_error = num_list[error_index]

    param = []
    for num_param in range(0,num_log_param):
       param.append([])

    for x in x_data:
        if not "iteration = " in x:
            k = 0
            for res in x:
                param[k].append(res)
                k = k + 1
    number_param = 0
    for p in param:
        fig = plt.figure()
        one = p
        if(isBounds == True):
            one = bounds(one, number_param)
            filename = folder + "/" + "param[" +str(number_param) +"]" + "_"+ "bounds_best.pdf"
        else: 
            filename = folder + "/" + "param[" +str(number_param) +"]" + "_" + "_best.pdf"
        plt.plot(one)
        plt.show()
        fig.savefig(filename)
            
        number_param += 1
    
def plot_table(folder, l_r_file, x_data, d_data, i_data):
    create_header(folder, d_data[0])
    find = False
    k = 0
    accept_deep_output = []
    chain_data = []
    best_error = 2.0
    append_num_list = ["iteration = "]
    error_index = 7
    start_take_param = 3#we skip element number, because there is no need to look for matches here
    best_deep_output = []
    with open(l_r_file) as f:
        for line in f:
            word_list = line.split()
            if(len(word_list) > 3):
                num_list = [num for num in word_list[start_take_param:len(word_list):3]]
            else:
                best_error = 2.0
                if("iteration = " not in append_num_list[0]):
                    chain_data.append(append_num_list[0])
                continue
            new_error = float(num_list[error_index])
            best_error = float(best_error)
            if(new_error <= best_error):
                append_num_list[0] = num_list
                best_error = num_list[error_index]
                
    for x in x_data:
        if "iteration = " in x:
            iteration = x.split()[2]
        else:
            element_number = x[0]
            for d in d_data:#in deep logs parallels output - so search by ids first
                curr_iter = d.split("iteration = ",1)[1].split(" ")[0]
                curr_element_number = d.split("element number = ", 1)[1].split(" ")[0]
                curr_seed = d.split("seed = ", 1)[1].split(" ")[0]
                сurr_deep_output = d
                if float(element_number) == float(curr_element_number) and float(iteration) == float(curr_iter):
                  #  print("true")
                 #   print(element_number, iteration)
                    find = True
                    break
            for i in  range(k, len(i_data)):
                if("INIT" in i_data[i] or "RUN" in i_data[i] or "RUN_APPROXIMATE" in i_data[i] or "iteration = " in i_data[i] or "element number = " in i_data[i] or "delta = " in i_data[i] or "error = " in i_data[i]):#delete
                    i += 1
                else:
                    check_param = [curr_element_number, i_data[i], i_data[i+1], i_data[i+2], i_data[i+3], i_data[i+4], i_data[i+5]]
                    i += 8
                    if "original alpha = " in i_data[i]:
                        o_alpha = i_data[i].split("original alpha = ",1)[1].rstrip()
                        alpha = i_data[i+1].split("alpha = ", 1)[1].rstrip()
                        accept = i_data[i+2].rstrip()
                        i += 3

                    else:
                        o_alpha = "init"
                        alpha = "init"
                        accept = "accept"
                    if(accept == "accept" or accept == "accept alpha"):
                        accept_deep_output.append(сurr_deep_output)
                    for c in chain_data:
                        if(c == x):
                           best_deep_output.append(сurr_deep_output)
                           break
                        
                    k = i
                    break
                    
            if(find == False):
                create_table(folder, ["null","null","null","null","null","null","null"], set_param(x), iteration, element_number, -1, "null", "null", "null", "null")
            else:
                x_param = []
                for i in range(0, len(x) - 3):
                    x_param.append(x[i])
                create_table(folder, x, set_param(x_param), check_param, set_param(check_param), iteration, element_number, curr_seed, сurr_deep_output.rstrip(), o_alpha, alpha, accept)
                find = False
    return accept_deep_output, best_deep_output

def read_logs(l_r_file, l_d_file, l_i_file):
    start_take_param = 3#because we need element number from log result
    #read log result 
    x_data = []
    with open(l_r_file) as f:
        for line in f:
            word_list = line.split()
            if(len(word_list) > 3):
                num_list = [num for num in word_list[start_take_param:len(word_list):3]]
            else:
                num_list = "iteration = " + word_list[2]
            x_data.append(num_list)
    
    #read log deeps        
    d_data = []
    with open(l_d_file) as f:
        for line in f:
            d_data.append(line)
    i = 0
    while i < len(d_data):
        if "/home/" in d_data[i]:
            del d_data[i]
        else:
            i += 1 
            
    #read log iter       
    i_data = []
    with open(l_i_file) as f:
        for line in f:
            i_data.append(line)
    
    return (x_data, d_data, i_data)

def plot_error(folder, cdo, _slice = 0, mode = 0):
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
        error_training.append(float(d.split("target[1]:", 1)[1].split(" ")[0]) / size_training)
        error_valid.append(float(d.split("target[21]:", 1)[1].split(" ")[0]) / size_valid)
    print(len(error_valid))
    print(len(error_training))
    f = plt.figure(figsize=(20,10))
    plt.hist(error_training, bins = 30, alpha=0.5, label='error_training')
    plt.hist(error_valid, bins = 30, alpha=0.5, label='error_valid')
    plt.legend(loc='upper right')
    plt.text(0,0, mannwhitneyu(error_training, error_valid))
 
    plt.axvline(np.mean(error_training), color='k', linestyle='dashed', linewidth=1)

    min_ylim, max_ylim = plt.ylim()
    plt.text(np.mean(error_training)*1.1, max_ylim*0.9, 'Mean: {:.2f}'.format(np.mean(error_training)))
    
    plt.axvline(np.mean(error_valid), color='g', linestyle='dashed', linewidth=1)

    min_ylim, max_ylim = plt.ylim()
    plt.text(np.mean(error_valid)*1.1, max_ylim*0.9, 'Mean: {:.2f}'.format(np.mean(error_valid)))
    
    plt.show()   
    
    if(len(cdo) != len(d_data)):
        if(mode == 0):
            filename = folder + "/" + "error_dist_accept" + "_slice_ " + str(_slice) + ".pdf"
        else:
            filename = folder + "/" + "error_dist_best" + "_slice_ " + str(_slice) + ".pdf"
    else:
        filename = folder + "/" + "error_dist_all"+ "_slice_ " + str(_slice) + ".pdf"
    f.savefig(filename)


for process in processes:
    folder = process
    l_r_file = folder + "/" + log_result   
    l_d_file = folder + "/" + log_deeps
    l_i_file = folder + "/" + log_iter
    
    #plot as graphs according to what is in log result
    #plot_graph_evolution_param(folder, l_r_file)
   # plot_graph_evolution_param(folder, l_r_file, True)
    
    plot_graph_evolution_param_best(folder, l_r_file)
    plot_graph_evolution_param_best(folder, l_r_file, True)

   # plot_graph_evolution_param_iter(folder, l_r_file)
  #  plot_graph_evolution_param_iter(folder, l_r_file, True)


    #read logs
    x_data, d_data, i_data = read_logs(l_r_file, l_d_file, l_i_file)
    #plot table
    accept_deep_output, best_deep_output = plot_table(folder, l_r_file, x_data, d_data, i_data)
    #plot error
    print("all")
    plot_error(folder, d_data, 0)
    print("accept")

    plot_error(folder, accept_deep_output, 0, 0)
    for i in range(0, 120, 10):
        print("best = ", i)
        plot_error(folder, best_deep_output, i, 1)

    

   
