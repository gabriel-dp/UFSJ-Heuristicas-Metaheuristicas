#include <bits/stdc++.h>

using namespace std;

#define ITERACOES 10000

pair<int, int> avalia(vector<bool> mochila, vector<pair<int, int>> itens, int capacidade) {  // <peso, valor>
    pair<int, int> resultado = {0, 0};
    for (int i = 0; i < mochila.size(); i++) {
        if (mochila[i]) {
            resultado.first += itens[i].first;
            resultado.second += itens[i].second;
        }
    }
    return (resultado.first <= capacidade) ? resultado : make_pair(-1, -1);
}

void imprime_mochila(vector<bool> mochila) {
    for (int i = 0; i < mochila.size(); i++) {
        cout << "|" << mochila[i];
    }
    cout << "|\n";
}

void imprime_mochila(vector<bool> mochila, pair<int, int> avaliacao) {
    cout << "Peso: " << avaliacao.first << " | Valor: " << avaliacao.second << " >>> ";
    imprime_mochila(mochila);
}

int main() {
    int n, capacidade, peso, valor;

    cin >> n >> capacidade;

    vector<pair<int, int>> itens(n);
    vector<bool> mochila(n, false);
    for (int i = 0; i < n; i++) {
        cin >> peso >> valor;
        itens[i] = {peso, valor};
        mochila[i] = false;
    }

    // Ordena para facilitar a heuristica
    sort(itens.begin(), itens.end(), [](const pair<int, int>& lhs, const pair<int, int>& rhs) {
        return lhs.first < rhs.first;  // Menor peso primeiro
        // return ((double)lhs.second / lhs.first) > ((double)rhs.second / rhs.first);  // Maior custo-beneficio primeiro
    });

    // Coloca os itens na mochila na ordem ate quando nao couber mais
    for (int i = 0; i < n; i++) {
        if (avalia(mochila, itens, capacidade).first != -1) {
            mochila[i] = true;
        } else {
            mochila[i - 1] = false;
            break;
        }
    }

    vector<bool> melhor_mochila(mochila);
    pair<int, int> melhor_avaliacao = avalia(melhor_mochila, itens, capacidade);
    cout << "Inicial\n";
    imprime_mochila(mochila, melhor_avaliacao);
    cout << "\n\n";

    // Tenta colocar um item, caso nao de, tira os dois ultimos colocados e prossegue
    for (int i = 0; i < ITERACOES && i < n; i++) {
        if (!mochila[i])
            mochila[i] = true;
        else
            continue;

        pair<int, int> avaliacao = avalia(mochila, itens, capacidade);
        if (avaliacao.first != -1) {
            imprime_mochila(mochila, avaliacao);
            if (avaliacao.second > melhor_avaliacao.second) {
                melhor_avaliacao = avaliacao;
                melhor_mochila = mochila;
            }
        } else {
            if (mochila[i]) mochila[i] = false;
            *find(mochila.rbegin(), mochila.rend(), true) = false;
        }
    }

    cout << "\n\nMelhor\n";
    imprime_mochila(melhor_mochila, melhor_avaliacao);

    return 0;
}
