import os
import sys
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

script_dir = os.path.dirname(os.path.abspath(__file__))
project_root = os.path.dirname(script_dir)
csv_path = os.path.join(project_root, 'build', 'benchmark_results.csv')

if not os.path.exists(csv_path):
    print(f"Error: File {csv_path} not found!")
    sys.exit(1)

df = pd.read_csv(csv_path, names=['algorithm', 'vertices', 'duration'])

sns.set_theme()  

plt.figure(figsize=(12, 8), dpi=300)

sns.lineplot(data=df, x='vertices', y='duration', hue='algorithm', 
             marker='o', linewidth=2, markersize=8)

plt.xlabel('Количество вершин', fontsize=12)
plt.ylabel('Время выполнения (мс)', fontsize=12)
plt.title('Сравнение производительности алгоритмов на графе', fontsize=14, pad=20)

plt.yscale('log')

plt.grid(True, which="both", ls="-", alpha=0.2)

plt.legend(title='Алгоритмы', 
          title_fontsize=12,
          fontsize=10, 
          bbox_to_anchor=(1.05, 1), 
          loc='upper left')

for algorithm in df['algorithm'].unique():
    data = df[df['algorithm'] == algorithm]
    for x, y in zip(data['vertices'], data['duration']):
        plt.annotate(f'{y:.1f}ms', 
                    (x, y), 
                    textcoords="offset points", 
                    xytext=(0,10), 
                    ha='center',
                    fontsize=8)

plt.tight_layout()

plt.savefig(os.path.join(os.path.dirname(csv_path), 'performance_analysis.png'), 
            dpi=300, 
            bbox_inches='tight')
plt.figure(figsize=(12, 8), dpi=300)

sns.boxplot(x='algorithm', y='duration', data=df)
plt.xticks(rotation=45)
plt.xlabel('Алгоритм', fontsize=12)
plt.ylabel('Время выполнения (мс)', fontsize=12)
plt.title('Распределение времени выполнения по алгоритмам', fontsize=14)
plt.yscale('log')

plt.tight_layout()
plt.savefig(os.path.join(os.path.dirname(csv_path), 'algorithm_comparison.png'), 
            dpi=300)

print("Графики сохранены в директории build/")
