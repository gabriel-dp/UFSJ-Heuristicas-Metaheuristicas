#include <bits/stdc++.h>

using namespace std;

#ifdef PRINT
#define LOG(x) x;
#else
#define LOG(x)
#endif

/* Constantes do algoritmo */
int ITERACOES = 100;
double A = 1;
double B = 1.5;
double EVAPORACAO = 0.05;
int FORMIGAS = 51;
double REFORCO = 1;

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

/* Cria solucao usando a trilha de feromonios */
vector<int> solucao_trilha_feromonio(map<int, map<int, double>>& trilha, map<int, map<int, double>>& grafo) {
    uniform_int_distribution<int> dist(1, grafo.size());
    uniform_real_distribution<double> dist_real(0, 1);
    vector<int> caminho;

    set<int> vertices;
    for (auto v : grafo) {
        vertices.insert(v.first);
    }

    int origem = dist(gen);
    caminho.push_back(origem);
    vertices.erase(origem);

    while (vertices.size() > 0) {
        vector<double> probabilidades, t_arr, n_arr;
        double tn_soma = 0, t, n, p;

        for (auto destino : grafo[origem]) {
            if (vertices.find(destino.first) != vertices.end()) {
                t = pow(trilha[origem][destino.first], A);
                n = pow(1 / destino.second, B);
                t_arr.push_back(t);
                n_arr.push_back(n);
                tn_soma += (t * n);
            }
        }

        for (size_t i = 0; i < t_arr.size(); i++) {
            p = tn_soma != 0 ? t_arr[i] * n_arr[i] / tn_soma : 0;
            probabilidades.push_back(p);
        }

        double aleatorio = dist_real(gen), acc = 0;
        for (size_t p = 0; p < probabilidades.size(); p++) {
            acc += probabilidades[p];
            if (acc == 0 || aleatorio < acc) {
                auto it = vertices.begin();
                advance(it, p);
                caminho.push_back(*it);
                origem = *it;
                vertices.erase(*it);
                break;
            }
        }
    }

    return caminho;
}

/* Funcao do ACO */
vector<int> colonia_formigas(map<int, map<int, double>>& grafo) {
    vector<int> melhor;
    map<int, map<int, double>> trilha;

    for (auto& origem : grafo) {
        for (auto& destino : origem.second) {
            trilha[origem.first][destino.first] = 1.0;
        }
    }

    for (int i = 0; i < ITERACOES; i++) {
        vector<int> melhor_formiga;
        for (int f = 0; f < FORMIGAS; f++) {
            vector<int> formiga = solucao_trilha_feromonio(trilha, grafo);
            LOG(imprime_caminho(formiga, grafo));
            if (melhor_formiga.size() == 0 || avalia(melhor_formiga, grafo) > avalia(formiga, grafo)) {
                melhor_formiga = formiga;
                if (melhor.size() == 0 || avalia(melhor, grafo) > avalia(melhor_formiga, grafo)) {
                    melhor = melhor_formiga;
                }
            }
        }
        for (auto origem : grafo) {
            for (auto destino : grafo[origem.first]) {
                trilha[origem.first][destino.first] = max(0.1, (1 - EVAPORACAO) * trilha[origem.first][destino.first]);
            }
        }
        for (size_t i = 1; i < melhor_formiga.size(); i++) {
            trilha[melhor_formiga[i - 1]][melhor_formiga[i]] += REFORCO;
        }
        trilha[melhor_formiga[melhor_formiga.size() - 1]][melhor_formiga[0]] += REFORCO;
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
    if (argc != 1 + 6) {
        cout << "Define the parameters"
             << " <ITERACOES> "
             << " <FORMIGAS> "
             << " <A> "
             << " <B>"
             << " <EVAPORACAO> "
             << " <REFORCO> "
             << "\n";
        exit(1);
    }

    ITERACOES = atoi(argv[1]);
    FORMIGAS = atoi(argv[2]);
    A = atof(argv[3]);
    B = atof(argv[4]);
    EVAPORACAO = atof(argv[5]);
    REFORCO = atof(argv[6]);
}

int main(int argc, char* argv[]) {
    recebe_parametros(argc, argv);
    auto grafo = recebe_grafo();

    vector<int> melhor_encontrado = colonia_formigas(grafo);
    imprime_caminho(melhor_encontrado, grafo);

    return 0;
}