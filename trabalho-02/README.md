# **Trabalho 2 - Heurísticas e Metaheurísticas**

```txt
#define TRABALHO 2
#define PROFESSOR "Guilherme Pena"
#define ESTUDANTE "Gabriel de Paula"
```

&nbsp;

---

&nbsp;

## 🧠 **Metaheurísticas**

Duas metaheurísticas foram desenvolvidas para resolver dois problemas clássicos de otimização combinatória: o TSP (Problema do Caixeiro Viajante) e o KP (Problema da Mochila).

&nbsp;

### **1) Algoritmo Genético**

O algoritmo genético (AG) é uma técnica de otimização inspirada na evolução biológica. Ele busca soluções aproximadas para problemas complexos utilizando mecanismos similares à seleção natural, como seleção, cruzamento e mutação.

| **Parâmetros** | **População inicial** | **Gerações** | **Crossover** | **Probabilidade de Mutação** | **Taxa de Reprodução** |
|:-|:-|:-|:-|:-|:-|
| **Constantes** | POPULACAO_INICIAL | GERACOES | CROSSOVER | MUTACAO | REPRODUCAO |

🧬 🔄 🧬 🔄 🧬 🔄 🧬

&nbsp;

### **2) Colônia de Formigas**

O algoritmo de colônia de formigas (ACO) imita o comportamento das formigas na busca por alimento, usando feromônios para guiar a exploração de soluções, reforçando os melhores caminhos ao longo das iterações.

| **Parâmetros** | **Iterações** | **Formigas** | **Expoente Trilha** | **Expoente Caminho** | **Evaporação** | **Reforço** |
|:-|:-|:-|:-|:-|:-|:-|
| **Constantes** | ITERACOES | FORMIGAS | A | B | EVAPORACAO | REFORCO |

🐜&nbsp;🐜&nbsp;&nbsp;🐜&nbsp;&nbsp;🐜&nbsp;&nbsp;&nbsp;🐜&nbsp;&nbsp;&nbsp;🐜

&nbsp;

&nbsp;

&nbsp;

## ⚙️ **Compilação**

Compilar todos os executáveis em modo otimizado:

```bash
make
```

Compilar todos os executáveis em modo otimizado, imprimindo todas as soluções aceitas:

```bash
make debug
```

&nbsp;

---

&nbsp;

## 🛠️ **Execução**

Programas gerados:

| **Algoritmo** | **Problema** | **Programa** |
|:-|:-|:-|
| AG | TSP | ag-tsp |
| AG | KP | ag-kp |
| ACO | TSP | aco-tsp |
| ACO | KP | aco-kp |

Para executar um programa é necessário fornecer os valores numéricos dos parâmetros conforme indicado anteriormente.

```bash
./bin/<programa> <parametro1> <...> <parametroN>
```

Exemplo de como executar um programa 50 vezes para gerar relatórios:

```bash
for i in {1..50}; do ./bin/<programa> <parametro1> <...> <parametroN>; done
```

&nbsp;

&nbsp;

&nbsp;

&nbsp;

## 📊 **Resultados**

Lorem ipsum dolor sit amet

> Solução ótima TSP (min) = **426**
>
> Solução ótima KP (max) = **9147**

---

### **Algoritmo Genético**

1) PARAMETRO = [0]

#### **ag-tsp**

Lorem ipsum dolor sit amet

| Parâmetros | Valor médio | Tempo médio |
|-|-|-|

#### **ag-kp**

Lorem ipsum dolor sit amet

| Parâmetros | Valor médio | Tempo médio |
|-|-|-|

---

### **Colônia de Formigas**

1) PARAMETRO = [0]

#### **aco-tsp**

Lorem ipsum dolor sit amet

| Parâmetros | Valor médio | Tempo médio |
|-|-|-|

#### **aco-kp**

Lorem ipsum dolor sit amet

| Parâmetros | Valor médio | Tempo médio |
|-|-|-|
