#include <bits/stdc++.h>

using namespace std;

#ifdef PRINT
#define LOG(x) x;
#else
#define LOG(x)
#endif

/* Constantes do algoritmo */
int VNSMAX = 50;
int VMAX = 51;

/* Aleatoriedade */
random_device rd;
mt19937 gen(rd());

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

/* Busca local para melhorar uma solucao */
vector<int> busca_local(vector<int>& caminho, map<int, map<int, double>>& grafo) {
    vector<int> vizinho = caminho, melhor_vizinho;

    for (size_t i = 0; i < vizinho.size(); i++) {
        for (size_t j = i + 1; j < vizinho.size(); j++) {
            swap(vizinho[i], vizinho[j]);
            if (melhor_vizinho.size() == 0 || avalia(melhor_vizinho, grafo) > avalia(vizinho, grafo)) {
                melhor_vizinho = vizinho;
            }
            swap(vizinho[i], vizinho[j]);
        }
    }

    return melhor_vizinho;
}

/* Escolhe um vizinho aleatorio da k-esima vizinhanca */
vector<int> vizinho_aleatorio(vector<int>& caminho, int k) {
    uniform_int_distribution<int> dis(0, caminho.size() - 1);
    vector<int> perturbado = caminho;

    int index1 = k % caminho.size();
    int index2 = dis(gen);
    perturbado[index1] = caminho[index2];
    perturbado[index2] = caminho[index1];

    return perturbado;
}

/* Variable Neighborhood Search */
vector<int> vns(map<int, map<int, double>> grafo) {
    vector<int> inicial = solucao_inicial(grafo);
    vector<int> caminho = busca_local(inicial, grafo);
    LOG(imprime_caminho(caminho, grafo));
    for (int i = 0; i < VNSMAX; i++) {
        for (int k = 1; k <= VMAX; k++) {
            vector<int> vizinho = vizinho_aleatorio(caminho, k);
            vector<int> vizinho_melhorado = busca_local(vizinho, grafo);
            if (avalia(vizinho_melhorado, grafo) < avalia(caminho, grafo)) {
                LOG(imprime_caminho(vizinho_melhorado, grafo));
                caminho = vizinho_melhorado;
                i = 0;
            }
        }
    }
    return caminho;
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
             << " <VNSMAX>"
             << " <VMAX>"
             << "\n";
        exit(1);
    }

    // ./main <VNSMAX> <VMAX>
    VNSMAX = stoi(argv[1]);
    VMAX = stoi(argv[2]);
}

int main(int argc, char* argv[]) {
    recebe_parametros(argc, argv);
    auto grafo = recebe_grafo();

    vector<int> melhor_encontrado = vns(grafo);
    imprime_caminho(melhor_encontrado, grafo);

    return 0;
}