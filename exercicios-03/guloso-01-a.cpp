#include <bits/stdc++.h>

using namespace std;

#define ITERACOES 10000

pair<int, int> avalia(vector<int> mochila, vector<pair<int, int>> itens, int capacidade) {  // <peso, valor>
    pair<int, int> resultado = {0, 0};
    for (int i = 0; i < mochila.size(); i++) {
        if (mochila[i]) {
            resultado.first += itens[i].first;
            resultado.second += itens[i].second;
        }
    }
    return (resultado.first <= capacidade) ? resultado : make_pair(-1, -1);
}

int main() {
    int n, capacidade, peso, valor;

    cin >> n >> capacidade;

    vector<pair<int, int>> itens(n);
    vector<int> mochila(n, 0);
    for (int i = 0; i < n; i++) {
        cin >> peso >> valor;
        itens[i] = {peso, valor};
        mochila[i] = 0;
    }

    // Ordena para facilitar a heuristica
    sort(itens.begin(), itens.end(), [](const pair<int, int>& lhs, const pair<int, int>& rhs) {
        return lhs.first < rhs.first;  // Menor peso primeiro
        // return lhs.second > rhs.second;  // Maior valor primeiro
    });

    // Coloca os itens na mochila na ordem ate quando nao couber mais
    for (int i = 0; i < n; i++) {
        if (avalia(mochila, itens, capacidade).first != -1) {
            mochila[i] = 1;
        } else {
            mochila[i - 1] = 0;
            break;
        }
    }

    for (auto item : mochila) {
        cout << item << "|";
    }
    cout << "\n";

    // Avaliação usando permutações
    int contador = 0;
    do {
        cout << "TESTE\n";
        pair<int, int> avaliacao = avalia(mochila, itens, capacidade);
        if (avaliacao.first != -1) {
            cout << "Peso: " << avaliacao.first << " | Valor: " << avaliacao.second << " >>> ";
            for (int i = 0; i < mochila.size(); i++) {
                cout << mochila[i] << "|";
            }
            cout << "\n";
        }
        contador++;
    } while (contador < ITERACOES && next_permutation(mochila.begin(), mochila.end()));

    return 0;
}
