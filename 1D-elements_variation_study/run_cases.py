import mooseutils
import os
from random import randint

input_file = "initialization.i"
mpi=1
executeable="../fenix-opt"
supress_output = False



num_elems = [10, 100, 1000]

ppe = 1000

num_seeds = 10

top_folder = f"{ppe:d}ppe"

if not os.path.exists(top_folder):
  os.mkdir(top_folder)

for i in range(num_seeds):
     curr_dir = f"{top_folder}/run_{i:d}"

     if not os.path.exists(curr_dir):
          os.mkdir(curr_dir)

     for n in num_elems:
          a = ["-i", input_file,
               "--allow-test-objects",
               f"Mesh/gmg/nx={n:d}",
               f"Outputs/csv/file_base={curr_dir}/{n:d}_elems",
               f"Samplers/sampler/seed={randint(0, 34912348):d}",
               f"UserObjects/study/particles_per_elememt{ppe:d}"
               ]
          if n > 10:
               mpi = 20
          else:
               mpi = n
          mooseutils.run_executable(executeable, *a, mpi=mpi, suppress_output=supress_output)
