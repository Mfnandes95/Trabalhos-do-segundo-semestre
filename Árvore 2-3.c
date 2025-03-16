#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int val1, val2;
    struct Node *esquerda;
    struct Node *meio;
    struct Node *direita;
    int is_leaf;
} Node;

Node* criarNode(int val, Node* esquerda, Node* meio, Node* direita, int is_leaf) {
    Node* novoNode = (Node*)malloc(sizeof(Node));
    novoNode->val1 = val;
    novoNode->val2 = -1;
    novoNode->esquerda = esquerda;
    novoNode->meio = meio;
    novoNode->direita = direita;
    novoNode->is_leaf = is_leaf;
    return novoNode;
}

Node* insert(Node* root, int val) {
    if (root == NULL) {
        return criarNode(val, NULL, NULL, NULL, 1);
    }

    if (root->is_leaf) {
        if (root->val2 == -1) {
            if (val < root->val1) {
                root->val2 = root->val1;
                root->val1 = val;
            } else {
                root->val2 = val;
            }
        } else {
            printf("Divisão de nó 3 não implementada neste exemplo.\n");
        }
        return root;
    }

    // Nó interno
    if (val < root->val1) {
        root->esquerda = insert(root->esquerda, val);
    } else if (root->val2 == -1 || val < root->val2) {
        root->meio = insert(root->meio, val);
    } else {
        root->direita = insert(root->direita, val);
    }

    return root;
}

void printArvore(Node* root) {
    if (root == NULL) return;

    printArvore(root->esquerda);
    printf("%d ", root->val1);
    if (root->val2 != -1) {
        printArvore(root->meio);
        printf("%d ", root->val2);
    }
    printArvore(root->direita);
}

int main() {
    Node* root = NULL;

    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 5);
    root = insert(root, 15);
    root = insert(root, 25);

    printf("Arvore 2-3 em-ordem: ");
    printArvore(root);
    printf("\n");

    return 0;
}
