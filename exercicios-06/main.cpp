#include <bits/stdc++.h>

#define ITERACOES_GRASP 100000
#define ITERACOES_BUSCA_LOCAL 1000
#define A 0.7

using namespace std;

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

vector<int> constroi_lista_restrita(set<int>& lista_candidatos, vector<int>& caminho, map<int, unordered_map<int, double>>& grafo) {
    vector<int> lista_restrita;
    vector<double> avaliacao;
    double cmin = -1, cmax = -1;

    // Preenche distancia de todos os candidatos
    for (auto c : lista_candidatos) {
        double dist = grafo[caminho[caminho.size() - 1]][c];
        if (cmin == -1 || dist < cmin) cmin = dist;
        if (cmax == -1 || dist > cmax) cmax = dist;
        avaliacao.push_back(dist);
    }

    // Separa apenas os que fazem parte da lrc
    for (int i = 0; i < avaliacao.size(); i++) {
        if (avaliacao[i] <= cmin + A * (cmax - cmin)) {
            auto it = lista_candidatos.begin();
            advance(it, i);
            lista_restrita.push_back(*it);
        }
    }

    return lista_restrita;
}

vector<int> caminho_aleatorio_guloso(int n, map<int, unordered_map<int, double>>& grafo) {
    vector<int> caminho = {1};
    set<int> lista_candidatos;

    // Preenche lc inicial com todos os indices
    for (int i = 2; i <= n; i++) {
        lista_candidatos.insert(i);
    }

    // Cria caminho guloso-aleatorio
    while (lista_candidatos.size() > 0) {
        vector<int> lrc = constroi_lista_restrita(lista_candidatos, caminho, grafo);
        uniform_int_distribution<int> dist(0, lrc.size() - 1);
        auto it = lista_candidatos.begin();
        advance(it, dist(gen));
        caminho.push_back(*it);
        lista_candidatos.erase(*it);
    }

    return caminho;
}

vector<int> busca_local(vector<int>& caminho, map<int, unordered_map<int, double>>& grafo) {
    int contador = 0;
    vector<int> melhor_caminho(caminho.size());
    double melhor_avaliacao = -1;

    do {
        contador++;
        double avaliacao = avalia(caminho, grafo);

        if (melhor_avaliacao == -1 || melhor_avaliacao > avaliacao) {
            melhor_avaliacao = avaliacao;
            melhor_caminho = caminho;
        }
    } while (contador < ITERACOES_BUSCA_LOCAL && next_permutation(caminho.begin(), caminho.end()));

    return melhor_caminho;
}

vector<int> grasp(int n, map<int, unordered_map<int, double>>& grafo) {
    vector<int> melhor;

    for (int i = 0; i < ITERACOES_GRASP; i++) {
        vector<int> aleatorio_guloso = caminho_aleatorio_guloso(n, grafo);
        vector<int> otimo_local = busca_local(aleatorio_guloso, grafo);
        if (melhor.size() == 0 || avalia(melhor, grafo) > avalia(otimo_local, grafo)) {
            melhor = otimo_local;
        }
    }

    return melhor;
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

    vector<int> resultado = grasp(n, grafo);
    imprime_caminho(resultado);
    cout << "Valor: " << avalia(resultado, grafo);

    return 0;
}