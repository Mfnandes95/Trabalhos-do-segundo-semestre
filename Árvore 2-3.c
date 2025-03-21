#include <stdio.h>
#include <stdlib.h>

#define MAX_CHAVES 2
#define MAX_FILHOS 3
#define MIN_CHAVES 1

typedef struct No {
    int num_chaves;
    int chaves[MAX_CHAVES];
    struct No* filhos[MAX_FILHOS];
} No;

// Função para criar um novo nó
No* criar_no() {
    No* novo_no = (No*)malloc(sizeof(No));
    novo_no->num_chaves = 0;
    for (int i = 0; i < MAX_FILHOS; i++) {
        novo_no->filhos[i] = NULL;
    }
    return novo_no;
}

// Função para inserir uma chave ordenada no nó
void inserir_chave_ordenada(No* no, int chave) {
    int i = no->num_chaves - 1;
    while (i >= 0 && chave < no->chaves[i]) {
        no->chaves[i + 1] = no->chaves[i];
        i--;
    }
    no->chaves[i + 1] = chave;
    no->num_chaves++;
}

// Função para dividir um nó
No* dividir_no(No* no, int* chave_promovida) {
    No* novo_no = criar_no();
    int meio = no->num_chaves / 2;

    // Promove a chave do meio
    *chave_promovida = no->chaves[meio];

    // Transfere as chaves e filhos para o novo nó
    novo_no->num_chaves = no->num_chaves - meio - 1;
    for (int i = 0; i < novo_no->num_chaves; i++) {
        novo_no->chaves[i] = no->chaves[meio + 1 + i];
    }
    for (int i = 0; i <= novo_no->num_chaves; i++) {
        novo_no->filhos[i] = no->filhos[meio + 1 + i];
    }

    // Atualiza o número de chaves no nó original
    no->num_chaves = meio;
    return novo_no;
}

// Função de inserção recursiva com melhoria para evitar inserções excessivas
No* inserir(No* raiz, int chave) {
    if (raiz == NULL) {
        raiz = criar_no();
        raiz->chaves[0] = chave;
        raiz->num_chaves = 1;
        return raiz;
    }

    // Caso o nó raiz esteja cheio, dividir
    if (raiz->num_chaves == MAX_CHAVES) {
        int chave_promovida;
        No* novo_no = dividir_no(raiz, &chave_promovida);

        // Se a raiz estava cheia, criar uma nova raiz e promover a chave
        No* nova_raiz = criar_no();
        nova_raiz->chaves[0] = chave_promovida;
        nova_raiz->num_chaves = 1;
        nova_raiz->filhos[0] = raiz;
        nova_raiz->filhos[1] = novo_no;

        raiz = nova_raiz;
    }

    // Encontra a posição correta para inserir a chave
    int i = raiz->num_chaves - 1;
    while (i >= 0 && chave < raiz->chaves[i]) {
        i--;
    }
    i++;

    // Se o nó filho não estiver vazio, recursivamente chama a inserção
    if (raiz->filhos[i] != NULL) {
        raiz->filhos[i] = inserir(raiz->filhos[i], chave);
    } else {
        inserir_chave_ordenada(raiz, chave);
    }

    return raiz;
}

// Função para encontrar o sucessor de uma chave
int encontrar_sucessor(No* raiz) {
    while (raiz->filhos[0] != NULL) {
        raiz = raiz->filhos[0];
    }
    return raiz->chaves[0];
}

// Função para remover uma chave
No* remover(No* raiz, int chave) {
    if (raiz == NULL) {
        return raiz;
    }

    // Encontra a posição da chave ou do filho onde a chave pode estar
    int i = 0;
    while (i < raiz->num_chaves && chave > raiz->chaves[i]) {
        i++;
    }

    // Caso 1: A chave está no nó atual
    if (i < raiz->num_chaves && chave == raiz->chaves[i]) {
        // Caso 1.1: O nó é uma folha
        if (raiz->filhos[0] == NULL) {
            for (int j = i; j < raiz->num_chaves - 1; j++) {
                raiz->chaves[j] = raiz->chaves[j + 1];
            }
            raiz->num_chaves--;
        }
        // Caso 1.2: O nó é interno
        else {
            int sucessor = encontrar_sucessor(raiz->filhos[i + 1]);
            raiz->chaves[i] = sucessor;
            raiz->filhos[i + 1] = remover(raiz->filhos[i + 1], sucessor);
        }
    }
    // Caso 2: A chave está em um filho
    else {
        raiz->filhos[i] = remover(raiz->filhos[i], chave);
    }

    // Balanceamento após a remoção
    if (raiz->filhos[i] != NULL && raiz->filhos[i]->num_chaves < MIN_CHAVES) {
        // Tentando emprestar uma chave do irmão esquerdo
        if (i > 0 && raiz->filhos[i - 1]->num_chaves > MIN_CHAVES) {
            No* filho = raiz->filhos[i];
            No* irmao = raiz->filhos[i - 1];

            // Move a chave do irmão para o nó pai
            filho->chaves[filho->num_chaves] = raiz->chaves[i - 1];
            filho->num_chaves++;

            // Move a chave do pai para o irmão
            raiz->chaves[i - 1] = irmao->chaves[irmao->num_chaves - 1];
            irmao->num_chaves--;

            // Se o irmão tiver filhos, move o filho mais à direita para o filho
            if (irmao->filhos[irmao->num_chaves] != NULL) {
                filho->filhos[filho->num_chaves] = irmao->filhos[irmao->num_chaves];
                irmao->filhos[irmao->num_chaves] = NULL;
            }
        }
        // Tentando emprestar uma chave do irmão direito
        else if (i < raiz->num_chaves && raiz->filhos[i + 1]->num_chaves > MIN_CHAVES) {
            No* filho = raiz->filhos[i];
            No* irmao = raiz->filhos[i + 1];

            // Move a chave do irmão para o nó pai
            filho->chaves[filho->num_chaves] = raiz->chaves[i];
            filho->num_chaves++;

            // Move a chave do pai para o irmão
            raiz->chaves[i] = irmao->chaves[0];
            for (int j = 0; j < irmao->num_chaves - 1; j++) {
                irmao->chaves[j] = irmao->chaves[j + 1];
            }
            irmao->num_chaves--;

            // Se o irmão tiver filhos, move o filho mais à esquerda para o filho
            if (irmao->filhos[0] != NULL) {
                filho->filhos[filho->num_chaves] = irmao->filhos[0];
                irmao->filhos[0] = NULL;
            }
        }
        // Caso não seja possível emprestar, funde os nós
        else {
            No* filho = raiz->filhos[i];
            No* irmao;

            // Fusão com o irmão esquerdo
            if (i > 0) {
                irmao = raiz->filhos[i - 1];

                // Move a chave do pai para o irmão
                irmao->chaves[irmao->num_chaves] = raiz->chaves[i - 1];
                irmao->num_chaves++;

                // Move as chaves do filho para o irmão
                for (int j = 0; j < filho->num_chaves; j++) {
                    irmao->chaves[irmao->num_chaves + j] = filho->chaves[j];
                }
                irmao->num_chaves += filho->num_chaves;

                // Move os filhos do filho para o irmão
                for (int j = 0; j <= filho->num_chaves; j++) {
                    irmao->filhos[irmao->num_chaves + j] = filho->filhos[j];
                }

                // Libera o filho
                free(filho);

                // Remove a chave do pai
                for (int j = i - 1; j < raiz->num_chaves - 1; j++) {
                    raiz->chaves[j] = raiz->chaves[j + 1];
                }
                for (int j = i; j < raiz->num_chaves; j++) {
                    raiz->filhos[j] = raiz->filhos[j + 1];
                }
                raiz->num_chaves--;
            }
            // Fusão com o irmão direito
            else {
                irmao = raiz->filhos[i + 1];

                // Move a chave do pai para o filho
                filho->chaves[filho->num_chaves] = raiz->chaves[i];
                filho->num_chaves++;

                // Move as chaves do irmão para o filho
                for (int j = 0; j < irmao->num_chaves; j++) {
                    filho->chaves[filho->num_chaves + j] = irmao->chaves[j];
                }
                filho->num_chaves += irmao->num_chaves;

                // Move os filhos do irmão para o filho
                for (int j = 0; j <= irmao->num_chaves; j++) {
                    filho->filhos[filho->num_chaves + j] = irmao->filhos[j];
                }

                // Libera o irmão
                free(irmao);

                // Remove a chave do pai
                for (int j = i; j < raiz->num_chaves - 1; j++) {
                    raiz->chaves[j] = raiz->chaves[j + 1];
                }
                for (int j = i + 1; j < raiz->num_chaves; j++) {
                    raiz->filhos[j] = raiz->filhos[j + 1];
                }
                raiz->num_chaves--;
            }
        }
    }

    // Se a raiz ficar sem chaves, substitua-a pelo único filho
    if (raiz->num_chaves == 0) {
        No* nova_raiz = raiz->filhos[0];
        free(raiz);
        raiz = nova_raiz;
    }

    return raiz;
}

// Função para imprimir a árvore
void imprimir(No* raiz, int nivel) {
    if (raiz != NULL) {
        for (int i = 0; i < nivel; i++) {
            printf("   ");
        }
        printf("[");
        for (int i = 0; i < raiz->num_chaves; i++) {
            printf("%d ", raiz->chaves[i]);
        }
        printf("]\n");

        // Recursão para imprimir os filhos
        for (int i = 0; i <= raiz->num_chaves; i++) {
            imprimir(raiz->filhos[i], nivel + 1);
        }
    }
}

// Função para liberar a memória da árvore
void liberar_arvore(No* raiz) {
    if (raiz != NULL) {
        for (int i = 0; i <= raiz->num_chaves; i++) {
            liberar_arvore(raiz->filhos[i]);
        }
        free(raiz);
    }
}

int main() {
    No* raiz = NULL;
    int escolha, chave;

    do {
        printf("\n--- Menu ---\n");
        printf("1. Inserir chave\n");
        printf("2. Remover chave\n");
        printf("3. Visualizar árvore\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                printf("Digite a chave a ser inserida: ");
                scanf("%d", &chave);
                raiz = inserir(raiz, chave);
                printf("Chave %d inserida com sucesso!\n", chave);
                break;
            case 2:
                printf("Digite a chave a ser removida: ");
                scanf("%d", &chave);
                raiz = remover(raiz, chave);
                printf("Chave %d removida com sucesso!\n", chave);
                break;
            case 3:
                printf("\nÁrvore 2-3:\n");
                imprimir(raiz, 0);
                break;
            case 4:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (escolha != 4);

    // Liberar memória antes de sair
    liberar_arvore(raiz);
    return 0;
}
