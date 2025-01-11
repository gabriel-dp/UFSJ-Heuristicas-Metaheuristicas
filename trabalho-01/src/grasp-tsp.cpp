#include <bits/stdc++.h>

using namespace std;

#ifdef PRINT
#define LOG(x) x;
#else
#define LOG(x)
#endif

/* Constantes do algoritmo */
double ITERACOES = 10000;
double A = 0.05;

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

/* Seleciona candidatos para serem escolhidos aleatoriamente para fazer parte do caminho */
vector<int> constroi_lista_restrita(set<int>& lista_candidatos, vector<int>& caminho, map<int, map<int, double>>& grafo) {
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
    for (size_t i = 0; i < avaliacao.size(); i++) {
        if (avaliacao[i] <= cmin + A * (cmax - cmin)) {
            auto it = lista_candidatos.begin();
            advance(it, i);
            lista_restrita.push_back(*it);
        }
    }

    return lista_restrita;
}

/* Cria solucao usando uma abordagem aleatoria-gulosa */
vector<int> solucao_aleatorio_guloso(map<int, map<int, double>>& grafo) {
    vector<int> caminho = {1};
    set<int> lista_candidatos;

    // Preenche lc inicial com todos os indices
    for (size_t i = 2; i <= grafo.size(); i++) {
        lista_candidatos.insert(i);
    }

    // Cria caminho guloso-aleatorio
    while (lista_candidatos.size() > 0) {
        vector<int> lrc = constroi_lista_restrita(lista_candidatos, caminho, grafo);
        uniform_int_distribution<int> dist(0, lrc.size() - 1);
        int aleatorio = lrc[dist(gen)];
        caminho.push_back(aleatorio);
        lista_candidatos.erase(aleatorio);
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

/* Funcao do GRASP */
vector<int> grasp(map<int, map<int, double>>& grafo) {
    vector<int> melhor;

    for (int i = 0; i < ITERACOES; i++) {
        vector<int> aleatorio_guloso = solucao_aleatorio_guloso(grafo);
        vector<int> otimo_local = busca_local(aleatorio_guloso, grafo);
        LOG(imprime_caminho(otimo_local, grafo));
        if (melhor.size() == 0 || avalia(melhor, grafo) > avalia(otimo_local, grafo)) {
            melhor = otimo_local;
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
    auto grafo = recebe_grafo();

    vector<int> melhor_encontrado = grasp(grafo);
    imprime_caminho(melhor_encontrado, grafo);

    return 0;
}