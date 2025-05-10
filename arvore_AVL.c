//CODIGO DA ARVORE AVL
#include <stdio.h>
#include <stdlib.h>

//criar nó para compor a arvore

typedef struct no{
    int valor;
    struct no *esquerda;//ponteiro para a sub-arvore a esquerda
    struct no *direita;//ponteiro para a sub-arvore a direita
    int altura;//registrar altura do nó altura do nó
}No;

//procedimento para inserir um elemento na arvore (Criar novo nó)

No *novoNo(int valor){//função para criar um novo nó, e toda a vez que eu insiro um nó ele é um nó folha
    No *novo_no = malloc(sizeof(No));//alocar memória para o novo nó

    if(novo_no){
        novo_no->valor = valor;//atribuir o valor ao novo nó da arvore
        novo_no->esquerda = NULL;//o nó a esquerda da arvore é NULL
        novo_no->direita = NULL;//o nó a direita da arvore é NULL
        novo_no->altura = 0;//altura do nó é 0, pois ela é a folha da arvore
    }
    else{
        printf("Erro de alocação de memória\n");
    }
    return novo_no;
}

//procedimento para verificar e retornar o lado maior da arvore

int maior(int a, int b){//função para retornar o maior valor entre dois valores
    return (a > b) ? a : b;//se a for maior que b, retorna a, senão retorna b
}//dado a alturas da sub-arvores, retorna a maior

//procedimento para retornar a altura da arvore

int alturaDoNo(No *no){//função para retornar a altura da arvore
    if(no == NULL){
        return -1;//se a no for NULL, a altura é -1
    }
    else{
        return no->altura;//retorna a altura da no
    }
}

//procedimento para calcular e retornar o fator de balanceamento da arvore

int fatorDeBalanceamento(No *no){//função para calcular e retornar o fator de balanceamento da arvore
    if(no){
        return (alturaDoNo(no->esquerda) - alturaDoNo(no->direita));//retorna o valor absoluto da diferença entre a altura da sub-arvore a esquerda e a altura da sub-arvore a direita
    }
    else{
        return 0;//se o no for NULL, o fator de balanceamento é 0
    }
}//dado um nó, retorna a diferença entre as alturas das sub-arvores

//procedimento para rotacionar a arvore para a esquerda

No *rotacaoEsquerda(No *no){//função para rotacionar a arvore para a esquerda
    No *aux_x, *aux_y;//criar dois ponteiros auxiliares

    aux_x = no->direita;//o auxiliar x recebe a sub-arvore a direita do nó
    aux_y = aux_x->esquerda;//o auxiliar y recebe a sub-arvore a esquerda do auxiliar x, pois o auxiliar x pode ter uma sub-arvore a esquerda

    aux_x->esquerda = no;//a sub-arvore a esquerda do auxiliar x recebe o nó
    no->direita = aux_y;//a sub-arvore a direita do nó recebe o auxiliar y

    no->altura = maior(alturaDoNo(no->esquerda), alturaDoNo(no->direita)) + 1;//a altura do nó é o maior valor entre a altura da sub-arvore a esquerda e a altura da sub-arvore a direita + 1
    aux_x->altura = maior(alturaDoNo(aux_x->esquerda), alturaDoNo(aux_x->direita)) + 1;//a altura do auxiliar x é o maior valor entre a altura da sub-arvore a esquerda e a altura da sub-arvore a direita + 1
    
    return aux_x;//retorna o auxiliar alterado
}//dado um nó, rotaciona a arvore para a esquerda

//procedimento para rotacionar a arvore para a direita

No *rotacaoDireita(No *no){//é a mesma coisa que o rotação esquerda, só que ao contrário
    No *aux_x, *aux_y;//criar dois ponteiros auxiliares

    aux_x = no->esquerda;//o auxiliar x recebe a sub-arvore a esquerda do nó
    aux_y = aux_x->direita;//o auxiliar y recebe a sub-arvore a direita do auxiliar x, pois o auxiliar x pode ter uma sub-arvore a direita

    aux_x->direita = no;//a sub-arvore a direita do auxiliar x recebe o nó
    no->esquerda = aux_y;//a sub-arvore a esquerda do nó recebe o auxiliar y

    no->altura = maior(alturaDoNo(no->esquerda), alturaDoNo(no->direita)) + 1;//a altura do nó é o maior valor entre a altura da sub-arvore a esquerda e a altura da sub-arvore a direita + 1
    aux_x->altura = maior(alturaDoNo(aux_x->esquerda), alturaDoNo(aux_x->direita)) + 1;//a altura do auxiliar x é o maior valor entre a altura da sub-arvore a esquerda e a altura da sub-arvore a direita + 1

    return aux_x;//retorna o auxiliar alterado
}

//procedimento para rotacionar a arvore para a direita e depois para a esquerda

No *rotacaoDireitaEsquerda(No *no){
    no->direita = rotacaoDireita(no->direita);//rotaciona a arvore para a direita
    return rotacaoEsquerda(no);//rotaciona a arvore para a esquerda
}//dado um nó, rotaciona a arvore para a direita e depois para a esquerda

//procedimento para rotacionar a arvore para a esquerda e depois para a direita

No *rotacaoEsquerdaDireita(No *no){
    no->esquerda = rotacaoEsquerda(no->esquerda);//rotaciona a arvore para a esquerda
    return rotacaoDireita(no);//rotaciona a arvore para a direita
}//dado um nó, rotaciona a arvore para a esquerda e depois para a direita

//procedimento para balancear a arvore

No *balancear(No *no){//função para balancear a arvore
    int fB = fatorDeBalanceamento(no);//calcula o fator de balanceamento da arvore

    //chama procedimento de rotação a esquerda
    if(fB < -1 && fatorDeBalanceamento(no->direita) <= 0){//se o fator de balanceamento for menor que -1 e o fator de balanceamento da sub-arvore a direita for menor ou igual a 0
        return rotacaoEsquerda(no);//chama a função de rotação a esquerda
    }

    //chama procedimento de rotação a direita
    else if(fB > 1 && fatorDeBalanceamento(no->esquerda) >= 0){//se o fator de balanceamento for maior que 1 e o fator de balanceamento da sub-arvore a esquerda for maior ou igual a 0
        return rotacaoDireita(no);//chama a função de rotação a direita
    }

    //chama procedimento de rotação a direita e depois a esquerda
    else if(fB > 1 && fatorDeBalanceamento(no->esquerda) < 0){//se o fator de balanceamento for maior que 1 e o fator de balanceamento da sub-arvore a esquerda for menor que 0
        no->esquerda = rotacaoEsquerda(no->esquerda);//chama a função de rotação a esquerda
        return rotacaoEsquerdaDireita(no);//chama a função de rotação a esquerda e depois a direita
    }

    //chama procedimento de rotação a esquerda e depois a direita
    else if(fB < -1 && fatorDeBalanceamento(no->direita) > 0){//se o fator de balanceamento for menor que -1 e o fator de balanceamento da sub-arvore a direita for maior que 0
        no->direita = rotacaoDireita(no->direita);//chama a função de rotação a direita
        return rotacaoDireitaEsquerda(no);//chama a função de rotação a direita e depois a esquerda
    }
    return no;//retorna o nó balanceado
}

//procedimento para inserir um elemento na arvore

No *inserir(No *no, int x){//função para inserir um elemento na arvore
    if(no == NULL){//se o nó for NULL, cria um novo nó, pois a arvore está vazia naquele ponto
        return novoNo(x);
    }
    else{
        if(x < no->valor){//se o valor for menor que o valor do nó
            no->esquerda = inserir(no->esquerda, x);//insere o valor na sub-arvore a esquerda
            //a recursividade vai inserir o valor na sub-arvore a esquerda até encontrar um nó folha
            //quando encontrar o nó folha, a recursividade vai retornar o nó folha e começar a fazer as rotações
        }
        else if(x > no->valor){//se o valor for maior que o valor do nó
            no->direita = inserir(no->direita, x);//insere o valor na sub-arvore a direita
            //a recursividade vai inserir o valor na sub-arvore a direita até encontrar um nó folha
            //quando encontrar o nó folha, a recursividade vai retornar o nó folha e começar a fazer as rotações
        }
        else{
            printf("O valor %d já existe na arvore\n", x);//se o valor já existir na arvore, imprime que o valor já existe, pois não pode haver valores repetidos
        }
        //calcular a altura do nó
        no->altura = maior(alturaDoNo(no->esquerda), alturaDoNo(no->direita)) + 1;//a altura do nó é o maior valor entre a altura da sub-arvore a esquerda e a altura da sub-arvore a direita + 1

        //verificar se a arvore está desbalanceada
        no = balancear(no);//se a arvore estiver desbalanceada, chama a função balancear para balancear a arvore
        //o balancear é uma função que verifica se a arvore está desbalanceada e chama as funções de rotação para balancear a arvore

        return no;//retorna o nó        
    }
}

//procedimento para remover um elemento da arvore

No *remover(No *no, int x){
    if(no == NULL){
        printf("O valor %d não existe na arvore\n", x);
        return NULL;
    }
    else{//procurar o nó para remover
        if(no->valor == x){//se o valor do nó for igual ao valor a ser removido, se não for, a recursividade vai procurar o valor a ser removido, caminha na arvore
            
            //remover os nós folhas, ou seja, os nós que não tem filhos
            if(no->esquerda == NULL && no->direita == NULL){//se o nó for folha
                free(no);//libera o nó
                printf("O valor %d foi removido da arvore\n", x);
                return NULL;
            }
            
            else{
                //remover nós que possuem 2 filhos
                if(no->esquerda != NULL && no->direita != NULL){
                    No *aux = no->esquerda;//cria um auxiliar para o nó a esquerda
                    while(aux->direita != NULL){//enquanto o nó a direita do auxiliar for diferente de NULL
                        aux = aux->direita;//o auxiliar recebe o nó a direita do auxiliar
                    }
                    no->valor = aux->valor;//o valor do nó é o valor do auxiliar
                    aux->valor = x;//o valor do auxiliar é o valor a ser removido
                    printf("O valor %d foi removido da arvore\n", x); // Corrigido de 'prinft' para 'printf'
                    no->esquerda = remover(no->esquerda, x);
                    return no;
                }
                else{
                    //remover no com 1 filho
                    No *aux;
                    if(no->esquerda != NULL){
                        aux = no->esquerda;
                    }
                    else{
                        aux = no->direita;
                    }
                    free(no);
                    printf("O valor %d foi removido da arvore\n", x);
                    return aux;
                }
            }
        }
        else{
            if(x < no->valor){//se o valor for menor que o valor do nó, então o valor a ser removido está na sub-arvore a esquerda
                no->esquerda = remover(no->esquerda, x);//para caminnhar na arvore, a recursividade vai chamar a função remover para a sub-arvore a esquerda
            }
            else{//se o valor for maior que o valor do nó, então o valor a ser removido está na sub-arvore a direita
                no->direita = remover(no->direita, x);//para caminnhar na arvore, a recursividade vai chamar a função remover para a sub-arvore a direita
            }

            //apos remover o nó, é necessário recalcular a altura do nó e fazer a verificação de balanceamento

            //calcular a altura do nó
            no->altura = maior(alturaDoNo(no->esquerda), alturaDoNo(no->direita)) + 1;//a altura do nó é o maior valor entre a altura da sub-arvore a esquerda e a altura da sub-arvore a direita + 1

            //verificar se a arvore está desbalanceada
            no = balancear(no);//se a arvore estiver desbalanceada, chama a função balancear para balancear a arvore

            return no;//retorna o nó
        }
    }
}

//procedimento para imprimir a arvore (raiz a esquerda e folhas a direita)

void imprimirArvore(No *no, int nivel){
    if(no != NULL){
        imprimirArvore(no->direita, nivel + 1);//volta a recursividade para a sub-arvore a direita, até encontrar um nó folha, pois o nó folha é o nó mais a direita da arvore, e quebra o if e começa a imprimir a arvore
        printf("\n\n");//pula duas linhas
        
        for(int i = 0; i < nivel; i++){
            printf("   ");//imprime os espaços para formatar a arvore
        }

        printf("%d", no->valor);//imprime o valor do nó
        imprimirArvore(no->esquerda, nivel + 1);//imprime a sub-arvore a esquerda
    }
}

//trabalhando no main

int main(){

    int opcao, valor;
    No *raiz = NULL;//criar a raiz da arvore

    do{
        printf("\n\n1 - Inserir\n2 - Remover\n3 - Imprimir\n4 - Sair\n\n");
        printf("Digite a opcao desejada: ");
        scanf("%d", &opcao);

        switch(opcao){
            case 1:
                printf("Digite o valor a ser inserido: ");
                scanf("%d", &valor);
                raiz = inserir(raiz, valor);
                break;
            case 2:
                printf("Digite o valor a ser removido: ");
                scanf("%d", &valor);
                raiz = remover(raiz, valor);
                break;
            case 3:
                imprimirArvore(raiz, 0);
                break;
            case 4:
                printf("Saindo do programa\n");
                break;
            default:
                printf("Opcao invalida\n");
                break;
        }
    }while(opcao != 4);
    return 0;
}
