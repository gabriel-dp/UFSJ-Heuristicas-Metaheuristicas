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
        cout << "| (" << valor_atual << ")\n";
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
double ITERACOES = 100;
double LISTA_TABU_MAX = 35;

/* Solucao inicial (itens ordenados por uma heuristica ate nao caber mais) */
Mochila solucao_inicial(peso_t capacidade, vector<Item>& itens) {
    Mochila mochila(capacidade);

    vector<Item> itens_copia = itens;

    // Ordena para facilitar a heuristica
    sort(itens_copia.begin(), itens_copia.end(), [](const Item& lhs, const Item& rhs) {
        return ((double)lhs.valor / lhs.peso) > ((double)rhs.valor / rhs.peso);  // Maior custo-beneficio primeiro (melhor)
        // return lhs.peso > rhs.peso;  // Maior peso primeiro (pessimo)
        // return lhs.peso < rhs.peso;  // Maior peso primeiro (boa)
        // return lhs.valor < rhs.valor; // Menor valor primeiro (ruim)
        // return lhs.valor > rhs.valor;  // Menor valor primeiro (ruim)
    });

    for (Item i : itens_copia) {
        mochila.adiciona(i);
        if (!mochila.valido()) {
            mochila.remove(i);
            break;
        }
    }

    return mochila;
}

/* Funcao objetivo com penalidade */
double avalia(Mochila mochila, double penalidade) {
    return mochila.valor_atual - penalidade * max(0, mochila.capacidade_atual - mochila.capacidade_max);
}

/* Funcao da busca tabu */
Mochila busca_tabu(Mochila& inicial, vector<Item>& itens) {
    Mochila atual = inicial.copia(), melhor = atual.copia();
    deque<Mochila> lista_tabu;

    /* Define penalidade como a soma dos valores */
    double penalidade = 0;
    for (Item i : itens) {
        penalidade += i.valor;
    }

    LOG(atual.imprime());
    for (int i = 0; i < ITERACOES; i++) {
        /* Seleciona melhor vizinho */
        Mochila vizinho = atual.copia(), melhor_vizinho = vizinho.copia();
        bool melhor_vizinho_definido = false;
        for (Item i : itens) {
            vizinho.contem(i) ? vizinho.remove(i) : vizinho.adiciona(i);
            bool nao_tabu = find(lista_tabu.begin(), lista_tabu.end(), vizinho) == lista_tabu.end();
            bool supera_melhor_vizinho = !melhor_vizinho_definido ? true : (avalia(melhor_vizinho, penalidade) < avalia(vizinho, penalidade));
            bool supera_melhor = avalia(melhor, penalidade) < avalia(vizinho, penalidade);
            if ((nao_tabu && supera_melhor_vizinho) || supera_melhor) {
                melhor_vizinho = vizinho.copia();
                melhor_vizinho_definido = true;
            }
            vizinho.contem(i) ? vizinho.remove(i) : vizinho.adiciona(i);
        }

        /* Adiciona vizinho na lista tabu e o torna o atual */
        if (!melhor_vizinho_definido) {
            melhor_vizinho = lista_tabu.front().copia();
            lista_tabu.pop_front();
        }
        lista_tabu.push_back(melhor_vizinho.copia());
        atual = melhor_vizinho.copia();
        LOG(atual.imprime());

        /* Avalia se o resultado melhora o melhor */
        if (avalia(melhor, penalidade) < avalia(atual, penalidade)) {
            melhor = atual.copia();
        }

        /* Permite o movimento tabu mais antigo */
        if (lista_tabu.size() > LISTA_TABU_MAX) {
            lista_tabu.pop_front();
        }
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
    if (argc != 1 + 2) {
        cout << "Define the parameters"
             << " <ITERACOES>"
             << " <LISTA_TABU_MAX>"
             << "\n";
        exit(1);
    }

    // ./bt-kp <ITERACOES> <LISTA_TABU_MAX>
    ITERACOES = stod(argv[1]);
    LISTA_TABU_MAX = stod(argv[2]);
}

int main(int argc, char* argv[]) {
    recebe_parametros(argc, argv);
    pair<double, vector<Item>> dados = recebe_capacidade_itens();

    Mochila inicial = solucao_inicial(dados.first, dados.second);
    Mochila melhor = busca_tabu(inicial, dados.second);
    melhor.imprime();

    return 0;
}