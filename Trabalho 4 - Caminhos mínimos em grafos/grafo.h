#ifndef GRAFO_H
#define GRAFO_H

typedef struct grafo Grafo;

typedef struct no No;

Grafo *cria_grafo(int nro_vertices);

void insere_vertice(Grafo *grafo, char *vertice);

void insere_aresta(Grafo *grafo, char *vertice1, char *vertice2, float custo);

float menor_caminho(Grafo *grafo, char *vertice_origem, char *vertice_destino, int nro_vertices);

void leitura_dados(Grafo *grafo, int nro_vertices);


#endif // GRAFO_H