
import mooseutils
import os
from random import randint

input_file = "initialization.i"
mpi=1
executeable="../fenix-opt"
supress_output = False



particles_per_element = [10, 100, 1000, 10000]

num_trials=10
mpi=24
top_folder = "particle_study"

if not os.path.exists(top_folder):
  os.mkdir(top_folder)

for i in range(num_trials):
     curr_dir = f"{top_folder}/run_{i:d}"

     if not os.path.exists(curr_dir):
          os.mkdir(curr_dir)

     for n in particles_per_element:
          a = ["-i", input_file,
               "--allow-test-objects",
               "Mesh/gmg/nx=100",
               f"Outputs/csv/file_base={curr_dir}/{n:d}_ppe",
               f"UserObjects/initializer/seed={randint(0,1329408):d}",
               f"GlobalParams/particles_per_element={n:d}"
               ]
          mooseutils.run_executable(executeable, *a, mpi=mpi, suppress_output=supress_output)
