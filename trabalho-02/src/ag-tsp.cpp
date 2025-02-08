#include <bits/stdc++.h>

using namespace std;

#ifdef PRINT
#define LOG(x) x;
#else
#define LOG(x)
#endif

/* Constantes do algoritmo */
int POPULACAO_INICIAL = 200;
int GERACOES = 100;
double MUTACAO = 0.1;
double REPRODUCAO = 1;

/* Aleatoriedade */
random_device rd;
mt19937 gen(rd());

class Caminho {
   private:
    map<int, map<int, double>> grafo;

   public:
    vector<int> cidades;
    unordered_set<int> cidades_adicionadas;
    double distancia_total = 0, distancia_origem_fim = 0;

    Caminho(map<int, map<int, double>>& grafo) {
        this->grafo = grafo;
    }

    int inicio() {
        return *cidades.begin();
    }

    int fim() {
        return *cidades.rbegin();
    }

    bool contem(int c) {
        return cidades_adicionadas.find(c) != cidades_adicionadas.end();
    }

    bool adiciona(int c) {
        if (contem(c)) {
            return false;
        }
        cidades_adicionadas.insert(c);

        if (cidades.size() > 0) {
            distancia_origem_fim += grafo[fim()][c];
            cidades.push_back(c);
            distancia_total = distancia_origem_fim + grafo[fim()][inicio()];
        } else {
            cidades.push_back(c);
        }

        return true;
    }

    bool troca(size_t i1, size_t i2) {
        if (i1 >= cidades.size() || i2 >= cidades.size()) {
            return false;
        }
        swap(cidades[i1], cidades[i2]);

        distancia_origem_fim = 0;
        for (size_t i = 1; i < cidades.size(); i++) {
            distancia_origem_fim += grafo[cidades[i - 1]][cidades[i]];
        }
        distancia_total = distancia_origem_fim + grafo[fim()][inicio()];

        return true;
    }

    Caminho copia() {
        Caminho novo(grafo);
        novo.cidades = cidades;
        novo.cidades_adicionadas = cidades_adicionadas;
        novo.distancia_origem_fim = distancia_origem_fim;
        novo.distancia_total = distancia_total;
        return novo;
    }

    static Caminho gera_filho(Caminho p1, Caminho p2, map<int, map<int, double>>& grafo) {
        size_t n = p1.cidades.size();

        Caminho filho(grafo);
        filho.cidades = vector<int>(n);

        uniform_int_distribution<> dist_c(0, n - 1);
        int divisao1 = dist_c(gen), divisao2 = dist_c(gen), i1 = min(divisao1, divisao2), i2 = max(divisao1, divisao2);

        copy(p1.cidades.begin() + i1, p1.cidades.begin() + i2 + 1, filho.cidades.begin() + i1);
        for (int i = i1; i <= i2; i++) {
            filho.cidades_adicionadas.insert(p1.cidades[i]);
        }

        int i = (i2 + 1) % n, j = (i2 + 1) % n;
        while (filho.cidades_adicionadas.size() != n) {
            if (filho.contem(p2.cidades[i])) {
                i = (i + 1) % n;
                continue;
            }

            filho.cidades[j] = p2.cidades[i];
            filho.cidades_adicionadas.insert(p2.cidades[i]);
            j = (j + 1) % n;
            if (j == i1) {
                j = (i2 + 1) % n;
            }
            i = (i + 1) % n;
        }

        filho.distancia_origem_fim = 0;
        for (size_t i = 1; i < filho.cidades.size(); i++) {
            filho.distancia_origem_fim += grafo[filho.cidades[i - 1]][filho.cidades[i]];
        }
        filho.distancia_total = filho.distancia_origem_fim + grafo[filho.fim()][filho.inicio()];

        return filho;
    }

    void imprime() {
        cout << "|";
        for (size_t i = 0; i < cidades.size(); i++) {
            cout << cidades[i] << "|";
        }
        cout << " (" << distancia_total << ")\n";
    }
};

/* Especifico do problema (distancia entre duas cidades)*/
double distancia(pair<int, int> c1, pair<int, int> c2) {
    return sqrt(pow(c1.first - c2.first, 2) + pow(c1.second - c2.second, 2));
}

Caminho solucao_aleatoria(map<int, map<int, double>>& grafo) {
    Caminho caminho(grafo);

    vector<int> cidades(grafo.size());
    int i = 0;
    for (auto origem : grafo) {
        cidades[i] = origem.first;
        i++;
    }
    shuffle(cidades.begin(), cidades.end(), gen);

    for (int cidade : cidades) {
        caminho.adiciona(cidade);
    }

    return caminho;
}

vector<Caminho> selecao(vector<Caminho>& caminhos) {
    vector<Caminho> selecionados;
    uniform_real_distribution<double> dist(0.0, 1.0);

    set<int> indexes;

    for (size_t i = 0; i < caminhos.size(); i++) {
        indexes.insert(i);
    }

    while (selecionados.size() < caminhos.size() * REPRODUCAO) {
        double soma = 0;
        vector<double> probabilities;

        for (int i : indexes) {
            soma += caminhos[i].distancia_total;
        }
        double acumulado = 0;
        for (int i : indexes) {
            double probabilidade = caminhos[i].distancia_total / soma;
            probabilities.push_back(acumulado + probabilidade);
            acumulado += probabilidade;
        }

        double aleatorio = dist(gen);
        for (size_t i = 0; i < probabilities.size(); i++) {
            if (aleatorio < probabilities[i]) {
                auto it = indexes.begin();
                std::advance(it, i);
                selecionados.push_back(caminhos[*it].copia());
                indexes.erase(it);
                break;
            }
        }
    }

    return selecionados;
}

vector<Caminho> reproducao(vector<Caminho>& pais, map<int, map<int, double>>& grafo) {
    vector<Caminho> filhos;
    uniform_int_distribution<> dist_c(0, pais[0].cidades.size() - 1);
    uniform_real_distribution<double> dist_m(0.0, 1.0);

    // Casal
    for (size_t i = 0; i < pais.size() - 1; i += 2) {
        // Crossover
        Caminho filho1 = Caminho::gera_filho(pais[i], pais[i + 1], grafo);
        Caminho filho2 = Caminho::gera_filho(pais[i + 1], pais[i], grafo);

        // Mutacao
        double probabilidade_mutacao_filho1 = dist_m(gen);
        if (probabilidade_mutacao_filho1 < MUTACAO) {
            int index_mutacao1 = dist_c(gen), index_mutacao2 = dist_c(gen);
            filho1.troca(index_mutacao1, index_mutacao2);
        }
        double probabilidade_mutacao_filho2 = dist_m(gen);
        if (probabilidade_mutacao_filho2 < MUTACAO) {
            int index_mutacao1 = dist_c(gen), index_mutacao2 = dist_c(gen);
            filho2.troca(index_mutacao1, index_mutacao2);
        }

        filhos.push_back(filho1);
        filhos.push_back(filho2);
    }

    return filhos;
}

vector<Caminho> sobrevivencia(vector<Caminho>& pais, vector<Caminho>& filhos) {
    vector<Caminho> geracao(pais);
    geracao.insert(geracao.end(), filhos.begin(), filhos.end());

    sort(geracao.begin(), geracao.end(), [](Caminho& a, Caminho& b) {
        return a.distancia_total < b.distancia_total;
    });

    geracao.erase(geracao.begin() + (geracao.size() / 2), geracao.end());

    return geracao;
}

Caminho genetico(map<int, map<int, double>>& grafo) {
    vector<Caminho> populacao;
    for (int i = 0; i < POPULACAO_INICIAL; i++) {
        populacao.push_back(solucao_aleatoria(grafo));
    }

    for (int i = 0; i < GERACOES; i++) {
        vector<Caminho> selecionados = selecao(populacao);
        vector<Caminho> filhos = reproducao(selecionados, grafo);
        vector<Caminho> sobreviventes = sobrevivencia(populacao, filhos);
        populacao = sobreviventes;
        LOG(
            for (Caminho c : populacao) {
                c.imprime();
            })
    }

    return populacao[0];
}

/* Leitura do grafo */
map<int, map<int, double>> recebe_grafo() {
    int n, x, y;
    map<int, pair<int, int>> cidades;
    map<int, map<int, double>> grafo;

    // Cabecalho
    string buffer;
    getline(cin, buffer);
    getline(cin, buffer);
    getline(cin, buffer);
    cin >> buffer >> buffer >> n;
    getline(cin, buffer);
    getline(cin, buffer);
    getline(cin, buffer);

    // Cidades e suas posicoes
    for (int i = 1; i <= n; i++) {
        cin >> x >> x >> y;
        cidades[i] = {x, y};
    }

    // EOF
    getline(cin, buffer);

    // Cria grafo completo bidirecional
    for (int i = 1; i <= n; i++) {
        grafo[i];
        for (int j = i + 1; j <= n; j++) {
            double dist = distancia(cidades[i], cidades[j]);
            grafo[i][j] = dist;
            grafo[j][i] = dist;
        }
    }

    return grafo;
}

/* Parametros passados por linha de comando */
void recebe_parametros(int argc, char* argv[]) {
    if (argc != 1 + 4) {
        cout << "Define the parameters"
             << "<POPULACAO_INICIAL>"
             << "<GERACOES>"
             << "<MUTACAO>"
             << "<REPRODUCAO>"
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
    auto grafo = recebe_grafo();

    Caminho melhor = genetico(grafo);
    melhor.imprime();

    return 0;
}