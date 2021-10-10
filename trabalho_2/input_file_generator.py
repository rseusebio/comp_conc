import random 
import sys

def create_input_file(lines: int, block_size: int) -> None:
    f = open("input_file_{0}_{1}.txt".format(lines, block_size), "w")

    # f2 = open("output_file_{0}_{1}.txt".format(lines, block_size), "w")

    f.write("{0}\n".format(lines))

    for i in range(lines):
        l = list(range(i + 1, i + 1 + block_size))

        # f2.write(" ".join(list(map(lambda x : str(x), l))) + "\n")

        random.shuffle(l)

        l = list(map(lambda x : str(x), l))

        f.write(" ".join(l) + "\n")

        if (i + 1) % 1000 == 0:
            p = (i+1) / lines
            print("done {0}/{1} ({2}%)\n".format(i+1, lines, p*100))
    
    f.close()
    # f2.close()

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Args expected: <number_of_line> <block_size>")

        exit(-1)
    
    lines = int(sys.argv[1])

    block_size = int(sys.argv[2])

    create_input_file(lines, block_size)

