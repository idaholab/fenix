
import matplotlib.pyplot as plt

def set_rcParams():
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
  plt.rcParams['lines.linewidth'] = linewidth

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
