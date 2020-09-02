import matplotlib.pyplot as plt
import csv
import math

folder = "result/"
count_process = 1
processes = list(range(0,count_process))
log_result = "log_result.txt"
log_iter = "log_iteration.txt"
log_deeps = "log_deeps"
num_log_param = 9
count_thetha = 12
lbound=[1,10,0.005,0.005,0.005,0.005]
hbound=[7,20,10,1,1,10]
size_training = 526
size_valid = 132

def set_param(x):
    for i in range(0, len(lbound)):
        a = (hbound[i] + lbound[i])/2.0
        b = (hbound[i] - lbound[i])/2.0
        x[i+1] = a + b * math.sin(float(x[i+1]))              
    return x
    
def create_header():
    with open("table.txt", "a") as csvfile:
       writer = csv.writer(csvfile,  delimiter=";")
       tab = ["element_number", "n", "l", "l1", "l2", "l3", "l4", "w", "error", "delta",  "n_deep", "l_deep", "l1_deep", "l2_deep", "l3_deep", "l4_deep",  "iter", "element_number", "seed", "deepoutput", "original_alpha", "alpha", "accept"]
       writer.writerow(tab)
       
def create_table(check_param, x, iteration, element_number, curr_seed, d, o_alpha, alpha, accept):
    with open("table.txt", "a") as csvfile:
       writer = csv.writer(csvfile,  delimiter=";")
       tab = [x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7] ,x[8], x[9], check_param[0], check_param[1], check_param[2], check_param[3], check_param[4], check_param[5], float(iteration), float(element_number), float(curr_seed), d, o_alpha, alpha, accept]
       writer.writerow(tab)
       
def plot_graph(file, process):
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
            plt.plot(one)
            name = str(number_param) + "_" +str(thetha) +"_process-"+str(process) +".pdf"
            plt.show()
            fig.savefig(name)
            
        number_param += 1
        
def plot_table(x_data, d_data, i_data):
    create_header()
    find = False
    k = 0
    accept_deep_output = []
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
                    find = True
                    break
            if(int(iteration) >= 0):  #-1 iteraition will give output 
                for i in  range(k, len(i_data)):
                    if("choice" in i_data[i] or "crossover" in i_data[i] or "mutation" in i_data[i] or "INIT" in i_data[i] or "RUN" in i_data[i] or "RUN_APPROXIMATE" in i_data[i]):#delete
                        i += 1
                    else:
                        check_param = [curr_element_number, i_data[i], i_data[i+1], i_data[i+2], i_data[i+3], i_data[i+4], i_data[i+5]]
                        i += 7
                        if("error" in i_data[i]):#in new launch - skip error and take
                            i += 1
                        if("original alpha" not in i_data[i]):
                            i -= 1
                        o_alpha = i_data[i].split("original alpha = ",1)[1].rstrip()
                        alpha = i_data[i+1].split("alpha = ", 1)[1].rstrip()
                        accept = i_data[i+2].rstrip()
                        #add delta and error 
                        if(accept == "accept"):
                            accept_deep_output.append(сurr_deep_output)
                        i += 3
                        k = i
                        break
                    
            if(find == False):
                create_table(["null","null","null","null","null","null","null"], set_param(x), iteration, element_number, -1, "null", "null", "null", "null")
            else:
                create_table(set_param(check_param), set_param(x), iteration, element_number, curr_seed, сurr_deep_output.rstrip(), o_alpha, alpha, accept)
                find = False

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


def plot_error(x_data, d_data):
    error_training = []
    error_valid = []
    for x in x_data:
        if "iteration = " in x:
            iteration = x.split()[2]
        else:
            element_number = x[0]
            for d in d_data:
                curr_iter = d.split("iteration = ",1)[1].split(" ")[0]
                curr_element_number = d.split("element number = ", 1)[1].split(" ")[0]
                if float(element_number) == float(curr_element_number) and float(iteration) == float(curr_iter):
                    error_training.append(float(d.split("target[1]:", 1)[1].split(" ")[0]) / size_training)
                    error_valid.append(float(d.split("target[21]:", 1)[1].split(" ")[0]) / size_valid)
                    break
                   
    f = plt.figure()
    plt.hist(error_training, alpha=0.5, label='error_training')
    plt.hist(error_valid, alpha=0.5, label='error_valid')
    plt.legend(loc='upper right')
    plt.show()    
    f.savefig("error_dist.pdf")

for process in processes:
    folder_file = folder + str(process)
    l_r_file = folder_file + "/" + log_result   
    l_d_file = folder_file + "/" + log_deeps
    l_i_file = folder_file + "/" + log_iter
    
    #plot as graphs according to what is in log result
    plot_graph(l_r_file, process)
    #read logs
    x_data, d_data, i_data = read_logs(l_r_file, l_d_file, l_i_file)
    #plot table
    plot_table(x_data, d_data, i_data)
    #plot error
    plot_error(x_data, d_data)

   