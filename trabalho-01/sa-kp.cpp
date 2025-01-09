#include <bits/stdc++.h>

using namespace std;

#ifdef PRINT
#define LOG(x) x;
#else
#define LOG(x)
#endif

typedef int peso_t;
typedef int valor_t;

class Item {
   public:
    int indice;
    peso_t peso;
    valor_t valor;

    Item() {
    }

    Item(peso_t peso, valor_t valor, int indice) {
        this->peso = peso;
        this->valor = valor;
        this->indice = indice;
    }

    bool operator==(const Item& other) const {
        return this->indice == other.indice;
    }
};

class Mochila {
   public:
    peso_t capacidade_max, capacidade_atual = 0;
    valor_t valor_atual = 0;  // Valor usado como funcao objetivo
    vector<Item> carga;

    Mochila(peso_t capacidade_max) {
        this->capacidade_max = capacidade_max;
    }

    bool adiciona(Item item) {
        if (capacidade_atual + item.peso > capacidade_max) return false;

        carga.push_back(item);
        capacidade_atual += item.peso;
        valor_atual += item.valor;
        return true;
    }

    bool remove(int indice) {
        if (!(indice >= 0 && indice < carga.size())) return false;

        Item removido = *(carga.begin() + indice);
        valor_atual -= removido.valor;
        capacidade_atual -= removido.peso;
        carga.erase(carga.begin() + indice);
        return true;
    }

    bool troca(int indice, Item novo) {
        if (!remove(indice)) return false;
        return adiciona(novo);
    }

    bool contem(Item item) {
        for (Item i : carga) {
            if (i.indice == item.indice) return true;
        }
        return false;
    }

    Mochila copia() {
        Mochila nova(capacidade_max);
        nova.carga = carga;
        nova.capacidade_atual = capacidade_atual;
        nova.valor_atual = valor_atual;
        return nova;
    }

    void imprime() {
        for (Item i : carga) {
            cout << "|" << i.indice;
        }
        cout << "| (v=" << valor_atual << ", p=" << capacidade_atual << ")\n";
    }
};

/* Constantes do algoritmo */
double T_INICIAL = 100000;
double T_MINIMO = 0.001;
double A = 0.999;
double SA_ITERACOES = 100;

/* Aleatoriedade */
random_device rd;
mt19937 gen(rd());

/* Solucao aleatoria (itens aleatorios ate o limite) */
Mochila solucao_aleatoria(peso_t capacidade, vector<Item>& itens) {
    Mochila mochila(capacidade);

    vector<Item> itens_copia = itens;
    shuffle(itens_copia.begin(), itens_copia.end(), gen);

    for (Item i : itens_copia) {
        if (!mochila.adiciona(i)) break;
    }

    return mochila;
}

/* Solucao vizinha a atual (troca de itens) */
Mochila solucao_vizinha(Mochila& mochila, vector<Item>& itens) {
    uniform_int_distribution<int> dis2(0, itens.size() - 1);
    Mochila vizinha = mochila.copia();
    int indice2 = dis2(gen);

    if (mochila.carga.empty()) {
        vizinha.adiciona(itens[indice2]);
    } else if (!vizinha.contem(itens[indice2]) && !vizinha.adiciona(itens[indice2])) {
        uniform_int_distribution<int> dis1(0, mochila.carga.size() - 1);
        int indice1 = dis1(gen);
        vizinha.troca(indice1, itens[indice2]);
    }

    return vizinha;
}

/* Probabilidade de aceitar uma mochila pior */
bool probabilidade_t(double mochila_avaliacao, double vizinho_avaliacao, double t) {
    uniform_real_distribution<double> dist(0.0, 1.0);

    double aleatorio = dist(gen);
    double probabilidade = exp((vizinho_avaliacao - mochila_avaliacao) / t);

    return aleatorio < probabilidade;
}

/* Simulated Annealing */
Mochila sa_func(peso_t capacidade, vector<Item> itens) {
    double t = T_INICIAL;
    double a = A;

    Mochila mochila = solucao_aleatoria(capacidade, itens), melhor = mochila.copia();

    while (t > T_MINIMO) {
        for (int i = 0; i < SA_ITERACOES; i++) {
            Mochila vizinho = solucao_vizinha(mochila, itens);
            if (vizinho.valor_atual > mochila.valor_atual) {
                mochila = vizinho.copia();
                if (vizinho.valor_atual > melhor.valor_atual) {
                    melhor = vizinho.copia();
                }
                LOG(mochila.imprime());
            } else if (probabilidade_t(mochila.valor_atual, vizinho.valor_atual, t)) {
                mochila = vizinho.copia();
                LOG(mochila.imprime());
            }
        }
        t *= a;
    }

    return melhor;
}

/* Leitura dos itens e capacidade */
pair<peso_t, vector<Item>> recebe_capacidade_itens() {
    int n, capacidade, peso, valor;

    cin >> n >> capacidade;
    vector<Item> itens(n);

    // Itens da mochila
    for (int i = 0; i < n; i++) {
        cin >> peso >> valor;
        itens[i] = Item(peso, valor, i + 1);
    }

    // Ignora ultima linha
    string buffer;
    getline(cin, buffer);
    getline(cin, buffer);

    return {capacidade, itens};
}

/* Parametros passados por linha de comando */
void recebe_parametros(int argc, char* argv[]) {
    if (argc != 1 + 4) {
        cout << "Define the parameters"
             << " <T_INICIAL>"
             << " <T_MINIMO>"
             << " <A>"
             << " <SA_ITERACOES>"
             << "\n";
        exit(1);
    }

    // ./sa-tsp <T_INICIAL> <T_MINIMO> <A> <SA_ITERACOES>
    T_INICIAL = stod(argv[1]);
    T_MINIMO = stod(argv[2]);
    A = stod(argv[3]);
    SA_ITERACOES = stod(argv[4]);
}

int main(int argc, char* argv[]) {
    recebe_parametros(argc, argv);
    pair<double, vector<Item>> dados = recebe_capacidade_itens();

    Mochila melhor_encontrado = sa_func(dados.first, dados.second);

    cout << melhor_encontrado.valor_atual << "\n";

    return 0;
}