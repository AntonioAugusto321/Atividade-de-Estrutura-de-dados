#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

void imprimir_tabuleiro(vector<vector<char>> &tabuleiro) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << tabuleiro[i][j] << (j < 2 ? " | " : "");
        }
        cout << endl;
        if (i < 2) cout << "---------" << endl;
    }
}

bool verificar_vitoria(vector<vector<char>> &tabuleiro, char jogador) {
    for (int i = 0; i < 3; i++) {
        if (tabuleiro[i][0] == jogador && tabuleiro[i][1] == jogador && tabuleiro[i][2] == jogador) return true; // Linhas
        if (tabuleiro[0][i] == jogador && tabuleiro[1][i] == jogador && tabuleiro[2][i] == jogador) return true; // Colunas
    }
    if (tabuleiro[0][0] == jogador && tabuleiro[1][1] == jogador && tabuleiro[2][2] == jogador) return true; // Diagonal principal
    if (tabuleiro[0][2] == jogador && tabuleiro[1][1] == jogador && tabuleiro[2][0] == jogador) return true; // Diagonal secundária
    return false;
}

bool tabuleiro_cheio(vector<vector<char>> &tabuleiro) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tabuleiro[i][j] == ' ') return false;
        }
    }
    return true;
}

vector<pair<int, int>> posicoes_livres(vector<vector<char>> &tabuleiro) {
    vector<pair<int, int>> posicoes;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tabuleiro[i][j] == ' ') posicoes.push_back({i, j});
        }
    }
    return posicoes;
}

//  encontra a melhor jogada
pair<int, int> melhor_jogada(vector<vector<char>> &tabuleiro, char jogador, char oponente) {
    // ganhar ou evitar derrota
    for (auto pos : posicoes_livres(tabuleiro)) {
        int linha = pos.first, coluna = pos.second;
        tabuleiro[linha][coluna] = jogador;
        if (verificar_vitoria(tabuleiro, jogador)) return {linha, coluna};
        tabuleiro[linha][coluna] = oponente;
        if (verificar_vitoria(tabuleiro, oponente)) {
            tabuleiro[linha][coluna] = ' ';
            return {linha, coluna};
        }
        tabuleiro[linha][coluna] = ' ';
    }
    
    // centro livre
    if (tabuleiro[1][1] == ' ') return {1, 1};

    // canto oposto
    vector<pair<int, int>> cantos = {{0, 0}, {0, 2}, {2, 0}, {2, 2}};
    for (int i = 0; i < 4; i++) {
        if (tabuleiro[cantos[i].first][cantos[i].second] == oponente &&
            tabuleiro[2 - cantos[i].first][2 - cantos[i].second] == ' ') {
            return {2 - cantos[i].first, 2 - cantos[i].second};
        }
    }

    // canto vazio
    for (auto canto : cantos) {
        if (tabuleiro[canto.first][canto.second] == ' ') return canto;
    }

    // arbitrário
    vector<pair<int, int>> livres = posicoes_livres(tabuleiro);
    return livres[rand() % livres.size()];
}

int main() {
    srand(time(0));
    vector<vector<char>> tabuleiro(3, vector<char>(3, ' '));
    char jogador = 'X', oponente = 'O';

    cout << "Bem-vindo ao Jogo da Velha\n\n";
    imprimir_tabuleiro(tabuleiro);

    while (!tabuleiro_cheio(tabuleiro)) {
        pair<int, int> jogada = melhor_jogada(tabuleiro, jogador, oponente);
        tabuleiro[jogada.first][jogada.second] = jogador;
        cout << "\nJogada do jogador (" << jogador << "): (" << jogada.first + 1 << ", " << jogada.second + 1 << ")" << endl;
        imprimir_tabuleiro(tabuleiro);
        if (verificar_vitoria(tabuleiro, jogador)) {
            cout << "\nO jogador venceu!" << endl;
            return 0;
        }
        if (tabuleiro_cheio(tabuleiro)) break;

        // Inimigo
        int linha, coluna;
        while (true) {
            cout << "\nInforme a linha (1-3) e coluna (1-3): ";
            cin >> linha >> coluna;
            linha--; coluna--;
            if (linha >= 0 && linha < 3 && coluna >= 0 && coluna < 3 && tabuleiro[linha][coluna] == ' ') {
                tabuleiro[linha][coluna] = oponente;
                break;
            }
            cout << "Posição inválida. Tente novamente." << endl;
        }
        imprimir_tabuleiro(tabuleiro);
        if (verificar_vitoria(tabuleiro, oponente)) {
            cout << "\nParabéns!" << endl;
            return 0;
        }
    }

    cout << "\nempate!" << endl;
    return 0;
}
