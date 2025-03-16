#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_INICIAL 10
#define FATOR_CARGA_MAXIMO 0.7

typedef struct node {
    char key[50];
    int valor;
    struct node* next;
} node;

unsigned int hash(const char* key, unsigned int tamanho_tabela) {
    unsigned int hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c; 
    }
    return hash % tamanho_tabela;
}

node* criarNo(const char* key, int value) {
    node* novoNo = (node*)malloc(sizeof(node));
    if (!novoNo) {
        printf("Erro ao alocar memória.\n");
        return NULL;
    }
    strcpy(novoNo->key, key);
    novoNo->valor = value;
    novoNo->next = NULL;
    return novoNo;
}

void insert(node** table, unsigned int* tamanho_tabela, unsigned int* num_elementos, const char* key, int value) {
   
    if ((double)(*num_elementos) / (*tamanho_tabela) >= FATOR_CARGA_MAXIMO) {
        unsigned int novoTamanho = (*tamanho_tabela) * 2;
        node** novaTabela = (node**)calloc(novoTamanho, sizeof(node*));
        if (!novaTabela) {
            printf("Erro ao alocar memória para redimensionamento.\n");
            return;
        }

        for (unsigned int i = 0; i < (*tamanho_tabela); i++) {
            node* current = table[i];
            while (current) {
                node* next = current->next;
                unsigned int novoIndice = hash(current->key, novoTamanho);
                current->next = novaTabela[novoIndice];
                novaTabela[novoIndice] = current;
                current = next;
            }
        }

        free(table);
        table = novaTabela;
        *tamanho_tabela = novoTamanho;
    }

    unsigned int index = hash(key, *tamanho_tabela);
    node* novoNo = criarNo(key, value);
    if (!novoNo) return;

    novoNo->next = table[index];
    table[index] = novoNo;
    (*num_elementos)++;
}

int search(node** table, unsigned int tamanho_tabela, const char* key) {
    unsigned int index = hash(key, tamanho_tabela);
    node* current = table[index];
    while (current) {
        if (strcmp(current->key, key) == 0) {
            return current->valor;
        }
        current = current->next;
    }
    return -1; 
}

void removeKey(node** table, unsigned int tamanho_tabela, unsigned int* num_elementos, const char* key) {
    unsigned int index = hash(key, tamanho_tabela);
    node* current = table[index];
    node* prev = NULL;

    while (current) {
        if (strcmp(current->key, key) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                table[index] = current->next;
            }
            free(current);
            (*num_elementos)--;
            printf("Chave '%s' removida.\n", key);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Chave '%s' não encontrada.\n", key);
}

void liberarTabela(node** table, unsigned int tamanho_tabela) {
    for (unsigned int i = 0; i < tamanho_tabela; i++) {
        node* current = table[i];
        while (current) {
            node* next = current->next;
            free(current);
            current = next;
        }
    }
    free(table);
}

void printTable(node** table, unsigned int tamanho_tabela) {
    for (unsigned int i = 0; i < tamanho_tabela; i++) {
        printf("Índice %d: ", i);
        node* current = table[i];
        while (current) {
            printf("(%s, %d) -> ", current->key, current->valor);
            current = current->next;
        }
        printf("NULL\n");
    }
}

void menu() {
    printf("\n===== MENU =====\n");
    printf("1. Adicionar\n");
    printf("2. Buscar\n");
    printf("3. Remover\n");
    printf("4. Exibir a Tabela\n");
    printf("5. Sair\n");
    printf("=================\n");
    printf("Escolha uma opcao: ");
}

int main() {
    unsigned int tamanho_tabela = TAMANHO_INICIAL;
    unsigned int num_elementos = 0;
    node** hashTable = (node**)calloc(tamanho_tabela, sizeof(node*));
    if (!hashTable) {
        printf("Erro ao alocar memória para a tabela hash.\n");
        return 1;
    }

    int opcao, valor;
    char chave[50];

    while (1) {
        menu();
        scanf("%d", &opcao);

        switch (opcao) {
        case 1:
            printf("Digite a chave: ");
            scanf("%s", chave);
            printf("Digite o valor: ");
            scanf("%d", &valor);
            insert(hashTable, &tamanho_tabela, &num_elementos, chave, valor);
            printf("Entrada adicionada com sucesso!\n");
            break;

        case 2:
            printf("Digite a chave para buscar: ");
            scanf("%s", chave);
            valor = search(hashTable, tamanho_tabela, chave);
            if (valor != -1) {
                printf("Valor encontrado: %d\n", valor);
            } else {
                printf("Chave '%s' não encontrada.\n", chave);
            }
            break;

        case 3:
            printf("Digite a chave para remover: ");
            scanf("%s", chave);
            removeKey(hashTable, tamanho_tabela, &num_elementos, chave);
            break;

        case 4:
            printTable(hashTable, tamanho_tabela);
            break;

        case 5:
            liberarTabela(hashTable, tamanho_tabela);
            printf("Memória liberada. Saindo do programa.\n");
            return 0;

        default:
            printf("Opção inválida. Tente novamente.\n");
        }
    }
    return 0;
}
