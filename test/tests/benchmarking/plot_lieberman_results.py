import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np
import pandas as pd

plt.rcParams['mathtext.fontset'] = 'stix'
plt.rcParams['font.family'] = 'STIXGeneral'
plt.rcParams['legend.frameon'] = False

font = 18
plt.rcParams['legend.fontsize'] = font-2
plt.rcParams['axes.titlesize'] = font
plt.rcParams['axes.labelsize'] = font

plt.rcParams['xtick.labelsize'] = font-1
plt.rcParams['ytick.labelsize'] = font-1

# overall figure
plt.rcParams['figure.frameon'] = False

# Resetting default matplotlib parameters
linewidth = 1.5
plt.rcParams['lines.linewidth'] = 3

# axes
plt.rcParams['axes.linewidth'] = linewidth
plt.rcParams['axes.titlesize'] = font
plt.rcParams['axes.labelsize'] = font
plt.rcParams['axes.labelpad'] = 4  # default is 4

# ticks
plt.rcParams['xtick.direction'] = 'in'
plt.rcParams['ytick.direction'] = 'in'
plt.rcParams['xtick.labelsize'] = font-1
plt.rcParams['ytick.labelsize'] = font-1
plt.rcParams['lines.markersize'] = 8

plt.rcParams['xtick.major.size'] = 10 / 1.5 # Length of major ticks on x-axis
plt.rcParams['xtick.minor.size'] = 5  / 1.5 # Length of minor ticks on x-axis
plt.rcParams['ytick.major.size'] = 10 / 1.5 # Length of major ticks on y-axis
plt.rcParams['ytick.minor.size'] = 5  / 1.5 # Length of minor ticks on y-axis

plt.rcParams['xtick.major.width'] = linewidth / 1.5
plt.rcParams['ytick.major.width'] = linewidth / 1.5
plt.rcParams['xtick.minor.width'] = linewidth / 1.5
plt.rcParams['ytick.minor.width'] = linewidth / 1.5

plt.rcParams['xtick.top'] = True
plt.rcParams['xtick.bottom'] = True
plt.rcParams['ytick.left'] = True
plt.rcParams['ytick.right'] = True

# legend
plt.rcParams['legend.frameon'] = False
#########################################
#           output file names           #
#########################################
# file parts
file_base = 'lieberman'
ray_data_str = 'ray_data_'
potential_str = 'potential_'
file_end = '.csv'
initial_qualifier = '_i_'
final_qualifier = '_f_'
# file names
particle_population_file = file_base + file_end
initial_ray_data_file = file_base + initial_qualifier + ray_data_str + '0001' + file_end
final_ray_data_file = file_base + final_qualifier + ray_data_str + '0261' + file_end
initial_potential_file = file_base + initial_qualifier + potential_str + '0001' + file_end
final_potential_file = file_base + final_qualifier + potential_str + '0261' + file_end

# Plotting results for the global particle count
xmin = 0
xmax = 2.5
ymin = 80
ymax = 100
fig, ax = plt.subplots()
im = plt.imread("lieberman_population.png")
df = pd.read_csv(particle_population_file)

ax.set_xticks([xmin, xmax])
ax.set_yticks([ymin, ymax])
ax.set_zorder(2)
ax.set_facecolor('none')
ax.set_ylim(bottom=ymin, top=ymax)
ax.set_xlim(left=xmin, right=xmax)
ax.set_title('Number', y=1)
ax.set_ylabel("$\mathcal{N}(t)$", rotation=0, labelpad=-15)
ax.set_xlabel("Time [s $\\times 10^{-8}$]", labelpad=-15)
ax.imshow(im, extent=[xmin, xmax, ymin, ymax], aspect='auto',  cmap='gray')
ax.plot(df["time"] * 1e8, df["ray_count"], '--')
ax.tick_params(axis='both', which='major', pad=10)
plt.savefig('lieberman_population_comparison.png', format='png', bbox_inches='tight')
# plt.show()



# Plotting results for the evolution of the electrostatic potential
xmin = 0
xmax = 0.1
ymin = 0
ymax = 2.5e5
fig, ax = plt.subplots()
im = plt.imread("lieberman_potential.png")

ax.set_xticks([xmin, xmax])
ax.set_yticks([ymin, ymax])
ax.set_zorder(2)
ax.set_facecolor('none')
ax.set_ylim(bottom=ymin, top=ymax)
ax.set_xlim(left=xmin, right=xmax)
df = pd.read_csv(initial_potential_file)
ax.plot(df["x"], df["phi"], '--', label="t = 0 [s]")


rect = patches.Rectangle((0.04, 1.56e5), 0.025, 1.5e4, linewidth=1, edgecolor='w', facecolor='w')
ax.text(0.04, 1.56e5, "$t=2.5\\times 10^{-8}$ [s]", fontsize=15)
ax.add_patch(rect)

df = pd.read_csv(final_potential_file)
ax.plot(df["x"], df["phi"], '--',label="$t=2.5\\times 10^{-8}$ [s]")
ax.legend()
ax.set_title('Potential', y=1)
ax.set_ylabel("$\Phi(x)$", rotation=0, labelpad=-30)
ax.set_xlabel("x", labelpad=-20)
ax.imshow(im, extent=[xmin, xmax, ymin, ymax], aspect='auto')
ax.tick_params(axis='both', which='major', pad=10)
plt.savefig('lieberman_potential_comparison.png', format='png', bbox_inches='tight')

# Plotting particle kinetic data
ymin = -1e6
ymax = 1e6
xmin = 0
xmax = 0.1
fig, ax = plt.subplots()
im = plt.imread("lieberman_vdf.png")

ax.set_xticks([xmin, xmax])
ax.set_yticks([ymin, ymax])
ax.set_zorder(2)
ax.set_facecolor('none')
ax.set_ylim(bottom=ymin, top=ymax)
ax.set_xlim(left=xmin, right=xmax)
ax.set_title('Phase Space', y=1)
df = pd.read_csv(initial_ray_data_file)
ax.plot(df["x"], df["v_x"], '.', markersize=3,  label="t = 0 [s]")
# ax.plot(df["mesh_model_coordinates:0"], df["phi"], label="t = 0 [s]")
df = pd.read_csv(final_ray_data_file)
ax.plot(df["x"], df["v_x"], '.', markersize=3, label="$t=2.5\\times 10^{-8}$ [s]")
# ax.set_title('Potential')
ax.set_ylabel("$v_x$ [m/s]", rotation=0)
ax.set_xlabel("$x$", labelpad=-20)
ax.imshow(im, extent=[xmin, xmax, ymin, ymax], aspect='auto', cmap='gray')
ax.tick_params(axis='both', which='major', pad=10)

ax.legend(markerscale=5, handletextpad=0.05)
plt.savefig('lieberman_vdf_comparison.png', format='png', bbox_inches='tight')
# plt.show()

