#include "trabalhoUm.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// ---------- Inicialização ----------
void initialize(Pilha* p, Fila* f) {
    p->topo = NULL;
    f->inicio = NULL;
}

// ---------- Criação de nós ----------
nohFrase* criarFrase(const char* texto) {
    nohFrase* inicio = NULL;
    nohFrase* atual = NULL;
    for (int i = 0; texto[i]; i++) {
        nohFrase* novo = (nohFrase*) malloc(sizeof(nohFrase));
        novo->letra = texto[i];
        novo->proxLetra = NULL;
        if (!inicio) inicio = atual = novo;
        else {
            atual->proxLetra = novo;
            atual = novo;
        }
    }
    return inicio;
}

nohLinha* criarNoh(const char* texto) {
    nohLinha* novo = (nohLinha*) malloc(sizeof(nohLinha));
    novo->linha = criarFrase(texto);
    novo->proxLinha = NULL;
    return novo;
}

// ---------- Pilha/Fila de linhas ----------
void push(Pilha* p, nohLinha* no) {
    no->proxLinha = p->topo;
    p->topo = no;
}

void enqueue(Fila* f, nohLinha* no) {
    no->proxLinha = NULL;
    if (!f->inicio) f->inicio = no;
    else {
        nohLinha* atual = f->inicio;
        while (atual->proxLinha) atual = atual->proxLinha;
        atual->proxLinha = no;
    }
}

// ---------- Pilha/Fila de letras ----------
void pushLetra(PilhaLetra* p, char c) {
    nohFrase* novo = (nohFrase*) malloc(sizeof(nohFrase));
    novo->letra = c;
    novo->proxLetra = p->topo;
    p->topo = novo;
}

char popLetra(PilhaLetra* p) {
    nohFrase* temp = p->topo;
    char c = temp->letra;
    p->topo = temp->proxLetra;
    free(temp);
    return c;
}

void enqueueLetra(FilaLetra* f, char c) {
    nohFrase* novo = (nohFrase*) malloc(sizeof(nohFrase));
    novo->letra = c;
    novo->proxLetra = NULL;
    if (!f->fim) f->inicio = f->fim = novo;
    else {
        f->fim->proxLetra = novo;
        f->fim = novo;
    }
}

char dequeueLetra(FilaLetra* f) {
    nohFrase* temp = f->inicio;
    char c = temp->letra;
    f->inicio = temp->proxLetra;
    if (!f->inicio) f->fim = NULL;
    free(temp);
    return c;
}

// ---------- Verifica palíndromo ----------
bool isPalindromo(nohFrase* inicio) {
    PilhaLetra pilha = {NULL};
    FilaLetra fila = {NULL, NULL};

    for (nohFrase* atual = inicio; atual; atual = atual->proxLetra) {
        if (isalpha(atual->letra)) {          // considera apenas letras
            char c = tolower(atual->letra);   // converte para minúscula
            pushLetra(&pilha, c);
            enqueueLetra(&fila, c);
        }
    }

    while (pilha.topo && fila.inicio)
        if (popLetra(&pilha) != dequeueLetra(&fila))
            return false;

    return true;
}

// ---------- Lê arquivo e imprime ----------
void lerArquivo(Pilha* p, Fila* f) {
    FILE* arquivo = fopen("entrada.txt", "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), arquivo)) {
        int len = 0;
        while (buffer[len]) len++;
        if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';

        nohLinha* no = criarNoh(buffer);
        push(p, no);
        enqueue(f, no);

        printf("%d\n", isPalindromo(no->linha)); // 1 se palíndromo, 0 caso contrário
    }

    fclose(arquivo);
}

// ---------- Main ----------
int main() {
    Pilha p;
    Fila f;
    initialize(&p, &f);

    lerArquivo(&p, &f);

    return 0;
}
