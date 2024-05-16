import mooseutils
import os
from random import randint

input_file = "initialization.i"
mpi=20
executeable="../fenix-opt"
supress_output = False



num_particles = [10, 100, 1000, 10000]

num_seeds = 10


for i in range(num_seeds):
     curr_dir = f"run_{i:d}"

     if not os.path.exists(curr_dir):
          os.mkdir(curr_dir)


     for n in num_particles:
          a = ["-i", input_file,
               "--allow-test-objects",
               f"UserObjects/study/particles_per_element={n:d}",
               f"Outputs/csv/file_base={curr_dir}/{n:d}_ppe",
               f"Samplers/sampler/seed={randint(0, 34912348):d}"
               ]
          mooseutils.run_executable(executeable, *a, mpi=mpi, suppress_output=supress_output)

