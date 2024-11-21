#include <bits/stdc++.h>

using namespace std;

#define T_INICIAL 100000
#define T_MINIMO 0.001
#define A 0.999
#define SA_ITERACOES 100

random_device rd;
mt19937 gen(rd());

double distancia(pair<int, int> c1, pair<int, int> c2) {
    return sqrt(pow(c1.first - c2.first, 2) + pow(c1.second - c2.second, 2));
}

double avalia(vector<int> caminho, map<int, unordered_map<int, double>>& grafo) {  // distancia total
    double total = 0;
    for (int i = 1; i < caminho.size(); i++) {
        total += grafo[caminho[i - 1]][caminho[i]];
    }
    total += grafo[caminho[caminho.size() - 1]][caminho[0]];
    return total;
}

void imprime_caminho(vector<int> caminho) {
    cout << "|";
    for (int i = 0; i < caminho.size(); i++) {
        cout << caminho[i] << "|";
    }
    cout << "\n";
}

vector<int> caminho_aleatorio(int n) {
    vector<int> caminho(n);

    for (int i = 0; i < n; i++) {
        caminho[i] = i + 1;
    }

    // shuffle(caminho.begin(), caminho.end(), gen);

    return caminho;
}

vector<int> caminho_vizinho(vector<int> caminho) {
    uniform_int_distribution<> dis(0, caminho.size() - 1);

    int index1 = dis(gen), index2 = dis(gen);
    int aux = caminho[index1];
    caminho[index1] = caminho[index2];
    caminho[index2] = aux;

    return caminho;
}

bool probilidade_t(double caminho_avaliacao, double vizinho_avaliacao, double t) {
    uniform_real_distribution<double> dist(0.0, 1.0);

    double aleatorio = dist(gen);
    double probabilidade = exp((caminho_avaliacao - vizinho_avaliacao) / t);
    // cout << aleatorio << probabilidade;

    return aleatorio < probabilidade;
}

pair<vector<int>, int> sa_func(map<int, unordered_map<int, double>> grafo) {
    double t = T_INICIAL;
    double a = A;

    vector<int> caminho = caminho_aleatorio(grafo.size()), melhor(caminho), vizinho;
    double caminho_avaliacao = avalia(caminho, grafo), melhor_avaliacao = caminho_avaliacao, vizinho_avaliacao;

    while (t > T_MINIMO) {
        for (int i = 0; i < SA_ITERACOES; i++) {
            vizinho = caminho_vizinho(caminho);
            vizinho_avaliacao = avalia(vizinho, grafo);
            if (vizinho_avaliacao < caminho_avaliacao) {
                caminho = vizinho;
                caminho_avaliacao = vizinho_avaliacao;
                if (vizinho_avaliacao < melhor_avaliacao) {
                    melhor = vizinho;
                    melhor_avaliacao = vizinho_avaliacao;
                }
            } else if (probilidade_t(caminho_avaliacao, vizinho_avaliacao, t)) {
                caminho = vizinho;
                caminho_avaliacao = vizinho_avaliacao;
            }
        }
        t *= a;
    }

    return {melhor, melhor_avaliacao};
}

int main() {
    map<int, unordered_map<int, double>> grafo;
    map<int, pair<int, int>> cidades;
    int n, x, y;

    string buffer;
    getline(cin, buffer);
    getline(cin, buffer);
    getline(cin, buffer);

    cin >> buffer >> buffer >> n;
    vector<int> caminho(n);
    unordered_set<int> caminho_vertices(n);

    getline(cin, buffer);
    getline(cin, buffer);
    getline(cin, buffer);

    // Cidades e suas posicoes
    for (int i = 1; i <= n; i++) {
        cin >> x >> x >> y;
        cidades[i] = {x, y};
    }

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

    pair<vector<int>, int> melhor_encontrado = sa_func(grafo);
    imprime_caminho(melhor_encontrado.first);
    cout << melhor_encontrado.second << "\n";

    return 0;
}