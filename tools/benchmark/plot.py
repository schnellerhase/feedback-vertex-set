import csv
import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('data.csv')

for solver in df['solver'].unique():
    solver_df = df[df['solver'] == solver]
    solver_df = solver_df.sort_values(by=['track'])
    plt.plot(solver_df['track'], solver_df['time'], marker='x', linestyle='None', label=solver)

plt.gcf().set_size_inches(30,10)
plt.legend()
plt.ylabel('seconds')
plt.yscale('log')
plt.grid()
xticks = [i+1 for i in range(0, 200, 2)]
xlabels = [f"e_{str(t).zfill(3)}" for t in xticks]
plt.xticks(xticks, xlabels, rotation=90)
plt.xlim((0,200))
plt.axhline(y=1800)
plt.tight_layout()
plt.savefig(f'times_plot.png', dpi=200)
plt.clf()
