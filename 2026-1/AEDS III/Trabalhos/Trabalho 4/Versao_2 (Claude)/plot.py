import os
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np

# =============================================================
# ESTILO GLOBAL
# =============================================================
plt.rcParams.update({
    "figure.facecolor":     "#FAFAFA",
    "axes.facecolor":       "#FFFFFF",
    "axes.edgecolor":       "#CCCCCC",
    "axes.linewidth":       0.8,
    "axes.grid":            True,
    "axes.grid.which":      "both",
    "axes.spines.top":      False,
    "axes.spines.right":    False,
    "grid.color":           "#E5E5E5",
    "grid.linewidth":       0.6,
    "grid.linestyle":       "--",
    "xtick.color":          "#555555",
    "ytick.color":          "#555555",
    "xtick.labelsize":      10,
    "ytick.labelsize":      10,
    "axes.labelsize":       11,
    "axes.labelcolor":      "#333333",
    "axes.titlesize":       13,
    "axes.titleweight":     "semibold",
    "axes.titlecolor":      "#1A1A1A",
    "axes.titlepad":        12,
    "legend.frameon":       True,
    "legend.framealpha":    0.92,
    "legend.edgecolor":     "#DDDDDD",
    "legend.fontsize":      9,
    "legend.title_fontsize": 9,
    "lines.linewidth":      2.0,
    "lines.markersize":     6,
    "font.family":          "sans-serif",
    "figure.dpi":           150,
    "savefig.dpi":          300,
    "savefig.bbox":         "tight",
    "savefig.facecolor":    "#FAFAFA",
})

# Paleta consistente por algoritmo
PALETTE = {
    "PartitionProblem":  "#5B8FF9",
    "SimulatedAnnealing": "#F6903D",
    "GeneticAlgorithm":  "#5AD8A6",
}
MARKERS = {
    "PartitionProblem":  "o",
    "SimulatedAnnealing": "s",
    "GeneticAlgorithm":  "^",
}

def alg_style(alg):
    color  = PALETTE.get(alg, "#999999")
    marker = MARKERS.get(alg, "o")
    return dict(color=color, marker=marker, markerfacecolor="white",
                markeredgewidth=1.8, linewidth=2.0, markersize=6)

def fmt_size(n):
    if n >= 1_000_000: return f"{n//1_000_000}M"
    if n >= 1_000:     return f"{n//1_000}k"
    return str(n)

# =============================================================
# LEITURA
# =============================================================
os.makedirs("Results/graficos", exist_ok=True)

df = pd.read_csv("Results/TodosResultados.txt").dropna()
df["Algoritmo"]     = df["Algoritmo"].astype(str)
df["Tamanho"]       = df["Tamanho"].astype(int)
df["TempoExecucao"] = df["TempoExecucao"].astype(float)
df["Qualidade"]     = df["Qualidade"].astype(float)
df = df.sort_values(["Tamanho"])

sizes = sorted(df["Tamanho"].unique())
algs  = df["Algoritmo"].unique()

# =============================================================
# 1) POR TAMANHO — TEMPO
# =============================================================
for size in sizes:
    fig, ax = plt.subplots(figsize=(7, 4))
    sub = df[df["Tamanho"] == size]

    for alg in algs:
        d = sub[sub["Algoritmo"] == alg].reset_index(drop=True)
        if d.empty: continue
        ax.plot(range(1, len(d) + 1), d["TempoExecucao"],
                label=alg, **alg_style(alg))

    ax.set_title(f"Tempo de execução — instâncias de tamanho {fmt_size(size)}")
    ax.set_xlabel("Instância")
    ax.set_ylabel("Tempo (s)")
    ax.set_xticks(range(1, sub["Instancia"].nunique() + 1) if "Instancia" in sub else range(1, 11))
    ax.yaxis.set_major_formatter(ticker.FormatStrFormatter("%.4f"))
    ax.legend(title="Algoritmo", loc="best")
    fig.tight_layout()
    fig.savefig(f"Results/graficos/tempo_{size}.png")
    plt.close(fig)

# =============================================================
# 2) POR TAMANHO — FITNESS
# =============================================================
for size in sizes:
    fig, ax = plt.subplots(figsize=(7, 4))
    sub = df[df["Tamanho"] == size]

    for alg in algs:
        d = sub[sub["Algoritmo"] == alg].reset_index(drop=True)
        if d.empty: continue
        ax.plot(range(1, len(d) + 1), d["Qualidade"],
                label=alg, **alg_style(alg))

    ax.set_title(f"Qualidade (fitness) — instâncias de tamanho {fmt_size(size)}")
    ax.set_xlabel("Instância")
    ax.set_ylabel("Fitness  |sumA − sumB|")
    ax.yaxis.set_major_formatter(ticker.FuncFormatter(lambda x, _: f"{int(x):,}"))
    ax.legend(title="Algoritmo", loc="best")
    fig.tight_layout()
    fig.savefig(f"Results/graficos/fitness_{size}.png")
    plt.close(fig)

# =============================================================
# 3 & 4) GLOBAL — TEMPO e FITNESS (lado a lado)
# =============================================================
agg = (df.groupby(["Algoritmo", "Tamanho"])
         .agg(tempo_med=("TempoExecucao", "mean"),
              tempo_std=("TempoExecucao", "std"),
              fit_med=("Qualidade", "mean"),
              fit_std=("Qualidade", "std"))
         .reset_index())

fig, axes = plt.subplots(1, 2, figsize=(13, 5))
fig.suptitle("Comparação global entre algoritmos", fontsize=14,
             fontweight="semibold", color="#1A1A1A", y=1.01)

# — Tempo
ax = axes[0]
for alg in algs:
    d = agg[agg["Algoritmo"] == alg].sort_values("Tamanho")
    if d.empty: continue
    kw = alg_style(alg)
    ax.plot(d["Tamanho"], d["tempo_med"], label=alg, **kw)
    ax.fill_between(d["Tamanho"],
                    d["tempo_med"] - d["tempo_std"].fillna(0),
                    d["tempo_med"] + d["tempo_std"].fillna(0),
                    color=kw["color"], alpha=0.12)

ax.set_xscale("log")
ax.set_yscale("log")
ax.set_title("Tempo médio de execução")
ax.set_xlabel("Tamanho da instância (log)")
ax.set_ylabel("Tempo médio (s, log)")
ax.xaxis.set_major_formatter(ticker.FuncFormatter(lambda x, _: fmt_size(int(x))))
ax.legend(title="Algoritmo")

# — Fitness
ax = axes[1]
for alg in algs:
    d = agg[agg["Algoritmo"] == alg].sort_values("Tamanho")
    if d.empty: continue
    kw = alg_style(alg)
    ax.plot(d["Tamanho"], d["fit_med"], label=alg, **kw)
    ax.fill_between(d["Tamanho"],
                    d["fit_med"] - d["fit_std"].fillna(0),
                    d["fit_med"] + d["fit_std"].fillna(0),
                    color=kw["color"], alpha=0.12)

ax.set_xscale("log")
ax.set_title("Fitness médio  |sumA − sumB|")
ax.set_xlabel("Tamanho da instância (log)")
ax.set_ylabel("Fitness médio")
ax.xaxis.set_major_formatter(ticker.FuncFormatter(lambda x, _: fmt_size(int(x))))
ax.yaxis.set_major_formatter(ticker.FuncFormatter(lambda x, _: f"{int(x):,}"))
ax.legend(title="Algoritmo")

fig.tight_layout()
fig.savefig("Results/graficos/comparacao_global.png")
plt.close(fig)

# =============================================================
# 5) BOXPLOT FITNESS POR ALGORITMO (visão distribuição)
# =============================================================
fig, ax = plt.subplots(figsize=(10, 5))

x_pos = np.arange(len(sizes))
width = 0.25
offsets = np.linspace(-width, width, len(algs))

for idx, alg in enumerate(algs):
    data_per_size = [
        df[(df["Algoritmo"] == alg) & (df["Tamanho"] == s)]["Qualidade"].values
        for s in sizes
    ]
    bp = ax.boxplot(data_per_size,
                    positions=x_pos + offsets[idx],
                    widths=width * 0.8,
                    patch_artist=True,
                    notch=False,
                    showfliers=True,
                    flierprops=dict(marker="x", markersize=4,
                                   color=PALETTE.get(alg, "#999"),
                                   alpha=0.5),
                    medianprops=dict(color="#333333", linewidth=1.8),
                    boxprops=dict(facecolor=PALETTE.get(alg, "#999"),
                                  alpha=0.5,
                                  linewidth=0.8),
                    whiskerprops=dict(linewidth=0.8,
                                      color=PALETTE.get(alg, "#999")),
                    capprops=dict(linewidth=0.8,
                                  color=PALETTE.get(alg, "#999")))
    # entrada na legenda
    ax.plot([], [], color=PALETTE.get(alg, "#999"),
            linewidth=6, alpha=0.5, label=alg)

ax.set_xticks(x_pos)
ax.set_xticklabels([fmt_size(s) for s in sizes])
ax.set_xlabel("Tamanho da instância")
ax.set_ylabel("Fitness  |sumA − sumB|")
ax.set_title("Distribuição do fitness por algoritmo e tamanho")
ax.legend(title="Algoritmo", loc="upper left")
fig.tight_layout()
fig.savefig("Results/graficos/boxplot_fitness.png")
plt.close(fig)

print("Gráficos gerados em Results/graficos/")
print("  tempo_{n}.png          — tempo por instância (por tamanho)")
print("  fitness_{n}.png        — fitness por instância (por tamanho)")
print("  comparacao_global.png  — tempo + fitness médios com banda de desvio")
print("  boxplot_fitness.png    — distribuição do fitness por algoritmo e tamanho")