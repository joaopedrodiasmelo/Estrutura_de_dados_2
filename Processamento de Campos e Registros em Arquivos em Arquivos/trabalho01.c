#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Registro
{
    int id;
    char titulo[100];
    char autor[100];

} registro;

typedef struct print_registros
{ // responsável por printar os registros na ordem da saida do caso de teste
    int id;
    char titulo[100];
    char autor[100];
    long byte;

} print_registro;

// estrutua responsável por armazenar a quantidade(tamanho) do titulo/autor de cada registro e isso ira auxiliar na leitura do arquivo
typedef struct tamanhos
{
    int tamanho_autor;
    int tamanho_titulo;

} tamanhos;

int main()
{
    int qnt_regitro;
    int tamanho_autor;
    registro auxiliar;
    char delimitador_titulo = '|';
    char delimitador_registro = '0';

    // abrindo o arquivo para escrita binária
    FILE *arq = fopen("registros.bin", "wb");

    if (arq == NULL)
    {
        printf("erro ao abrir o arquivo");
        return -1;
    }

    scanf("%d", &qnt_regitro); // leitura da quantidade de registros

    tamanhos vet_reg[qnt_regitro]; // responsável por armazenar o tamanho(int) do autor e do titulo em cada registro

    for (int a = 0; a < qnt_regitro; a++)

    {
        scanf("%d", &auxiliar.id);
        scanf(" %[^(\r|\n)]", auxiliar.titulo);
        // auxiliar.titulo[strcspn(auxiliar.titulo, "\n")] = '\0'; // remove o caractere de quebra de linha e o substitui por \0
        scanf(" %[^(\r|\n)]", auxiliar.autor);
        //   auxiliar.autor[strcspn(auxiliar.autor, "\n")] = '\0'; // remove o caractere de quebra de linha e o substitui por \0

        // armazena o tamanho das strings

        vet_reg[a].tamanho_autor = strlen(auxiliar.autor);   // não considera o \0 na contagem
        vet_reg[a].tamanho_titulo = strlen(auxiliar.titulo); // não considera o \0 na contagem

        // escrita no arquivo

        fwrite(&auxiliar.id, sizeof(int), 1, arq);
        fwrite(&auxiliar.titulo, sizeof(char), strlen(auxiliar.titulo), arq);
        fwrite(&delimitador_titulo, sizeof(char), 1, arq);
        fwrite(&tamanho_autor, sizeof(int), 1, arq);
        fwrite(&auxiliar.autor, sizeof(char), strlen(auxiliar.autor), arq);
        fwrite(&delimitador_registro, sizeof(char), 1, arq);
    }

    fclose(arq);

    // leitura do arquivo

    int saida_arquivo; // recebe quantos registros devem ser recuperados do arquivo
    long byte_offset;  // representa a posição que o registro possui no arquivo
    long aux, posicao_final;

    scanf("%d", &saida_arquivo);

    if (saida_arquivo <= qnt_regitro)
    {
        print_registro print[saida_arquivo]; // vetor que ira armazenar os dados do registro na ordem correta de print
        arq = fopen("registros.bin", "rb");

        if (arq == NULL)
        {
            printf("erro ao abrir o arquivo para leitura\n");
            return -1;
        }

        fseek(arq, 0, SEEK_END);    // posicionar o ponteiro arq no fim do arquivo
        posicao_final = ftell(arq); // recebe o valor da ultima posição do arquivo(tamanho total)

        for (int a = 1; a <= saida_arquivo; a++)
        {

            // ira determinar o tamanho do bloco de registro a ser lido e começar a leitura a partir dele
            aux = (sizeof(int) * 2) + (sizeof(char) * (vet_reg[qnt_regitro - a].tamanho_titulo + 2)) + (sizeof(char) * (vet_reg[qnt_regitro - a].tamanho_autor));
            posicao_final = posicao_final - aux; // a nova posição do ponteiro vai para o começo do pŕóximo bloco de registro
            byte_offset = posicao_final;

            fseek(arq, byte_offset, SEEK_SET); // coloca o ponteiro arq no inicio do registro para então comecar a leitura

            // leitura do bloco de registro

            fread(&auxiliar.id, sizeof(int), 1, arq);
            fread(&auxiliar.titulo, sizeof(char), vet_reg[qnt_regitro - a].tamanho_titulo, arq);
            auxiliar.titulo[vet_reg[qnt_regitro - a].tamanho_titulo] = '\0'; // Adiciona o caractere nulo de terminação ao final da string,pois ao ler a string tal caracter não foi lido
            fread(&delimitador_titulo, sizeof(char), 1, arq);
            fread(&tamanho_autor, sizeof(int), 1, arq);
            fread(&auxiliar.autor, sizeof(char), vet_reg[qnt_regitro - a].tamanho_autor, arq);
            auxiliar.autor[vet_reg[qnt_regitro - a].tamanho_autor] = '\0'; // Adiciona o caractere nulo de terminação ao final da string,pois ao ler a string tal caracter não foi lido
            fread(&delimitador_registro, sizeof(char), 1, arq);

            // aloca os dados na posição correta para ser printada
            print[a - 1].id = auxiliar.id;
            strcpy(print[a - 1].autor, auxiliar.autor);
            strcpy(print[a - 1].titulo, auxiliar.titulo);
            print[a - 1].byte = byte_offset;
        }
        fclose(arq);

        for (int b = saida_arquivo - 1; b >= 0; b--)
        {
            printf("Id: %d\n", print[b].id);
            printf("Titulo: %s\n", print[b].titulo);
            printf("Autor: %s\n", print[b].autor);
            printf("Byte offset: %ld", print[b].byte);
            if (b > 0)
                printf("\n\n");
        }
    }

    return 0;
}
