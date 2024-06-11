import pandas as pd
import matplotlib.pyplot as plt

# Charger le fichier Excel
file_path = 'timing_results.xlsx'
df = pd.read_excel(file_path)

# Extraire les données du dataframe
n_values = df['Run: 100 fois'].tolist()
sequential = df['Seq'].tolist()
parallel_k1 = df['Paral k=1'].tolist()
parallel_k2 = df['Paral k=2'].tolist()
parallel_k3 = df['Paral k=3'].tolist()
parallel_k4 = df['Paral k=4'].tolist()
parallel_k5 = df['Paral k=5'].tolist()
parallel_k6 = df['Paral k=6'].tolist()
parallel_k7 = df['Paral k=7'].tolist()

# Créer le graphique
plt.figure(figsize=(12, 6))

# Tracer les données avec des marqueurs et des couleurs spécifiées
plt.plot(n_values, sequential, label='Sequential', color='b', marker='o')
plt.plot(n_values, parallel_k1, label='Parallel k=1', color='g', marker='o')
plt.plot(n_values, parallel_k2, label='Parallel k=2', color='r', marker='o')
plt.plot(n_values, parallel_k3, label='Parallel k=3', color='c', marker='o')
plt.plot(n_values, parallel_k4, label='Parallel k=4', color='m', marker='o')
plt.plot(n_values, parallel_k5, label='Parallel k=5', color='y', marker='o')
plt.plot(n_values, parallel_k6, label='Parallel k=6', color='k', marker='o')
plt.plot(n_values, parallel_k7, label='Parallel k=7', color='orange', marker='o')

plt.xlabel('Nombre Premier jusqu\'a n')
plt.ylabel('Execution Time (seconds)')
plt.title('Temps d\'execution du Crible d\'Eratosthène')
plt.legend(loc='center left', bbox_to_anchor=(1, 0.5))

# Afficher les valeurs de l'axe des x avec un espacement égal et rotation pour meilleure lisibilité
plt.xticks(n_values, [str(val) for val in n_values])

plt.grid(True, which='both', linestyle='--', linewidth=0.5)
plt.tight_layout()
plt.show()