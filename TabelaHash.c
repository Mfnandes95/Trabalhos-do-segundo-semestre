#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_TABELA 10

// Estrutura do nó
typedef struct node {
    char key[50];
    int valor; // Corrigido para manter consistência com o uso no código
    struct node* next;
} node;

// Função de hash
unsigned int hash(const char* key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash << 5) + *key++;
    }
    return hash % TAMANHO_TABELA;
}

// Função para inserir na tabela
void insert(node* table[], const char* key, int value) {
    unsigned int index = hash(key);
    node* newNode = (node*)malloc(sizeof(node));
    if (!newNode) {
        printf("Erro ao alocar memória.\n");
        return;
    }
    strcpy(newNode->key, key);
    newNode->valor = value; // Corrigido: atribui ao membro correto
    newNode->next = table[index];
    table[index] = newNode;
}

// Função para buscar na tabela
int search(node* table[], const char* key) {
    unsigned int index = hash(key);
    node* current = table[index];
    while (current) {
        if (strcmp(current->key, key) == 0) {
            return current->valor;
        }
        current = current->next;
    }
    return -1; // Retorna -1 se não encontrado
}

// Função para remover um elemento da tabela
void removeKey(node* table[], const char* key) {
    unsigned int index = hash(key);
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
            printf("Chave '%s' removida.\n", key);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Chave '%s' não encontrada.\n", key);
}

// Função para imprimir a tabela
void printTable(node* table[]) {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        printf("Índice %d: ", i);
        node* current = table[i];
        while (current) {
            printf("(%s, %d) -> ", current->key, current->valor);
            current = current->next;
        }
        printf("NULL\n");
    }
}

// Exibe o menu de opções
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
    node* hashTable[TAMANHO_TABELA] = { NULL };
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
            insert(hashTable, chave, valor);
            printf("Entrada adicionada com sucesso!\n");
            break;

        case 2:
            printf("Digite a chave para buscar: ");
            scanf("%s", chave);
            valor = search(hashTable, chave);
            if (valor != -1) {
                printf("Valor encontrado: %d\n", valor);
            } else {
                printf("Chave '%s' não encontrada.\n", chave);
            }
            break;

        case 3:
            printf("Digite a chave para remover: ");
            scanf("%s", chave);
            removeKey(hashTable, chave);
            break;

        case 4:
            printTable(hashTable);
            break;

        case 5:
            printf("Saindo do programa.\n");
            return 0;

        default:
            printf("Opção inválida. Tente novamente.\n");
        }
    }
    return 0;
}
