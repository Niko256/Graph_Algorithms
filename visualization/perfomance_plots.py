import os
import sys
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from adjustText import adjust_text

script_dir = os.path.dirname(os.path.abspath(__file__))
project_root = os.path.dirname(script_dir)
csv_path = os.path.join(project_root, 'build', 'benchmark_results.csv')

if not os.path.exists(csv_path):
    print(f"Error: File {csv_path} not found!")
    sys.exit(1)

df = pd.read_csv(csv_path, names=['algorithm', 'vertices', 'duration'])

plt.style.use('dark_background')
sns.set_palette("husl")

plt.figure(figsize=(10, 6), dpi=300)

plt.gca().set_facecolor('#1C1C1C')
plt.gcf().set_facecolor('#1C1C1C')

markers = ['o', 's', 'D', '^', 'v']

sns.lineplot(data=df, x='vertices', y='duration', hue='algorithm', 
             style='algorithm', markers=markers, dashes=False, linewidth=1.5,
             markersize=8)

plt.xlabel('Number of Vertices', fontsize=12, color='white')
plt.ylabel('Execution Time (ms)', fontsize=12, color='white')
plt.title('Algorithm Performance Comparison', fontsize=14, pad=15, color='white')

plt.yscale('log')
plt.xscale('log')

plt.grid(True, which="both", ls="--", alpha=0.2, color='gray')

plt.tick_params(colors='white')

# Ось X (количество вершин)
x_ticks = [1000, 5000, 10000, 50000, 100000]
x_tick_labels = [f"{x}" for x in x_ticks]
plt.xticks(x_ticks, x_tick_labels, fontsize=10, color='white')

y_ticks = [1, 10, 100, 1000, 10000, 100000]
y_tick_labels = [f"{y}" for y in y_ticks]
plt.yticks(y_ticks, y_tick_labels, fontsize=10, color='white')

legend = plt.legend(title='Algorithms', title_fontsize=12, fontsize=10, 
                   bbox_to_anchor=(1.05, 1), loc='upper left')
legend.get_title().set_color('white')
for text in legend.get_texts():
    text.set_color('white')

plt.tight_layout()

plt.savefig(os.path.join(os.path.dirname(csv_path), 'performance_analysis.png'), 
            bbox_inches='tight', facecolor='#1C1C1C')

plt.show()
