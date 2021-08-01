from typing import List, Tuple
from os import path, curdir

def get_stats() -> List[str]:

    seq_stats = {}

    lines = []

    for thread in [0, 2, 4]:
        for dim in [10**5, 10**7, 10**9]:
            file_name = path.join(curdir, "stats/stats_{0}_{1}.csv".format(thread, dim))

            value = get_mean_value(path.realpath(file_name))

            s_line = "{0}, {1}, {2}".format(thread, dim, value)

            if thread == 0:
                seq_stats[str(dim)] = value
                
                s_line += ", 0\n"
            else:
                performance = seq_stats[str(dim)] /  value

                s_line += ", {0}\n".format(performance)
            
            lines.append(s_line)
    
    return lines

def get_mean_value(file_name: str) -> Tuple[str, float]:

    sum = 0
    count = 0

    with open(file_name, "r") as f:
        for l in f.readlines():
            metrics = l.split(",")

            total = float(metrics[2])

            sum += total
            count += 1

    return sum / count

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
            

def write_csv(lines: List[str]) -> None:
    file_name = path.join(curdir, "stats/result.csv")
    with open(file_name, "w") as f:
        f.write("threads, dimension, execution_time, performance_gain\n")
        
        for line in lines:
            f.write(line)

if __name__ == "__main__":
    lines = get_stats()
    write_csv(lines)