#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// Define uma constante para representar infinito (não há aresta entre dois vértices)
#define INFINITO 9999999

struct no
{
    char nome[1000];
    float custo;
    struct no *proximo; // Ponteiro para o próximo nó na lista de adjacência
};

struct grafo
{
    struct no **lista_adjacencia; // Ponteiro para um array de ponteiros para nós (lista de adjacência)
};

typedef struct VerticeInfo
{
    char nome[1000];
    float distancia;
    bool visitado;
    int indice_anterior; // Armazena o índice do vértice anterior no menor caminho

} verticeBusca;

Grafo *cria_grafo(int nro_vertices)
{
    // Aloca memória para a estrutura do grafo
    Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));

    // Aloca memória para o array de ponteiros para nós (lista de adjacência)
    grafo->lista_adjacencia = (struct no **)malloc(nro_vertices * sizeof(struct no *));

    int i;
    for (i = 0; i < nro_vertices; i++)
    {
        grafo->lista_adjacencia[i] = NULL; // Inicializa todos os elementos do array como NULL
    }
    return grafo; // Retorna o grafo criado
}

void insere_vertice(Grafo *grafo, char *vertice)
{
    if (grafo == NULL)
    {
        printf("Grafo nao existe\n");
        return;
    }

    // Aloca memória para o novo nó a ser inserido
    struct no *novo_no = (struct no *)malloc(sizeof(struct no));
    strcpy(novo_no->nome, vertice); // Define o nome do vértice no novo nó
    novo_no->proximo = NULL;        // Define o próximo como NULL, pois é o último nó da lista de adjacência
    novo_no->custo = 0;             // Inicializa o custo do nó como 0 (apenas para fins ilustrativos, pode ser atualizado em outras funções)

    int i = 0;
    // Percorre a lista de adjacência procurando se o vértice já existe
    while (grafo->lista_adjacencia[i] != NULL)
    {
        if (strcmp(grafo->lista_adjacencia[i]->nome, vertice) == 0)
        {
            printf("Vertice ja existe\n");
            return;
        }
        i++;
    }

    // Se o vértice não existe na lista de adjacência, adiciona-o no primeiro elemento vazio encontrado
    grafo->lista_adjacencia[i] = novo_no;
}

void insere_aresta(Grafo *grafo, char *vertice1, char *vertice2, float custo)
{
    if (grafo == NULL)
    {
        printf("Grafo nao existe\n");
        return;
    }

    int i = 0;
    // Procura o vértice1 na lista de adjacência para adicionar a aresta
    while (grafo->lista_adjacencia[i] != NULL)
    {
        if (strcmp(grafo->lista_adjacencia[i]->nome, vertice1) == 0)
        {
            break;
        }
        i++;
    }

    // Cria um novo nó representando a aresta e aponta-o para o próximo nó do vértice1
    struct no *novo_no = (struct no *)malloc(sizeof(struct no));
    strcpy(novo_no->nome, vertice2);                        // Define o nome do vértice2 na aresta
    novo_no->proximo = grafo->lista_adjacencia[i]->proximo; // Aponta o próximo da aresta para o próximo nó de vertice1
    novo_no->custo = custo;                                 // Define o custo associado à aresta

    grafo->lista_adjacencia[i]->proximo = novo_no; // Atualiza o próximo nó de vertice1 para a nova aresta

    i = 0;
    // Procura o vértice2 na lista de adjacência para adicionar a aresta
    while (grafo->lista_adjacencia[i] != NULL)
    {
        if (strcmp(grafo->lista_adjacencia[i]->nome, vertice2) == 0)
        {
            break;
        }
        i++;
    }

    struct no *novo_no2 = (struct no *)malloc(sizeof(struct no));
    // Cria outro novo nó representando a aresta e aponta-o para o próximo nó do vértice2
    strcpy(novo_no2->nome, vertice1);                        // Define o nome do vértice1 na aresta
    novo_no2->proximo = grafo->lista_adjacencia[i]->proximo; // Aponta o próximo da aresta para o próximo nó de vertice2
    novo_no2->custo = custo;                                 // Define o custo associado à aresta

    grafo->lista_adjacencia[i]->proximo = novo_no2; // Atualiza o próximo nó de vertice2 para a nova aresta
}

int encontra_vertice_menor_distancia(struct VerticeInfo *vertices, int nro_vertices)
{
    float menor_distancia = INFINITO;
    int indice_menor = -1;

    for (int i = 0; i < nro_vertices; i++)
    {
        if (!vertices[i].visitado && vertices[i].distancia < menor_distancia)
        {
            menor_distancia = vertices[i].distancia;
            indice_menor = i;
        }
    }

    return indice_menor;
}

float menor_caminho(Grafo *grafo, char *vertice_origem, char *vertice_destino, int nro_vertices)
{
    if (grafo == NULL)
    {
        printf("Grafo nao existe\n");
        return -1; // Retorna um valor inválido caso o grafo não exista
    }

    struct VerticeInfo *vertices = (struct VerticeInfo *)malloc(nro_vertices * sizeof(struct VerticeInfo));

    // Inicializa informações dos vértices para o algoritmo de Dijkstra
    for (int i = 0; i < nro_vertices; i++)
    {
        strcpy(vertices[i].nome, grafo->lista_adjacencia[i]->nome);
        vertices[i].distancia = INFINITO; // Inicialmente, todas as distâncias são definidas como infinito
        vertices[i].visitado = false;
        vertices[i].indice_anterior = -1; // Inicializa o índice anterior como -1 (não visitado ainda)
    }

    // Encontra o índice do vértice de origem
    int indice_origem = -1;
    for (int i = 0; i < nro_vertices; i++)
    {
        if (strcmp(vertices[i].nome, vertice_origem) == 0)
        {
            vertices[i].distancia = 0; // Define a distância da origem para ela mesma como 0
            indice_origem = i;
            break;
        }
    }

    // Executa o algoritmo de Dijkstra para encontrar o menor caminho
    for (int count = 0; count < nro_vertices - 1; count++)
    {
        int indice_atual = encontra_vertice_menor_distancia(vertices, nro_vertices);
        vertices[indice_atual].visitado = true;

        // Atualiza as distâncias dos vértices vizinhos não visitados
        struct no *no_atual = grafo->lista_adjacencia[indice_atual]->proximo;
        while (no_atual != NULL)
        {
            int indice_vizinho = -1;
            for (int i = 0; i < nro_vertices; i++)
            {
                if (strcmp(vertices[i].nome, no_atual->nome) == 0)
                {
                    indice_vizinho = i;
                    break;
                }
            }

            float distancia_aresta = no_atual->custo;
            if (!vertices[indice_vizinho].visitado && vertices[indice_atual].distancia + distancia_aresta < vertices[indice_vizinho].distancia)
            {
                vertices[indice_vizinho].distancia = vertices[indice_atual].distancia + distancia_aresta;
                vertices[indice_vizinho].indice_anterior = indice_atual; // Atualiza o vértice anterior no menor caminho
            }

            no_atual = no_atual->proximo;
        }
    }

    // Encontra o índice do vértice de destino
    int indice_destino = -1;
    for (int i = 0; i < nro_vertices; i++)
    {
        if (strcmp(vertices[i].nome, vertice_destino) == 0)
        {
            indice_destino = i;
            break;
        }
    }

    // Calcula o custo total do percurso
    float custo_total = vertices[indice_destino].distancia;

    free(vertices); // Libera a memória alocada para o vetor de vertices
    return custo_total;
}

void leitura_dados(Grafo *grafo, int nro_vertices)
{
    int quantidade_arestas, leitura = 0;
    char nomes[10000], input[10000];

    for (int i = 0; i < nro_vertices; i++)
    {
        scanf(" %[^(\r|\n)]", nomes);
        insere_vertice(grafo, nomes);
    }

    scanf("%d", &quantidade_arestas);
    for (int a = 0; a < quantidade_arestas; a++)
    {
        scanf(" %[^(\r|\n)]", input);
        char *vertice1 = strtok(input, " ");
        char *vertice2 = strtok(NULL, " ");
        float custo = atof(strtok(NULL, " "));
        insere_aresta(grafo, vertice1, vertice2, custo);
    }
}
