#include <bits/stdc++.h>

using namespace std;

#ifdef PRINT
#define LOG(x) cout << x << endl;
#else
#define LOG(x)
#endif

/* Constantes do algoritmo */
double T_INICIAL = 100000;
double T_MINIMO = 0.001;
double A = 0.999;
double SA_ITERACOES = 100;

/* Aleatoriedade */
random_device rd;
mt19937 gen(rd());

/* Especifico do problema (distancia entre duas cidades)*/
double distancia(pair<int, int> c1, pair<int, int> c2) {
    return sqrt(pow(c1.first - c2.first, 2) + pow(c1.second - c2.second, 2));
}

/* Funcao objetivo (distancia total do caminho para voltar a origem) */
double avalia(vector<int> caminho, map<int, map<int, double>>& grafo) {
    double total = 0;
    for (int i = 1; i < caminho.size(); i++) {
        total += grafo[caminho[i - 1]][caminho[i]];
    }
    total += grafo[caminho[caminho.size() - 1]][caminho[0]];
    return total;
}

/* Imprimindo solucao (cidades do caminho) */
void imprime_caminho(vector<int> caminho) {
    cout << "|";
    for (int i = 0; i < caminho.size(); i++) {
        cout << caminho[i] << "|";
    }
    cout << "\n";
}

/* Solucao aleatoria (caminho embaralhado) */
vector<int> solucao_aleatoria(int n) {
    vector<int> caminho(n);

    for (int i = 0; i < n; i++) {
        caminho[i] = i + 1;
    }

    shuffle(caminho.begin(), caminho.end(), gen);

    return caminho;
}

/* Caminho vizinho ao atual (troca de posicoes) */
vector<int> solucao_vizinha(vector<int> caminho) {
    uniform_int_distribution<int> dis(0, caminho.size() - 1);

    int index1 = dis(gen), index2 = dis(gen);
    int aux = caminho[index1];
    caminho[index1] = caminho[index2];
    caminho[index2] = aux;

    return caminho;
}

/* Probabilidade de aceitar um caminho pior */
bool probilidade_t(double caminho_avaliacao, double vizinho_avaliacao, double t) {
    uniform_real_distribution<double> dist(0.0, 1.0);

    double aleatorio = dist(gen);
    double probabilidade = exp((caminho_avaliacao - vizinho_avaliacao) / t);

    return aleatorio < probabilidade;
}

/* Simulated Annealing */
pair<vector<int>, double> sa_func(map<int, map<int, double>> grafo) {
    double t = T_INICIAL;
    double a = A;

    vector<int> caminho = solucao_aleatoria(grafo.size()), melhor(caminho), vizinho;
    double caminho_avaliacao = avalia(caminho, grafo), melhor_avaliacao = caminho_avaliacao, vizinho_avaliacao;

    while (t > T_MINIMO) {
        for (int i = 0; i < SA_ITERACOES; i++) {
            vizinho = solucao_vizinha(caminho);
            vizinho_avaliacao = avalia(vizinho, grafo);
            if (vizinho_avaliacao < caminho_avaliacao) {
                caminho = vizinho;
                caminho_avaliacao = vizinho_avaliacao;
                if (vizinho_avaliacao < melhor_avaliacao) {
                    melhor = vizinho;
                    melhor_avaliacao = vizinho_avaliacao;
                }
                LOG(caminho_avaliacao);
            } else if (probilidade_t(caminho_avaliacao, vizinho_avaliacao, t)) {
                caminho = vizinho;
                caminho_avaliacao = vizinho_avaliacao;
                LOG(caminho_avaliacao);
            }
        }
        t *= a;
    }

    return {melhor, melhor_avaliacao};
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
    auto grafo = recebe_grafo();

    pair<vector<int>, double> melhor_encontrado = sa_func(grafo);

    imprime_caminho(melhor_encontrado.first);
    cout << melhor_encontrado.second << "\n";

    return 0;
}