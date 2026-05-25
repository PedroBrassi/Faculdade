#ifndef _H_TRABALHOUM
#define _H_TRABALHOUM

#include <stdbool.h>

// ---------------- Estruturas de letras ----------------
typedef struct nohFrase {
    char letra;                  // caractere da linha
    struct nohFrase* proxLetra;  // próximo caractere
} nohFrase;

// ---------------- Estruturas de linhas ----------------
typedef struct nohLinha {
    nohFrase* linha;             // lista encadeada de caracteres da linha
    struct nohLinha* proxLinha;  // próxima linha
} nohLinha;

// ---------------- Pilha de linhas ----------------
typedef struct {
    nohLinha* topo;              // topo da pilha
} Pilha;

// ---------------- Fila de linhas ----------------
typedef struct {
    nohLinha* inicio;            // início da fila
} Fila;

// ---------------- Pilha de letras ----------------
typedef struct {
    nohFrase* topo;              // topo da pilha de letras
} PilhaLetra;

// ---------------- Fila de letras ----------------
typedef struct {
    nohFrase* inicio;            // início da fila de letras
    nohFrase* fim;               // fim da fila de letras
} FilaLetra;

// ---------------- Inicialização ----------------
void initialize(Pilha* p, Fila* f);  // inicializa pilha e fila de linhas

// ---------------- Criação de nós ----------------
nohFrase* criarFrase(const char* texto);  // cria lista de letras a partir de uma string
nohLinha* criarNoh(const char* texto);    // cria nó de linha com lista de letras

// ---------------- Pilha/Fila de linhas ----------------
void push(Pilha* p, nohLinha* no);        // adiciona linha na pilha
void enqueue(Fila* f, nohLinha* no);      // adiciona linha na fila

// ---------------- Pilha/Fila de letras ----------------
void pushLetra(PilhaLetra* p, char c);    // adiciona letra na pilha
char popLetra(PilhaLetra* p);             // remove letra da pilha
void enqueueLetra(FilaLetra* f, char c);  // adiciona letra na fila
char dequeueLetra(FilaLetra* f);          // remove letra da fila

// ---------------- Verificação de palíndromo ----------------
bool isPalindromo(nohFrase* inicio);      // retorna true se a linha for palíndromo

// ---------------- Leitura de arquivo ----------------
void lerArquivo(Pilha* p, Fila* f);      // lê "entrada.txt" e preenche pilha e fila de linhas

#endif
