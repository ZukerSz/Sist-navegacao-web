#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Noh{ 
    char url_Site[10000];
    struct Noh *prev;
    struct Noh *next;
} Noh;

typedef struct{ 
    Noh *head;
    Noh *tail;
    Noh *current;
} Historico;

typedef struct Noh_Favorito{ 
    char url_Site[10000];
    struct Noh_Favorito *next;
} Noh_Favorito;

typedef struct{ 
    Noh_Favorito *front;
    Noh_Favorito *rear;
} Fila_Favorita;

Noh *cria_Noh(const char *url_Site){ 
    Noh *novo_Noh = (Noh *)malloc(sizeof(Noh));
    if (novo_Noh){
        strncpy(novo_Noh->url_Site, url_Site, sizeof(novo_Noh->url_Site) - 1);
        novo_Noh->url_Site[sizeof(novo_Noh->url_Site) - 1] = '\0';
        novo_Noh->prev = NULL;
        novo_Noh->next = NULL;
    }
    return novo_Noh;
}

void inicia_Historico(Historico *historico){ 
    historico->head = NULL;
    historico->tail = NULL;
    historico->current = NULL;
}

void insira_Nova_pagina(Historico *historico, const char *url_Site){ 
    Noh *novo_Noh = cria_Noh(url_Site);

    if (!novo_Noh){
        printf("\nErro ao alocar memoria!\n");
        return;
    }

    if (historico->current){ 
        Noh *temp = historico->current->next;
        while (temp){
            Noh *toDelete = temp;
            temp = temp->next;
            free(toDelete);
        }
        historico->current->next = NULL;
    }

    if (!historico->head){ 
        historico->head = novo_Noh;
        historico->tail = novo_Noh;
    }
    else{
        historico->tail->next = novo_Noh;
        novo_Noh->prev = historico->tail;
        historico->tail = novo_Noh;
    }

    historico->current = novo_Noh; 

    printf("\nPagina (%s) adicionada com sucesso ao historico.\n", url_Site);
}

void navega_Tras_historico(Historico *historico){ 
    if (historico->current && historico->current->prev){
        historico->current = historico->current->prev;
        printf("\nPagina atual: (%s)\n", historico->current->url_Site);
    }
    else{
        printf("\nNao eh possivel voltar, inicio do historico alcancado.\n");
    }
}

void navega_Frente_historico(Historico *historico){ 
    if (historico->current && historico->current->next){
        historico->current = historico->current->next;
        printf("\nPagina atual: (%s)\n", historico->current->url_Site);
    }
    else{
        printf("\nNao eh possivel avancar, fim do historico alcancado.\n");
    }
}

void remove_Pagina(Historico *historico, char *url_Site) { 
    Noh *temp = historico->head;
    Noh *prev = NULL;

    if (temp == NULL) {
        printf("\nO historico esta vazio.\n");
        return;
    }

    if (temp != NULL && strcmp(temp->url_Site, url_Site) == 0) {
        historico->head = temp->next; 
        free(temp); 
        printf("\nPagina (%s) removida do historico.\n", url_Site);
        return;
    }

    while (temp != NULL && strcmp(temp->url_Site, url_Site) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("\nPagina (%s) nao encontrada no historico.\n", url_Site);
        return;
    }

    prev->next = temp->next;
    free(temp);  
    printf("\nPagina (%s) removida do historico.\n", url_Site);
}

void limpa_Historico(Historico *historico){ 
    Noh *temp = historico->head;

    while (temp){
        Noh *toDelete = temp;
        temp = temp->next;
        free(toDelete);
    }

    historico->head = NULL;
    historico->tail = NULL;
    historico->current = NULL;
    printf("\nHistorico limpo.\n");
}

void mostra_Historico(const Historico *historico){ 
    Noh *temp = historico->head;

    if (!temp){
        printf("\nO historico esta vazio.\n");
        return;
    }

    printf("\n----- Historico de Navegacao -----\n\n");
    while (temp){
        if (temp == historico->current){
            printf("-> %s : URL (atual)\n", temp->url_Site);
        }
        else{
            printf("   %s\n", temp->url_Site);
        }
        temp = temp->next;
    }
}

void inicia_Fila(Fila_Favorita *fila){ 
    fila->front = NULL;
    fila->rear = NULL;
}

void enfileira_adiciona_Fila(Fila_Favorita *fila, const char *url_Site){ // enfileirar (adicionar) um site aos favoritos
    Noh_Favorito *novo_Noh = (Noh_Favorito *)malloc(sizeof(Noh_Favorito));
    if (!novo_Noh){
        printf("Erro ao alocar memoria!\n");
        return;
    }
    strncpy(novo_Noh->url_Site, url_Site, sizeof(novo_Noh->url_Site) - 1);
    novo_Noh->url_Site[sizeof(novo_Noh->url_Site) - 1] = '\0';
    novo_Noh->next = NULL;

    if (!fila->rear){
        fila->front = novo_Noh;
        fila->rear = novo_Noh;
    }
    else{
        fila->rear->next = novo_Noh;
        fila->rear = novo_Noh;
    }
    printf("\nSite (%s) adicionado aos favoritos.\n", url_Site);
}

void desenfileira_acessa_Fila(Fila_Favorita *fila){ // desenfileirar (acessar) o proximo site favorito
    if (!fila->front){
        printf("\nA fila de favoritos esta vazia.\n");
        return;
    }
    Noh_Favorito *temp = fila->front;
    printf("\nAcessando o site favorito:(%s)\n", temp->url_Site);
    fila->front = fila->front->next;

    if (!fila->front){
        fila->rear = NULL;
    }

    free(temp);
}

void exibe_Fila(const Fila_Favorita *fila){ 
    if (!fila->front){
        printf("\nA fila de favoritos esta vazia.\n");
        return;
    }

    Noh_Favorito *temp = fila->front;
    printf("\n----- Favoritos na ordem de prioridade -----\n\n");
    while (temp){
        printf("-> %s\n", temp->url_Site);
        temp = temp->next;
    }
}

void limpa_Fila(Fila_Favorita *fila){ 
    Noh_Favorito *temp = fila->front;
    while (temp){
        Noh_Favorito *toDelete = temp;
        temp = temp->next;
        free(toDelete);
    }
    fila->front = NULL;
    fila->rear = NULL;
    printf("\nTodos os favoritos foram removidos.\n");
}

void listar_Historico(const Historico *historico){
    Noh *temp = historico->head;
    int index = 1;

    if (!temp){
        printf("\nO historico esta vazio.\n");
        return;
    }

    printf("\nSelecione uma pagina pelo indice:\n\n");
    while (temp){
        printf("[%d] %s\n", index, temp->url_Site);
        temp = temp->next;
        index++;
    }
}

void limpa_Tela(){
#ifdef _WIN32
    system("cls"); 
#else
    system("clear"); 
#endif
}

void pergunta_Continua_s_n(){
    char escolha;
    do{
        printf("\nDeseja continuar? (s/n): ");
        scanf(" %c", &escolha);
        if (escolha == 'n' || escolha == 'N'){
            printf("\nSaindo do programa...\n");
            exit(0); 
        }
    } while (escolha != 's' && escolha != 'S'); 
}

void medir_tempo_execucao(void (*func)(void *), void *arg, const char *nome_funcao) { 
    clock_t inicio, fim;
    double tempo_execucao;

    inicio = clock(); 
    func(arg);        
    fim = clock();    

    tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC; 
    printf("\nTempo de execucao de %s: %.6f segundos\n", nome_funcao, tempo_execucao);
}

int main() { 
    Historico historico;
    inicia_Historico(&historico);

    Fila_Favorita favoritos;
    inicia_Fila(&favoritos);

    int escolha;
    char url_Site[100];
    int indice_Remove;

    do {
        printf("\n!!!Para interagir com o menu, coloque os numeros \ncorrespondentes à acao desejada!!!\n");
        printf("\n----- Historico de Navegacao -----\n");
        printf("1. Inserir uma nova pagina\n");
        printf("2. Navegar para tras\n");
        printf("3. Navegar para frente\n");
        printf("4. Remover uma pagina especifica\n");
        printf("5. Limpar todo o historico\n");
        printf("6. Imprimir o historico\n");
        printf("\n----- Sistema de Favoritos -----\n");
        printf("7. Adicionar um site aos favoritos\n");
        printf("8. Acessar o proximo site favorito\n");
        printf("9. Exibir a lista completa de favoritos\n");
        printf("10. Limpar todos os favoritos\n");
        printf("0. Sair\n");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &escolha);
        switch (escolha) {
        case 1:
            printf("\nDigite a URL da nova pagina: ");
            scanf("%s", url_Site);
        
            clock_t inicio = clock();
            insira_Nova_pagina(&historico, url_Site);
            clock_t fim = clock(); 
        
            double tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC; 
            printf("\nTempo de execucao de insira_Nova_pagina: %.6f segundos\n", tempo_execucao);
        
            pergunta_Continua_s_n();
            limpa_Tela();
            break;
        case 2:
            inicio = clock(); 
            navega_Tras_historico(&historico); 
            fim = clock(); 
        
            tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC; 
            printf("\nTempo de execucao de navega_Tras_historico: %.6f segundos\n", tempo_execucao);
        
            pergunta_Continua_s_n();
            limpa_Tela();
            break;
        case 3:
            inicio = clock();
            navega_Frente_historico(&historico); 
            fim = clock(); 
        
            tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
            printf("\nTempo de execucao de navega_Frente_historico: %.6f segundos\n", tempo_execucao);
        
            pergunta_Continua_s_n();
            limpa_Tela();
            break;
        case 4:
            if (historico.head) {
                listar_Historico(&historico);
                printf("\nDigite o indice da pagina que deseja remover: ");
                scanf("%d", &indice_Remove);
        
                Noh *temp = historico.head;
                int indice_Atual = 1;
        
                while (temp && indice_Atual < indice_Remove) { 
                    temp = temp->next;
                    indice_Atual++;
                }
        
                if (temp) {
                    clock_t inicio = clock();
                    remove_Pagina(&historico, temp->url_Site);
                    clock_t fim = clock();
                    double tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
                    printf("\nTempo de execucao de remove_Pagina: %.6f segundos\n", tempo_execucao);
                } else {
                    printf("Indice invalido. Tente novamente.\n");
                }
            } else {
                printf("\nO historico esta vazio. Nao ha paginas para remover.\n");
            }
            pergunta_Continua_s_n();
            limpa_Tela();
            break;
        case 5:
            inicio = clock(); 
            limpa_Historico(&historico); 
            fim = clock();
        
            tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
            printf("\nTempo de execucao de limpa_Historico: %.6f segundos\n", tempo_execucao);
        
            pergunta_Continua_s_n();
            limpa_Tela();
            break;
        case 6:
            inicio = clock();
            mostra_Historico(&historico); 
            fim = clock();
        
            tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
            printf("\nTempo de execucao de mostra_Historico: %.6f segundos\n", tempo_execucao);
        
            pergunta_Continua_s_n();
            limpa_Tela();
            break;
        case 7:
            if (historico.head) {
                listar_Historico(&historico);
                int indice_Selecionado;
                printf("\nDigite o indice da pagina que deseja adicionar aos favoritos: ");
                scanf("%d", &indice_Selecionado);
        
                Noh *temp = historico.head;
                int indice_Atual = 1;
                while (temp && indice_Atual < indice_Selecionado) {
                    temp = temp->next;
                    indice_Atual++;
                }
        
                if (temp) {
                    inicio = clock();
                    enfileira_adiciona_Fila(&favoritos, temp->url_Site);
                    fim = clock();
        
                    tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
                    printf("\nTempo de execucao de enfileira_adiciona_Fila: %.6f segundos\n", tempo_execucao);
                }
                else {
                    printf("\nIndice invalido. Tente novamente.\n");
                }
            }
            else {
                printf("\nO historico esta vazio. Nao ha paginas para adicionar aos favoritos.\n");
            }
            pergunta_Continua_s_n();
            limpa_Tela();
            break;
        case 8:
            inicio = clock();
            desenfileira_acessa_Fila(&favoritos);
            fim = clock();
        
            tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;  // Calcula o tempo em segundos
            printf("\nTempo de execucao de desenfileira_acessa_Fila: %.6f segundos\n", tempo_execucao);
        
            pergunta_Continua_s_n();
            limpa_Tela();
            break;
        case 9:
            inicio = clock(); 
            exibe_Fila(&favoritos);  
            fim = clock();  
        
            tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC; 
            printf("\nTempo de execucao de exibe_Fila: %.6f segundos\n", tempo_execucao);
        
            pergunta_Continua_s_n();
            limpa_Tela();
            break;
        case 10:
            if (!favoritos.front) {
                printf("\nA fila de favoritos esta vazia. Nao ha nada para limpar.\n");
            }
            else {
                inicio = clock(); 
                limpa_Fila(&favoritos);  
                fim = clock();  
        
                tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
                printf("\nTempo de execucao de limpa_Fila: %.6f segundos\n", tempo_execucao);
            }
            pergunta_Continua_s_n();
            limpa_Tela();
            break;
                }
            } while (escolha != 0);

    printf("\nSaindo do programa...\n");
    return 0;
}