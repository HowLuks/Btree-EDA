#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class NoDaArvore {
    int *chaves;  // Array de chaves
    int grau;      // Grau mínimo (t é o grau da árvore B)
    NoDaArvore **C; // Array de ponteiros para os filhos
    int numChaves;     // Número atual de chaves
    bool folha; // Verdadeiro se o nó for folha

public:
    NoDaArvore(int grau1, bool folha1);

    // Função para percorrer todos os nós em uma subárvore enraizada neste nó
    void traverse();

    // Função para buscar uma chave na subárvore enraizada neste nó
    NoDaArvore *busca(int k);

    // Função que insere uma nova chave na subárvore enraizada neste nó
    void inserir(int k);

    // Função que divide o filho y deste nó. i é o índice de y em C[]
    void separar(int i, NoDaArvore *y);

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

    void traverse() {
        if (raiz != NULL) raiz->traverse();
    }

    NoDaArvore* busca(int k) {
        return (raiz == NULL) ? NULL : raiz->busca(k);
    }

    void insert(int k);
};

NoDaArvore::NoDaArvore(int grau1, bool folha1) {
    grau = grau1;
    folha = folha1;
    chaves = new int[2*grau-1];
    C = new NoDaArvore *[2*grau];
    numChaves = 0;
}

void NoDaArvore::traverse() {
    int i;
    for (i = 0; i < numChaves; i++) {
        if (!folha) {
            C[i]->traverse();
        }
        cout << " " << chaves[i];
    }
    if (!folha) {
        C[i]->traverse();
    }
}

NoDaArvore* NoDaArvore::busca(int k) {
    int i = 0;
    while (i < numChaves && k > chaves[i]) {
        i++;
    }
    if (chaves[i] == k) {
        return this;
    }
    if (folha) {
        return NULL;
    }
    return C[i]->busca(k);
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
    BTree t(3); // Criando uma Árvore B com grau 3
    t.insert(10);
    t.insert(20);
    t.insert(5);
    t.insert(6);
    t.insert(12);
    t.insert(30);
    t.insert(7);
    t.insert(17);
    printf("inseriu");

    cout << "Percorrendo a arvore construida\n";
    t.traverse();
    printf("printou");
    int k = 6;
    (t.busca(k) != NULL) ? cout << "\nPresente" : cout << "\nNao Presente\n";

    k = 15;
    (t.busca(k) != NULL) ? cout << "\nPresente" : cout << "\nNao Presente\n";

    return 0;
}
