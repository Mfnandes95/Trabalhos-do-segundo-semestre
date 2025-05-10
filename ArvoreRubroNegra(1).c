#define RUBRO_NEGRA_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define PRETO 0
#define VERMELHO 1

typedef struct NoRN {
    int chave;
    int cor;
    struct NoRN *esq;
    struct NoRN *dir;
    struct NoRN *pai;
} NoRN;

NoRN* criarNo(int chave, NoRN* pai) {
    NoRN* novo = (NoRN*)malloc(sizeof(NoRN));
    if (!novo) return NULL;

    novo->chave = chave;
    novo->cor = VERMELHO;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->pai = pai;

    printf("DEBUG: Criado nó %d\n", chave);
    return novo;
}

void mostrarArvore(NoRN* no, int nivel) {
    if (!no) {
        for (int i = 0; i < nivel; i++) printf("    ");
        printf("NULL\n");
        return;
    }

    mostrarArvore(no->dir, nivel + 1);

    for (int i = 0; i < nivel; i++) printf("    ");
    printf("%d (%s)\n", no->chave, no->cor == VERMELHO ? "V" : "P");

    mostrarArvore(no->esq, nivel + 1);
}

void rotacaoEsquerda(NoRN **raiz, NoRN *x) {
    printf("DEBUG: Rotacionando à esquerda em %d\n", x->chave);

    NoRN *y = x->dir;
    x->dir = y->esq;

    if (y->esq != NULL)
        y->esq->pai = x;

    y->pai = x->pai;

    if (x->pai == NULL)
        *raiz = y;
    else if (x == x->pai->esq)
        x->pai->esq = y;
    else
        x->pai->dir = y;

    y->esq = x;
    x->pai = y;
}

void rotacaoDireita(NoRN **raiz, NoRN *y) {
    printf("DEBUG: Rotacionando à direita em %d\n", y->chave);

    NoRN *x = y->esq;
    y->esq = x->dir;

    if (x->dir != NULL)
        x->dir->pai = y;

    x->pai = y->pai;

    if (y->pai == NULL)
        *raiz = x;
    else if (y == y->pai->dir)
        y->pai->dir = x;
    else
        y->pai->esq = x;

    x->dir = y;
    y->pai = x;
}

void balancearInsercao(NoRN **raiz, NoRN *no) {
    while (no != *raiz && no->pai->cor == VERMELHO) {
        NoRN *avo = no->pai->pai;

        if (no->pai == avo->esq) {
            NoRN *tio = avo->dir;
            if (tio != NULL && tio->cor == VERMELHO) {
                no->pai->cor = PRETO;
                tio->cor = PRETO;
                avo->cor = VERMELHO;
                no = avo;
            } else {
                if (no == no->pai->dir) {
                    no = no->pai;
                    rotacaoEsquerda(raiz, no);
                }
                no->pai->cor = PRETO;
                avo->cor = VERMELHO;
                rotacaoDireita(raiz, avo);
            }
        } else {
            NoRN *tio = avo->esq;
            if (tio != NULL && tio->cor == VERMELHO) {
                no->pai->cor = PRETO;
                tio->cor = PRETO;
                avo->cor = VERMELHO;
                no = avo;
            } else {
                if (no == no->pai->esq) {
                    no = no->pai;
                    rotacaoDireita(raiz, no);
                }
                no->pai->cor = PRETO;
                avo->cor = VERMELHO;
                rotacaoEsquerda(raiz, avo);
            }
        }
    }
    (*raiz)->cor = PRETO;
}

void inserirRN(NoRN **raiz, int chave) {
    printf("\nDEBUG: Inserindo chave %d\n", chave);

    NoRN **atual = raiz;
    NoRN *pai = NULL;

    while (*atual != NULL) {
        pai = *atual;
        if (chave < (*atual)->chave) {
            atual = &((*atual)->esq);
        } else if (chave > (*atual)->chave) {
            atual = &((*atual)->dir);
        } else {
            printf("DEBUG: Chave %d já existe\n", chave);
            return;
        }
    }

    NoRN *novo = criarNo(chave, pai);
    if (!novo) {
        fprintf(stderr, "Erro: falha na alocação do nó %d\n", chave);
        return;
    }
    *atual = novo;

    balancearInsercao(raiz, novo);
    mostrarArvore(*raiz, 0);
}

/* ========== FUNÇÕES DE REMOÇÃO ========== */

NoRN* encontrarMinimo(NoRN* no) {
    while (no->esq != NULL) {
        no = no->esq;
    }
    return no;
}

void transplantar(NoRN **raiz, NoRN *u, NoRN *v) {
    if (u->pai == NULL) {
        *raiz = v;
    } else if (u == u->pai->esq) {
        u->pai->esq = v;
    } else {
        u->pai->dir = v;
    }
    
    if (v != NULL) {
        v->pai = u->pai;
    }
}

void balancearRemocao(NoRN **raiz, NoRN *x) {
    while (x != *raiz && (x == NULL || x->cor == PRETO)) {
        if (x == x->pai->esq) {
            NoRN *irmao = x->pai->dir;
            
            if (irmao->cor == VERMELHO) {
                irmao->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoEsquerda(raiz, x->pai);
                irmao = x->pai->dir;
            }
            
            if ((irmao->esq == NULL || irmao->esq->cor == PRETO) &&
                (irmao->dir == NULL || irmao->dir->cor == PRETO)) {
                irmao->cor = VERMELHO;
                x = x->pai;
            } else {
                if (irmao->dir == NULL || irmao->dir->cor == PRETO) {
                    if (irmao->esq != NULL) {
                        irmao->esq->cor = PRETO;
                    }
                    irmao->cor = VERMELHO;
                    rotacaoDireita(raiz, irmao);
                    irmao = x->pai->dir;
                }
                
                irmao->cor = x->pai->cor;
                x->pai->cor = PRETO;
                if (irmao->dir != NULL) {
                    irmao->dir->cor = PRETO;
                }
                rotacaoEsquerda(raiz, x->pai);
                x = *raiz;
            }
        } else {
            NoRN *irmao = x->pai->esq;
            
            if (irmao->cor == VERMELHO) {
                irmao->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoDireita(raiz, x->pai);
                irmao = x->pai->esq;
            }
            
            if ((irmao->dir == NULL || irmao->dir->cor == PRETO) &&
                (irmao->esq == NULL || irmao->esq->cor == PRETO)) {
                irmao->cor = VERMELHO;
                x = x->pai;
            } else {
                if (irmao->esq == NULL || irmao->esq->cor == PRETO) {
                    if (irmao->dir != NULL) {
                        irmao->dir->cor = PRETO;
                    }
                    irmao->cor = VERMELHO;
                    rotacaoEsquerda(raiz, irmao);
                    irmao = x->pai->esq;
                }
                
                irmao->cor = x->pai->cor;
                x->pai->cor = PRETO;
                if (irmao->esq != NULL) {
                    irmao->esq->cor = PRETO;
                }
                rotacaoDireita(raiz, x->pai);
                x = *raiz;
            }
        }
    }
    
    if (x != NULL) {
        x->cor = PRETO;
    }
}

int removerRN(NoRN **raiz, int chave) {
    printf("\nDEBUG: Removendo chave %d\n", chave);
    
    NoRN *z = *raiz;
    while (z != NULL) {
        if (chave < z->chave) {
            z = z->esq;
        } else if (chave > z->chave) {
            z = z->dir;
        } else {
            break;
        }
    }
    
    if (z == NULL) {
        printf("DEBUG: Chave %d não encontrada\n", chave);
        return 0;
    }
    
    NoRN *y = z;
    NoRN *x;
    int corOriginalY = y->cor;
    
    if (z->esq == NULL) {
        x = z->dir;
        transplantar(raiz, z, z->dir);
    } else if (z->dir == NULL) {
        x = z->esq;
        transplantar(raiz, z, z->esq);
    } else {
        y = encontrarMinimo(z->dir);
        corOriginalY = y->cor;
        x = y->dir;
        
        if (y->pai == z) {
            if (x != NULL) {
                x->pai = y;
            }
        } else {
            transplantar(raiz, y, y->dir);
            y->dir = z->dir;
            y->dir->pai = y;
        }
        
        transplantar(raiz, z, y);
        y->esq = z->esq;
        y->esq->pai = y;
        y->cor = z->cor;
    }
    
    free(z);
    
    if (corOriginalY == PRETO) {
        balancearRemocao(raiz, x);
    }
    
    printf("DEBUG: Chave %d removida com sucesso\n", chave);
    mostrarArvore(*raiz, 0);
    return 1;
}

void menu(NoRN **raiz) {
    int opcao, valor;

    while (1) {
        printf("\nMENU:\n");
        printf("1. Inserir\n");
        printf("2. Remover\n");
        printf("3. Mostrar Árvore\n");
        printf("4. Sair\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) {
            fprintf(stderr, "Entrada inválida!\n");
            int c; while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        switch(opcao) {
            case 1:
                printf("Valor: ");
                if (scanf("%d", &valor) != 1) {
                    fprintf(stderr, "Valor inválido!\n");
                    int c; while ((c = getchar()) != '\n' && c != EOF);
                    break;
                }
                inserirRN(raiz, valor);
                break;
            case 2:
                printf("Valor a remover: ");
                if (scanf("%d", &valor) != 1) {
                    fprintf(stderr, "Valor inválido!\n");
                    int c; while ((c = getchar()) != '\n' && c != EOF);
                    break;
                }
                removerRN(raiz, valor);
                break;
            case 3:
                printf("\nESTRUTURA DA ÁRVORE:\n");
                mostrarArvore(*raiz, 0);
                break;
            case 4:
                exit(0);
            default:
                printf("Opção inválida!\n");
        }
    }
}

int main() {
    NoRN *raiz = NULL;
    printf("DEBUG: Programa iniciado\n");

    menu(&raiz);
    return 0;
}