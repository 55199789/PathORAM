import subprocess
import os 
import json

process = subprocess.Popen(f"(cd result && rm -f *.json)", shell=True)
process = subprocess.Popen(f"(cd ../build && cmake .. && make -j)", shell=True)
process.wait()
for n_base in range(8, 26):
    n = 2**n_base 
    file_name = f"result/results_omap_{n}.txt"
    if os.path.exists(file_name):
        with open(file_name, 'r') as f:
            data = f.readlines()
            if data and data[-1].find("Finished") != -1:
                continue
    # cmd = f"../bin/PathORAMBenchmark  --benchmark_out={file_name} \
    #                                 --benchmark_filter=OMapDataFixture.*/OMap/{n}/*"
    cmd = f"../bin/PathORAM {n} > {file_name}"
    print(cmd)
    try:
        process = subprocess.Popen(cmd, shell=True)
        ret_code = process.wait()
        if ret_code != 0:
            raise Exception("Error")
    except Exception as e:
        print(e)
        # input("Press Enter to continue...")