from typing import List, Tuple
from os import path, curdir

def get_stats() -> List[str]:
    seq_stats = {}

    lines = []

    for thread in [0, 2, 4, 6]:
        for dim in [5, 7, 10, 12]:
            file_name = path.join(curdir, "stats/stats_{0}_{1}.csv".format(thread, dim))

            start_time, exec_time, end_time, total_time = get_mean_stats(path.realpath(file_name))

            line = "{0}, {1}, {2}, {3}, {4}, {5}".format(thread, dim, start_time, exec_time, end_time, total_time)

            if thread == 0:
                seq_stats[str(dim)] = exec_time
                
                line += ", 0\n"
            else:
                performance = seq_stats[str(dim)] /  exec_time

                line += ", {0}\n".format(performance)
            
            lines.append(line)
    
    return lines

def get_smallest_line(file_name: str) -> Tuple[str, float]:
    s_line = None
    s_time = None

    with open(file_name, "r") as f:
        for l in f.readlines():
            metrics = l.split(",")

            total = float(metrics[5])

            if not s_time or total < s_time:
                s_time = total

                s_line = l

    return s_line, s_time


def get_mean_stats(file_name: str) -> Tuple[float, float, float, float]:
    s_line = None
    s_time = None

    start_time = 0
    exec_time = 0
    end_time = 0
    total_time = 0

    with open(file_name, "r") as f:
        count = 0
        for l in f.readlines():
            count += 1

            if count == 1:
                continue 

            metrics = l.split(",")

            start_time += float(metrics[2])
            exec_time += float(metrics[3])
            end_time += float(metrics[4])
            total_time += float(metrics[5])

    d = count - 1

    return start_time/d, exec_time/d, end_time/d, total_time/d
            

def write_csv(lines: List[str]) -> None:

    file_name = path.join(curdir, "stats/result.csv")

    with open(file_name, "w") as f:

        f.write("threads, dimension, initialization_time, execution_time, termination_time, total_time, performance_gain\n")
        
        for line in lines:
            f.write(line)

if __name__ == "__main__":
    lines = get_stats()

    write_csv(lines)