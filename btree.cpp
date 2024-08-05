#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
using namespace std;

class NoDaArvore {
    int *chaves;  // Array de chaves
    int grau;      // Grau mínimo (t é o grau da árvore B)
    NoDaArvore **C; // Array de ponteiros para os filhos
    int numChaves;     // Número atual de chaves
    bool folha; // Verdadeiro se o nó for folha

public:
    NoDaArvore(int grau1, bool folha1);

    void inserir(int k);

    void separar(int i, NoDaArvore *y);

    void printarq(FILE *saida, int level);

    friend class BTree;
};

class BTree {
    NoDaArvore *raiz; // Ponteiro para a raiz
    int grau;  // Grau mínimo

public:
    BTree(int _grau) {
        raiz = NULL;
        grau = _grau;
    }

    void insert(int k);

    int altura();

    void printarq();
};

NoDaArvore::NoDaArvore(int grau1, bool folha1) {
    grau = grau1;
    folha = folha1;
    chaves = new int[2*grau-1];
    C = new NoDaArvore *[2*grau];
    numChaves = 0;
}

void BTree::insert(int k) {
    if (raiz == NULL) {
        raiz = new NoDaArvore(grau, true);
        raiz->chaves[0] = k;
        raiz->numChaves = 1;
    } else {
        if (raiz->numChaves == 2*grau-1) {
            NoDaArvore *s = new NoDaArvore(grau, false);
            s->C[0] = raiz;
            s->separar(0, raiz);
            int i = 0;
            if (s->chaves[0] < k) {
                i++;
            }
            s->C[i]->inserir(k);
            raiz = s;
        } else {
            raiz->inserir(k);
        }
    }
}

int BTree::altura() {
    int alt = 0;
    NoDaArvore *atual = raiz;
    while (atual != NULL) {
        alt++;
        atual = (atual->folha) ? NULL : atual->C[0];
    }
    return alt;
}

void BTree::printarq() {
    FILE *saida = fopen("saida.txt", "w");
    if (saida != NULL) {
        int alt = altura();
        fprintf(saida, "Ordem: %d, Níveis: %d\n", grau, alt);
        if (raiz != NULL) raiz->printarq(saida, 1);
        fclose(saida);
    } else {
        cout << "Não foi possível abrir o arquivo de saída.\n";
    }
}

void NoDaArvore::printarq(FILE *saida, int level) {
    queue<pair<NoDaArvore*, int>> nos;
    nos.push({this, level});

    while (!nos.empty()) {
        int numNo = nos.size();
        int nivelAtual = nos.front().second;
        fprintf(saida, "%d - ", nivelAtual);
        while (numNo > 0) {
            NoDaArvore *atualNo = nos.front().first;
            nos.pop();

            for (int i = 0; i < atualNo->numChaves; i++) {
                fprintf(saida, "%d", atualNo->chaves[i]);
                if (i < atualNo->numChaves - 1) {
                    fprintf(saida, ", ");
                }
            }
            if (numNo > 1) {
                fprintf(saida, " - ");
            }

            if (!atualNo->folha) {
                for (int i = 0; i <= atualNo->numChaves; i++) {
                    nos.push({atualNo->C[i], nivelAtual + 1});
                }
            }
            numNo--;
        }
        fprintf(saida, "\n");
    }
}


void NoDaArvore::inserir(int k) {
    int i = numChaves-1;
    if (folha) {
        while (i >= 0 && chaves[i] > k) {
            chaves[i+1] = chaves[i];
            i--;
        }
        chaves[i+1] = k;
        numChaves = numChaves+1;
    } else {
        while (i >= 0 && chaves[i] > k) {
            i--;
        }
        if (C[i+1]->numChaves == 2*grau-1) {
            separar(i+1, C[i+1]);
            if (chaves[i+1] < k) {
                i++;
            }
        }
        C[i+1]->inserir(k);
    }
}

void NoDaArvore::separar(int i, NoDaArvore *y) {
    NoDaArvore *z = new NoDaArvore(y->grau, y->folha);
    z->numChaves = grau - 1;
    for (int j = 0; j < grau-1; j++) {
        z->chaves[j] = y->chaves[j+grau];
    }
    if (!y->folha) {
        for (int j = 0; j < grau; j++) {
            z->C[j] = y->C[j+grau];
        }
    }
    y->numChaves = grau - 1;
    for (int j = numChaves; j >= i+1; j--) {
        C[j+1] = C[j];
    }
    C[i+1] = z;
    for (int j = numChaves-1; j >= i; j--) {
        chaves[j+1] = chaves[j];
    }
    chaves[i] = y->chaves[grau-1];
    numChaves = numChaves + 1;
}



int main() {

    FILE *arquivo;
    int entrada[201];
    int aux = 0;

    
    arquivo = fopen("entrada.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }

    
    while (fscanf(arquivo, "%d", &entrada[aux]) != EOF) {
        aux++;
        if (aux >= 201) {
            printf("Vetor cheio. Certifique-se de que o número de linhas no arquivo não exceda %d.\n", 201);
            break;
        }
    }

    fclose(arquivo);

    
    for (int j = 0; j < aux; j++) {
        printf("Número %d: %d\n", j+1, entrada[j]);
    }

    BTree t(entrada[0]);
    for (int j = 1; j < aux; j++) {
        t.insert(entrada[j]);
    }
    
    printf("inseriu");

    t.printarq();

    return 0;
}
