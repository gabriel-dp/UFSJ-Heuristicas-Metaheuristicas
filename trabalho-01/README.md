# **Trabalho 1 - Heurísticas e Metaheurísticas**

```cpp
#define TRABALHO 1
#define PROFESSOR "Guilherme Pena"
#define ESTUDANTE "Gabriel de Paula"
```

&nbsp;

---

&nbsp;

## 🧠 **Metaheurísticas**

Três metaheurísticas foram desenvolvidas para resolver dois problemas clássicos de otimização combinatória: o TSP (Problema do Caixeiro Viajante) e o KP (Problema da Mochila).

### **Simulated Annealing**

Um algoritmo de otimização inspirado no processo de resfriamento de metais, que busca minimizar uma função de custo explorando soluções de forma probabilística, aceitando até mesmo soluções piores temporariamente para evitar ficar preso em ótimos locais.

| **Parâmetros** | **Temperatura Inicial** | **Temperatura Mínima** |**Coeficiente de Resfriamento** | **Iterações** |
|:-|:-|:-|:-|:-|
| **Constantes** | T_INICIAL | T_MINIMO | A | SA_ITERACOES |

### **Busca Tabu**

Uma técnica de otimização iterativa que utiliza uma lista de movimentos proibidos (tabu) para evitar ciclos e explorar o espaço de soluções de forma mais eficiente, frequentemente usada em problemas de otimização combinatória.

| **Parâmetros** | **Iterações** | **Tamanho da Lista Tabu** |
|:-|:-|:-|
| **Constantes** | ITERACOES | LISTA_TABU_MAX |

### **GRASP**

Um algoritmo de busca heurística que combina duas fases: uma construção gulosa, que constrói uma solução de forma iterativa, e uma fase de aprimoramento, que tenta melhorar a solução utilizando técnicas de busca local.

| **Parâmetros** | **Iterações GRASP** | **Iterações Busca Local** | **Coeficiente de Aceitação** |
|:-|:-|:-|:-|
| **Constantes** | ITERACOES_GRASP | ITERACOES_BUSCA_LOCAL | A |

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
| Simulated Annealing | TSP | sa-tsp |
| Simulated Annealing | KP | sa-kp |
| Busca Tabu | TSP | bt-tsp |
| Busca Tabu | KP | bt-kp |
| GRASP | TSP | grasp-tsp |
| GRASP | KP | grasp-kp |

Para executar um programa é necessário fornecer os valores numéricos dos parâmetros conforme indicado anteriormente.

```bash
./bin/<programa> <parametro1> <...> <parametroN>
```

Exemplo de como executar um programa 50 vezes para gerar relatórios:

```bash
for i in {1..50}; do ./bin/<programa> <parametro1> <...> <parametroN>; done
```

&nbsp;

## 📊 **Resultados**

> Solução ótima TSP (min) = **426**
>
> Solução ótima KP (max) = **9147**

---

### **Simulated Annealing - TSP**

É possível observar uma característica comum entre os melhores resultados obtidos: Coeficiente de redução de temperatura próximo de 1 e número máximo de iterações.

| Parâmetros | Valor médio | Tempo médio |
|-|-|-|
| (100, 0.1, 0.999, 100) | **453.38** | 0.6234 s |
| (1000, 0.001, 0.999, 100) | 457.02 | 1.2246 s |
| (10000, 0.1, 0.999, 100) | 458.39 | 1.0618 s |
| (10000, 0.01, 0.999, 100) | 455.42 | 1.2613 s |
| (10000, 0.001, 0.999, 100) | 459.19 | 1.4437 s |

Contudo, é possível obter bons resultados com tempos médios de execução menores reduzindo o valor da temperatura inicial.

| Parâmetros | Valor médio | Tempo médio |
|-|-|-|
| (100, 0.1, 0.999, 50) | 469.69 | 0.3269 |
| (100, 0.1, 0.999, 100) | **453.38** | 0.6234 |
| (100, 0.01, 0.999, 50) | 468.25 | 0.4218 |
| (100, 0.01, 0.999, 100) | 463.18 | 0.8081 |
| (100, 0.001, 0.999, 50) | 469.77 | 0.5100 |
