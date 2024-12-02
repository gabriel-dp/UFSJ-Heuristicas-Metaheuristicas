#include <bits/stdc++.h>

#define ITERACOES 200
#define LISTA_TABU_MAX 3

using namespace std;

typedef int peso_t;
typedef int valor_t;

class Item {
   public:
    peso_t peso;
    valor_t valor;
    int original;

    Item(peso_t peso, valor_t valor, int original) {
        this->peso = peso;
        this->valor = valor;
        this->original = original;
    }

    bool operator==(const Item& other) const {
        return this->original == other.original && this->peso == other.peso && this->valor == other.valor;
    }
};

class Mochila {
   public:
    vector<pair<Item, bool>> carga;
    peso_t capacidade = -1;

    Mochila() {
    }

    Mochila(peso_t capacidade, const vector<Item>& itens) {
        this->capacidade = capacidade;
        for (const Item& i : itens) {
            this->carga.push_back({i, false});
        }
    }

    Mochila(peso_t capacidade, vector<pair<Item, bool>> carga) {
        this->capacidade = capacidade;
        this->carga = carga;
    }

    static Mochila copia(Mochila original) {
        return Mochila(original.capacidade, original.carga);
    }

    bool valido() const {
        return this->capacidade != -1;
    }

    void imprime() {
        for (auto& i : this->carga) {
            cout << "|" << i.second << "(" << i.first.original << ")";
        }
        cout << "|\n";
    }

    bool operator==(const Mochila& other) const {
        if (this->capacidade != other.capacidade) {
            return false;
        }
        for (int i = 0; i < carga.size(); i++) {
            if (this->carga[i].second != other.carga[i].second || !(this->carga[i].first == other.carga[i].first)) {
                return false;
            }
        }
        return true;
    }
};

class Avaliacao {
   public:
    peso_t peso = 0;
    valor_t valor = 0;
    bool valido = true;

    Avaliacao(const Mochila& mochila) {
        for (auto& item : mochila.carga) {
            if (item.second) {
                this->peso += item.first.peso;
                this->valor += item.first.valor;
            }
        }
        if (!mochila.valido() || this->peso > mochila.capacidade) {
            this->valido = false;
        }
    }

    void imprime() {
        cout << "Peso: " << this->peso << " | Valor: " << this->valor << " >>> ";
    }

    bool operator<(const Avaliacao& other) const {
        if (other.valido)
            if (this->valor != other.valor)
                return this->valor < other.valor;
        return this->peso > other.peso;
    }
};

Mochila busca_tabu(Mochila inicial) {
    Mochila atual(inicial), melhor;
    deque<Mochila> lista_tabu;

    for (int i = 0; i < ITERACOES; i++) {
        /* Seleciona melhor vizinho */
        Mochila vizinho(atual), melhor_vizinho;
        for (int i = 0; i < vizinho.carga.size(); i++) {
            vizinho.carga[i].second = !vizinho.carga[i].second;
            Avaliacao atual = Avaliacao(vizinho);
            if ((!melhor_vizinho.valido() || Avaliacao(melhor_vizinho) < atual) && (find(lista_tabu.begin(), lista_tabu.end(), vizinho) == lista_tabu.end() || Avaliacao(melhor) < Avaliacao(melhor_vizinho))) {
                melhor_vizinho = Mochila::copia(vizinho);
            }
            vizinho.carga[i].second = !vizinho.carga[i].second;
        }

        // cout << "\nvizinho\n";
        // Avaliacao(melhor_vizinho).imprime();
        // melhor_vizinho.imprime();

        /* Adiciona vizinho na lista tabu e o torna o atual */
        lista_tabu.push_back(melhor_vizinho);
        atual = Mochila::copia(melhor_vizinho);

        /* Avalia se o resultado melhora o melhor */
        if (Avaliacao(melhor) < Avaliacao(atual)) {
            melhor = Mochila::copia(atual);
        }

        /* Permite o movimento tabu mais antigo */
        if (lista_tabu.size() > LISTA_TABU_MAX) {
            lista_tabu.pop_front();
        }
    }

    return melhor;
}

int main() {
    int n, capacidade, peso, valor;

    cin >> n >> capacidade;

    // Cria todos os itens
    vector<Item> itens;
    for (int i = 0; i < n; i++) {
        cin >> peso >> valor;
        itens.push_back(Item(peso, valor, i));
    }

    // Ordena para facilitar a heuristica
    sort(itens.begin(), itens.end(), [](const Item& lhs, const Item& rhs) {
        return ((double)lhs.valor / lhs.peso) > ((double)rhs.valor / rhs.peso);  // Maior custo-beneficio primeiro
    });

    // Cria a mochila e coloca os itens na mochila na ordem ate quando nao couber mais
    Mochila mochila = Mochila(capacidade, itens);
    for (int i = 0; i < n; i++) {
        mochila.carga[i].second = true;
        if (!Avaliacao(mochila).valido) {
            mochila.carga[i].second = false;
        }
    }

    cout << "Inicial (custo-beneficio):\n";
    Avaliacao(mochila).imprime();
    mochila.imprime();

    cout << "\nBusca Tabu:\n";
    Mochila melhor = busca_tabu(mochila);
    Avaliacao(melhor).imprime();
    melhor.imprime();

    return 0;
}