#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int quantida_vertices, quantidade_caminhos;
    char nome[1000];
    scanf("%d", &quantida_vertices);

    Grafo *grafo = cria_grafo(quantida_vertices);

    leitura_dados(grafo, quantida_vertices);

    scanf("%d", &quantidade_caminhos);

    for (int a = 0; a < quantidade_caminhos; a++)
    {
        scanf(" %[^(\r|\n)]", nome);
        float custo_percurso = menor_caminho(grafo, "Pizzaria", nome, quantida_vertices);
        float c_d = 1.50 + 0.2 * custo_percurso;
        printf("%.2f\n", c_d);
    }

    return 0;
}
