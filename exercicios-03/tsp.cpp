#include <bits/stdc++.h>

using namespace std;

#define ITERACOES 100

double distancia(pair<int, int> c1, pair<int, int> c2) {
    return sqrt(pow(c1.first - c2.first, 2) + pow(c1.second - c2.second, 2));
}

double avalia(vector<int> caminho, map<int, unordered_map<int, double>> grafo) {  // distancia total
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

    // Formula solucao inicial
    caminho[0] = 1;
    caminho_vertices.insert(1);
    while (caminho_vertices.size() != n) {
        int melhor_vertice = -1, melhor_distancia = -1;
        for (int i = 2; i <= n; i++) {
            if (caminho_vertices.find(i) == caminho_vertices.end()) {
                // A condicao para a melhor_distancia define a heuristica
                if (melhor_vertice == -1 || melhor_distancia > grafo[caminho[caminho_vertices.size() - 1]][i]) {
                    melhor_vertice = i;
                    melhor_distancia = grafo[caminho[caminho_vertices.size() - 1]][i];
                }
            }
        }
        caminho[caminho_vertices.size()] = melhor_vertice;
        caminho_vertices.insert(melhor_vertice);
    }

    cout << "Distancia Inicial: " << avalia(caminho, grafo) << " >>>";
    imprime_caminho(caminho);
    cout << "\n\n";

    // Busca local
    int contador = 0;
    vector<int> melhor_caminho(n);
    double melhor_avaliacao = -1;
    do {
        contador++;
        double avaliacao = avalia(caminho, grafo);

        if (melhor_avaliacao == -1 || melhor_avaliacao > avaliacao) {
            melhor_avaliacao = avaliacao;
            melhor_caminho = caminho;
        }

        cout << "Distancia: " << avaliacao << " >>>";
        imprime_caminho(caminho);
    } while (contador < ITERACOES && next_permutation(caminho.begin(), caminho.end()));

    cout << "\n\nMelhor Distancia: " << melhor_avaliacao << " >>>";
    imprime_caminho(melhor_caminho);

    return 0;
}