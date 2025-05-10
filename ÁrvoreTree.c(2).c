#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define ALFABETO 26

typedef struct NoTrie {
    struct NoTrie* filhos[ALFABETO];
    bool fim_palavra;
    unsigned int uso;
} NoTrie;

NoTrie* criar_no() {
    NoTrie* novo = (NoTrie*)calloc(1, sizeof(NoTrie));
    if (!novo) {
        fprintf(stderr, "Erro: Falha ao alocar memória\n");
        exit(EXIT_FAILURE);
    }
    return novo;
}

int mapear_char(char c) {
    c = tolower(c);
    return (c >= 'a' && c <= 'z') ? c - 'a' : -1;
}

void inserir_palavra(NoTrie* raiz, const char* palavra) {
    if (!raiz || !palavra) return;

    NoTrie* atual = raiz;
    for (int i = 0; palavra[i]; i++) {
        int idx = mapear_char(palavra[i]);
        if (idx == -1) continue;
        
        if (!atual->filhos[idx]) {
            atual->filhos[idx] = criar_no();
        }
        atual = atual->filhos[idx];
        atual->uso++;
    }
    atual->fim_palavra = true;
}

bool buscar_palavra(NoTrie* raiz, const char* palavra) {
    if (!raiz || !palavra) return false;

    NoTrie* atual = raiz;
    for (int i = 0; palavra[i]; i++) {
        int idx = mapear_char(palavra[i]);
        if (idx == -1 || !atual->filhos[idx]) return false;
        atual = atual->filhos[idx];
    }
    return atual->fim_palavra;
}

bool no_tem_filhos(NoTrie* no) {
    if (!no) return false;
    for (int i = 0; i < ALFABETO; i++) {
        if (no->filhos[i]) return true;
    }
    return false;
}

bool remover_palavra_rec(NoTrie* no, const char* palavra, int profundidade) {
    if (!no) return false;

    if (palavra[profundidade]) {
        int idx = mapear_char(palavra[profundidade]);
        if (idx == -1 || !no->filhos[idx]) return false;

        if (remover_palavra_rec(no->filhos[idx], palavra, profundidade + 1)) {
            no->filhos[idx]->uso--;
            if (!no_tem_filhos(no->filhos[idx]) && !no->filhos[idx]->fim_palavra) {
                free(no->filhos[idx]);
                no->filhos[idx] = NULL;
                return no->uso == 0;
            }
        }
    } else if (no->fim_palavra) {
        no->fim_palavra = false;
        return !no_tem_filhos(no);
    }
    return false;
}

void remover_palavra(NoTrie* raiz, const char* palavra) {
    if (buscar_palavra(raiz, palavra)) {
        remover_palavra_rec(raiz, palavra, 0);
    }
}

void liberar_trie(NoTrie* no) {
    if (!no) return;
    for (int i = 0; i < ALFABETO; i++) {
        if (no->filhos[i]) liberar_trie(no->filhos[i]);
    }
    free(no);
}

/* Função melhorada para exibir a Trie como árvore */
void exibir_trie_visual(NoTrie* no, char* prefixo, int nivel, bool* marcadores) {
    if (!no) return;

    // Imprime a estrutura em árvore
    for (int i = 0; i < nivel; i++) {
        printf("%s", (i == 0) ? "" : (marcadores[i] ? "│   " : "    "));
    }
    
    if (nivel > 0) {
        printf("%s──%c", marcadores[nivel] ? "├" : "└", prefixo[nivel-1]);
        if (no->fim_palavra) printf(" (fim)");
        printf("\n");
    }

    // Conta quantos filhos existem
    int contador = 0;
    for (int i = 0; i < ALFABETO; i++) {
        if (no->filhos[i]) contador++;
    }

    // Imprime os filhos
    int impressos = 0;
    for (int i = 0; i < ALFABETO; i++) {
        if (no->filhos[i]) {
            prefixo[nivel] = 'a' + i;
            marcadores[nivel+1] = (impressos < contador-1);
            exibir_trie_visual(no->filhos[i], prefixo, nivel+1, marcadores);
            impressos++;
        }
    }
}

void menu_interativo() {
    NoTrie* raiz = criar_no();
    char palavra[100];
    int opcao;
    
    while (1) {
        printf("\n=== MENU TRIE ===\n");
        printf("1. Inserir palavra\n");
        printf("2. Buscar palavra\n");
        printf("3. Remover palavra\n");
        printf("4. Mostrar árvore\n");
        printf("5. Sair\n");
        printf("Opção: ");
        
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            printf("Entrada inválida!\n");
            continue;
        }
        
        switch (opcao) {
            case 1:
                printf("Palavra: ");
                scanf("%99s", palavra);
                inserir_palavra(raiz, palavra);
                break;
                
            case 2:
                printf("Palavra: ");
                scanf("%99s", palavra);
                printf(buscar_palavra(raiz, palavra) ? "Encontrada!\n" : "Não encontrada!\n");
                break;
                
            case 3:
                printf("Palavra: ");
                scanf("%99s", palavra);
                remover_palavra(raiz, palavra);
                break;
                
            case 4: {
                printf("\nESTRUTURA DA TRIE:\n");
                char prefixo[100] = {0};
                bool marcadores[100] = {false};
                exibir_trie_visual(raiz, prefixo, 0, marcadores);
                break;
            }
                
            case 5:
                liberar_trie(raiz);
                printf("Saindo...\n");
                return;
                
            default:
                printf("Opção inválida!\n");
        }
        while (getchar() != '\n');
    }
}

int main() {
    menu_interativo();
    return 0;
}