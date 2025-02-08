#include <bits/stdc++.h>

using namespace std;

#ifdef PRINT
#define LOG(x) x;
#else
#define LOG(x)
#endif

typedef int peso_t;
typedef double valor_t;

class Item {
   public:
    int indice;
    peso_t peso;
    valor_t valor;

    Item() {}

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

    bool valido() {
        return capacidade_atual <= capacidade_max;
    }

    void adiciona(Item item) {
        carga.push_back(item);
        capacidade_atual += item.peso;
        valor_atual += item.valor;
    }

    void remove(Item item) {
        int indice = 0;
        for (Item i : carga) {
            if (i == item) {
                valor_atual -= i.valor;
                capacidade_atual -= i.peso;
                carga.erase(carga.begin() + indice);
                return;
            }
            indice++;
        }
    }

    void troca(Item antigo, Item novo) {
        remove(antigo);
        adiciona(novo);
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
        cout << "| (" << valor_atual << "/" << capacidade_atual << ")\n";
    }

    bool operator==(const Mochila& other) const {
        if (this->capacidade_atual != other.capacidade_atual || this->valor_atual != other.valor_atual) {
            return false;
        }
        for (size_t i = 0; i < carga.size(); i++) {
            if (this->carga[i].indice != other.carga[i].indice) {
                return false;
            }
        }
        return true;
    }
};

/* Constantes do algoritmo */
int POPULACAO_INICIAL = 200;
int GERACOES = 100;
double MUTACAO = 0.1;
double REPRODUCAO = 1;

/* Aleatoriedade */
random_device rd;
mt19937 gen(rd());

Mochila solucao_aleatoria(peso_t capacidade, vector<Item>& itens) {
    Mochila mochila(capacidade);

    vector<Item> itens_copia = itens;
    shuffle(itens_copia.begin(), itens_copia.end(), gen);

    for (Item i : itens_copia) {
        mochila.adiciona(i);
        if (!mochila.valido()) {
            mochila.remove(i);
            break;
        }
    }

    return mochila;
}

vector<Mochila> selecao(vector<Mochila>& mochilas) {
    vector<Mochila> selecionados;
    uniform_real_distribution<double> dist(0.0, 1.0);

    set<int> indexes;

    for (size_t i = 0; i < mochilas.size(); i++) {
        indexes.insert(i);
    }

    while (selecionados.size() < mochilas.size() * REPRODUCAO) {
        valor_t soma = 0;
        vector<double> probabilities;

        for (int i : indexes) {
            soma += mochilas[i].valor_atual;
        }
        double acumulado = 0;
        for (int i : indexes) {
            double probabilidade = mochilas[i].valor_atual / soma;
            probabilities.push_back(acumulado + probabilidade);
            acumulado += probabilidade;
        }

        double aleatorio = dist(gen);
        for (size_t i = 0; i < probabilities.size(); i++) {
            if (aleatorio < probabilities[i]) {
                auto it = indexes.begin();
                std::advance(it, i);
                selecionados.push_back(mochilas[*it]);
                indexes.erase(it);
                break;
            }
        }
    }

    return selecionados;
}

vector<Mochila> reproducao(vector<Mochila>& pais, vector<Item>& itens) {
    vector<Mochila> filhos;
    uniform_int_distribution<> dist_c(0, itens.size() - 1);
    uniform_real_distribution<double> dist_m(0.0, 1.0);

    // Casal
    for (size_t i = 0; i < pais.size() - 1; i += 2) {
        int divisao = dist_c(gen);
        Mochila filho1 = pais[i].copia(), filho2 = pais[i + 1].copia();

        // Crossover
        for (size_t j = divisao; j < itens.size(); j++) {
            Item item = itens[j];
            bool filho1_tem = filho1.contem(item), filho2_tem = filho2.contem(item);
            if (filho1_tem && !filho2_tem) {
                filho1.remove(item);
                filho2.adiciona(item);
            } else if (!filho1_tem && filho2_tem) {
                filho1.adiciona(item);
                filho2.remove(item);
            }
        }

        // Mutacao
        double probabilidade_mutacao_filho1 = dist_m(gen);
        if (probabilidade_mutacao_filho1 < MUTACAO) {
            int index_mutacao = dist_c(gen);
            Item mutacao = itens[index_mutacao];
            filho1.contem(mutacao) ? filho1.remove(mutacao) : filho1.adiciona(mutacao);
        }
        double probabilidade_mutacao_filho2 = dist_m(gen);
        if (probabilidade_mutacao_filho2 < MUTACAO) {
            int index_mutacao = dist_c(gen);
            Item mutacao = itens[index_mutacao];
            filho2.contem(mutacao) ? filho2.remove(mutacao) : filho2.adiciona(mutacao);
        }

        filhos.push_back(filho1);
        filhos.push_back(filho2);
    }

    return filhos;
}

vector<Mochila> sobrevivencia(vector<Mochila>& pais, vector<Mochila>& filhos) {
    vector<Mochila> geracao(pais);
    geracao.insert(geracao.end(), filhos.begin(), filhos.end());

    sort(geracao.begin(), geracao.end(), [](Mochila& a, Mochila& b) {
        if ((a.valido() && b.valido()) || (!a.valido() && !b.valido())) {
            return a.valor_atual > b.valor_atual;
        }
        return a.valido();
    });

    geracao.erase(geracao.begin() + (geracao.size() / 2), geracao.end());

    int index_limite = 0;
    for (Mochila m : geracao) {
        if (!m.valido()) {
            break;
        }
        index_limite++;
    }
    geracao.erase(geracao.begin() + index_limite, geracao.end());

    return geracao;
}

Mochila genetico(peso_t capacidade, vector<Item>& itens) {
    vector<Mochila> populacao;
    for (int i = 0; i < POPULACAO_INICIAL; i++) {
        populacao.push_back(solucao_aleatoria(capacidade, itens));
    }

    for (int i = 0; i < GERACOES; i++) {
        vector<Mochila> selecionados = selecao(populacao);
        vector<Mochila> filhos = reproducao(selecionados, itens);
        vector<Mochila> sobreviventes = sobrevivencia(populacao, filhos);
        populacao = sobreviventes;
        LOG(
            for (Mochila m : populacao) {
                m.imprime();
            })
    }

    return populacao[0];
}

/* Leitura dos itens e capacidade */
pair<peso_t, vector<Item>> recebe_capacidade_itens() {
    int n, capacidade, peso, valor;

    cin >> n >> capacidade;
    vector<Item> itens(n);

    // Itens da mochila
    for (int i = 0; i < n; i++) {
        cin >> valor >> peso;
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
             << " <POPULACAO_INICIAL>"
             << " <GERACOES>"
             << " <MUTACAO>"
             << " <REPRODUCAO>"
             << "\n";
        exit(1);
    }

    // ./ag-kp <POPULACAO_INICIAL> <GERACOES> <MUTACAO> <REPRODUCAO>
    POPULACAO_INICIAL = atoi(argv[1]);
    GERACOES = atoi(argv[2]);
    MUTACAO = atof(argv[2]);
    REPRODUCAO = atof(argv[4]);
}

int main(int argc, char* argv[]) {
    recebe_parametros(argc, argv);
    pair<peso_t, vector<Item>> dados = recebe_capacidade_itens();

    Mochila melhor = genetico(dados.first, dados.second);
    melhor.imprime();

    return 0;
}