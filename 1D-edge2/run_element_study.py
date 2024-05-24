
import mooseutils
import os
from random import randint

input_file = "initialization.i"
mpi=1
executeable="../fenix-opt"
supress_output = False


refine_levels=[0, 1, 2, 3]
particles_per_element = 1000

num_trials=10

top_folder = "element_study"

if not os.path.exists(top_folder):
  os.mkdir(top_folder)

for i in range(num_trials):
     curr_dir = f"{top_folder}/run_{i:d}"

     if not os.path.exists(curr_dir):
          os.mkdir(curr_dir)

     for level in refine_levels:
          a = ["-i", input_file,
               "--allow-test-objects",
               f"Mesh/uniform_refine={level:n}",
               f"Outputs/csv/file_base={curr_dir}/{level:d}_refinement",
               f"UserObjects/initializer/seed={randint(0,1329408):d}",
               f"particles_per_element={particles_per_element:d}"
               ]
          if level > 1:
               mpi = 20
          else:
               mpi = 8
          mooseutils.run_executable(executeable, *a, mpi=mpi, suppress_output=supress_output)
