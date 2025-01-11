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

    bool remove(Item item) {
        int indice = 0;
        for (Item i : carga) {
            if (i == item) {
                valor_atual -= i.valor;
                capacidade_atual -= i.peso;
                carga.erase(carga.begin() + indice);
                return true;
            }
            indice++;
        }
        return false;
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
};

/* Constantes do algoritmo */
double ITERACOES = 100000;
double A = 0.25;

/* Aleatoriedade */
random_device rd;
mt19937 gen(rd());

/* Seleciona candidatos para serem escolhidos aleatoriamente para fazer parte do caminho */
vector<int> constroi_lista_restrita(set<int>& lista_candidatos, Mochila& mochila, vector<Item>& itens) {
    vector<int> lista_restrita;
    vector<double> avaliacao;
    double cmin = -1, cmax = -1;

    // Preenche distancia de todos os candidatos
    for (int c : lista_candidatos) {
        if (!mochila.adiciona(itens[c])) {
            avaliacao.push_back(-1);
            continue;
        }

        double valor = (double)mochila.valor_atual / mochila.capacidade_atual;

        if (cmin == -1 || valor < cmin) cmin = valor;
        if (cmax == -1 || valor > cmax) cmax = valor;
        avaliacao.push_back(valor);

        mochila.remove(itens[c]);
    }

    // Separa apenas os que fazem parte da lrc
    for (size_t i = 0; i < avaliacao.size(); i++) {
        if (avaliacao[i] >= cmax - A * (cmax - cmin)) {
            auto it = lista_candidatos.begin();
            advance(it, i);
            lista_restrita.push_back(*it);
        }
    }

    return lista_restrita;
}

/* Cria solucao usando uma abordagem aleatoria-gulosa */
Mochila solucao_aleatorio_guloso(peso_t capacidade, vector<Item>& itens) {
    Mochila mochila(capacidade);
    set<int> lista_candidatos;

    // Preenche lc inicial com todos os indices
    for (size_t i = 0; i < itens.size(); i++) {
        lista_candidatos.insert(i);
    }

    // Cria caminho guloso-aleatorio
    while (lista_candidatos.size() > 0) {
        vector<int> lrc = constroi_lista_restrita(lista_candidatos, mochila, itens);
        uniform_int_distribution<int> dist(0, lrc.size() - 1);
        int aleatorio = lrc[dist(gen)];
        mochila.adiciona(itens[aleatorio]);
        lista_candidatos.erase(aleatorio);
    }

    return mochila;
}

/* Busca local para melhorar uma solucao */
Mochila busca_local(Mochila& mochila, vector<Item>& itens) {
    Mochila vizinho = mochila.copia(), melhor_vizinho = vizinho.copia();

    for (Item i : itens) {
        if (!(vizinho.contem(i) ? vizinho.remove(i) : vizinho.adiciona(i))) continue;
        if (vizinho.valor_atual > melhor_vizinho.valor_atual) {
            melhor_vizinho = vizinho.copia();
        }
        vizinho.contem(i) ? vizinho.remove(i) : vizinho.adiciona(i);
    }

    return melhor_vizinho;
}

/* Funcao do GRASP */
Mochila grasp(peso_t capacidade, vector<Item> itens) {
    Mochila melhor(capacidade);

    for (int i = 0; i < ITERACOES; i++) {
        Mochila aleatorio_guloso = solucao_aleatorio_guloso(capacidade, itens);
        LOG(aleatorio_guloso.imprime());
        Mochila otimo_local = busca_local(aleatorio_guloso, itens);
        if (otimo_local.valor_atual > melhor.valor_atual) {
            melhor = otimo_local;
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
             << " <A>"
             << "\n";
        exit(1);
    }

    // ./grasp-tsp <ITERACOES> <A>
    ITERACOES = stod(argv[1]);
    A = stod(argv[2]);
}

int main(int argc, char* argv[]) {
    recebe_parametros(argc, argv);
    pair<peso_t, vector<Item>> dados = recebe_capacidade_itens();

    Mochila melhor_encontrado = grasp(dados.first, dados.second);
    melhor_encontrado.imprime();

    return 0;
}