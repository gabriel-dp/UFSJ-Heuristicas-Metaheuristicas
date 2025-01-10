#include <bits/stdc++.h>

using namespace std;

#ifdef PRINT
#define LOG(x) x;
#else
#define LOG(x)
#endif

/* Constantes do algoritmo */
double ITERACOES = 100;
double LISTA_TABU_MAX = 35;

/* Especifico do problema (distancia entre duas cidades)*/
double distancia(pair<int, int> c1, pair<int, int> c2) {
    return sqrt(pow(c1.first - c2.first, 2) + pow(c1.second - c2.second, 2));
}

/* Funcao objetivo (distancia total do caminho para voltar a origem) */
double avalia(vector<int>& caminho, map<int, map<int, double>>& grafo) {
    double total = 0;
    for (size_t i = 1; i < caminho.size(); i++) {
        total += grafo[caminho[i - 1]][caminho[i]];
    }
    total += grafo[caminho[caminho.size() - 1]][caminho[0]];
    return total;
}

/* Imprimindo solucao (cidades do caminho) */
void imprime_caminho(vector<int>& caminho, map<int, map<int, double>>& grafo) {
    cout << "|";
    for (size_t i = 0; i < caminho.size(); i++) {
        cout << caminho[i] << "|";
    }
    cout << " (" << avalia(caminho, grafo) << ")\n";
}

/* Solucao inicial (itens ordenados por uma heuristica ate nao caber mais) */
vector<int> solucao_inicial(map<int, map<int, double>>& grafo) {
    vector<int> caminho(grafo.size());
    unordered_set<int> caminho_vertices;

    // Formula solucao inicial
    caminho[0] = 1;
    caminho_vertices.insert(1);
    while (caminho_vertices.size() != grafo.size()) {
        int melhor_vertice = -1, melhor_distancia = -1;
        for (size_t i = 2; i <= grafo.size(); i++) {
            if (caminho_vertices.find(i) == caminho_vertices.end()) {
                if (melhor_vertice == -1 || melhor_distancia > grafo[caminho[caminho_vertices.size() - 1]][i]) {
                    melhor_vertice = i;
                    melhor_distancia = grafo[caminho[caminho_vertices.size() - 1]][i];
                }
            }
        }
        caminho[caminho_vertices.size()] = melhor_vertice;
        caminho_vertices.insert(melhor_vertice);
    }

    return caminho;
}

/* Funcao da busca tabu */
vector<int> busca_tabu(vector<int>& inicial, map<int, map<int, double>>& grafo) {
    vector<int> atual = inicial, melhor = atual;
    deque<vector<int>> lista_tabu;

    LOG(imprime_caminho(atual, grafo));
    for (int i = 0; i < ITERACOES; i++) {
        /* Seleciona melhor vizinho */
        vector<int> vizinho = atual, melhor_vizinho;
        for (size_t i = 0; i < vizinho.size(); i++) {
            for (size_t j = i + 1; j < vizinho.size(); j++) {
                swap(vizinho[i], vizinho[j]);
                bool nao_tabu = find(lista_tabu.begin(), lista_tabu.end(), vizinho) == lista_tabu.end();
                bool supera_melhor_vizinho = melhor_vizinho.size() == 0 ? true : (avalia(melhor_vizinho, grafo) > avalia(vizinho, grafo));
                bool supera_melhor = avalia(melhor, grafo) > avalia(vizinho, grafo);
                if ((nao_tabu && supera_melhor_vizinho) || supera_melhor) {
                    melhor_vizinho = vizinho;
                }
                swap(vizinho[i], vizinho[j]);
            }
        }

        /* Adiciona vizinho na lista tabu e o torna o atual */
        if (melhor_vizinho.size() == 0) {
            melhor_vizinho = lista_tabu.front();
            lista_tabu.pop_front();
        }
        lista_tabu.push_back(melhor_vizinho);
        atual = melhor_vizinho;
        LOG(imprime_caminho(atual, grafo));

        /* Avalia se o resultado melhora o melhor */
        if (avalia(melhor, grafo) > avalia(atual, grafo)) {
            melhor = atual;
        }

        /* Permite o movimento tabu mais antigo */
        if (lista_tabu.size() > LISTA_TABU_MAX) {
            lista_tabu.pop_front();
        }
    }

    return melhor;
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
    auto grafo = recebe_grafo();

    vector<int> inicial = solucao_inicial(grafo);
    vector<int> melhor_encontrado = busca_tabu(inicial, grafo);
    imprime_caminho(melhor_encontrado, grafo);

    return 0;
}