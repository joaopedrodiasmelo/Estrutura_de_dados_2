#include "indice_primario.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define ARQUIVO_PRIMARIO "arquivoIndicePrimario.bin"
#define DELIMITADOR_INDICES "#"

void selection(indice_primario *vetor, int tamanho);
void ordenar_indices_primarios();
bool verificar_existencia_id(long id);
long int busca_binaria(indice_primario *vetor, int tamanho, long int id);
long int busca_binaria_remover(indice_primario *vetor, int tamanho, long int id);

// função que insere registros novos(chaves que não estão presentes no arquivo de indice ainda)
indice_secundario *inserir_registro_indicePrimario(registro entradaDado)
{

    registro entrada = entradaDado;
    indice_primario indice;
    indice.id = (long int)entrada.id;

    FILE *arquivo_dados = fopen(ARQUIVO_PRIMARIO, "ab");
    
    fseek(arquivo_dados, 0, SEEK_END);

    if (verificar_existencia_id(indice.id)) // verifica se  o ID ja está presente no arquivo
    {
        indice_secundario *retorno = calloc(1, sizeof(indice_secundario));
        // salva os dados no  registro do arquivo  de índice secundário
        retorno[0].id = indice.id;
        strcpy(retorno[0].autor, entrada.autor);

        long int *aux = inserir_registro(entrada); // função que insere um novo registro no arquivo de dados e retorna  o Byte_ofset
        indice.byte_offset = aux[0];
        free(aux);
        // escrita do indice primário no arquivo de indice primário
        fwrite(&indice.id, sizeof(long int), 1, arquivo_dados);
        fwrite(&indice.byte_offset, sizeof(long int), 1, arquivo_dados);
        fwrite(DELIMITADOR_INDICES, sizeof(char), 1, arquivo_dados);
        fclose(arquivo_dados);
        ordenar_indices_primarios(); // ordena o arquivo de indices primários

        return retorno; // retona a variável que irá ser adicionada ao arquivo de indice secundário
    }

    return NULL; // retorna NULL para o arquivo de indice secundário o que garante que não se pode inserir um novo registro
}

// função que pesquisa um registro no arquivo de dados a partir da chave ID do arquivo de indice primário
void pesquisar_registro_indicePrimario(long int id)
{
    FILE *arquivo = fopen(ARQUIVO_PRIMARIO, "rb");
    if (arquivo == NULL)
    {
        printf("Não encontrado\n");
        return;
    }
    int qnt_registros = 0;
    long int aux;
    char aux2;

    // percorre o arquivo de índice primário e verifica a quantidade de registros presentes
    while (fread(&aux, sizeof(long int), 1, arquivo) == 1)
    {
        fread(&aux, sizeof(long int), 1, arquivo);
        fread(&aux2, sizeof(char), 1, arquivo);
        qnt_registros++;
    }
    fclose(arquivo);

    indice_primario *auxiliar = calloc(qnt_registros, sizeof(indice_primario));

    arquivo = fopen(ARQUIVO_PRIMARIO, "rb");

    // leitura de cada registro em um vetor auxiliar
    int a = 0;
    while (fread(&aux, sizeof(long int), 1, arquivo) == 1)
    {
        auxiliar[a].id = aux;
        fread(&aux, sizeof(long int), 1, arquivo);
        auxiliar[a].byte_offset = aux;
        fread(&aux2, sizeof(char), 1, arquivo);
        a++;
    }

    long int byteOfseet = busca_binaria(auxiliar, qnt_registros, id); //encontra o id procurado e retorna o byteofset
    if (byteOfseet == -1)
        printf("Não encontrado\n");

    else
    {
        registro auxiliar = pesquisar_registro_id(byteOfseet);
        printf("%d - %s - %s\n", auxiliar.id, auxiliar.titulo, auxiliar.autor);
    }
}

// função responsável por remover um registro do arquivo de dados e do arquivo de indice primário
void remover_registro_indicePrimario(long int id)
{
    FILE *arquivo = fopen(ARQUIVO_PRIMARIO, "rb");
    if (arquivo == NULL)
    {
        printf("Erro ao remover\n");
        return;
    }

    int qnt_registros = 0;
    long int aux;
    char aux2;

    // percorre o arquivo de índice primário e verifica a quantidade de registros presentes
    while (fread(&aux, sizeof(long int), 1, arquivo) == 1)
    {
        fread(&aux, sizeof(long int), 1, arquivo);
        fread(&aux2, sizeof(char), 1, arquivo);
        qnt_registros++;
    }
    fclose(arquivo);

    indice_primario *auxiliar = calloc(qnt_registros, sizeof(indice_primario));

    arquivo = fopen(ARQUIVO_PRIMARIO, "rb");

    // leitura de cada registro em um vetor auxiliar
    int a = 0;
    while (fread(&aux, sizeof(long int), 1, arquivo) == 1)
    {
        auxiliar[a].id = aux;
        fread(&aux, sizeof(long int), 1, arquivo);
        auxiliar[a].byte_offset = aux;
        fread(&aux2, sizeof(char), 1, arquivo);
        a++;
    }

    fclose(arquivo);

    long int byteOfseet = busca_binaria(auxiliar, qnt_registros, id);

    if (byteOfseet == -1)
        printf("Erro ao remover\n");
    else
    {
        remover_registro(byteOfseet);             // realiza a remoção lógica no arquivo de dados
        remover_registro_indiceSecundario_id(id); // realiza a remoção do arquivo de indice secundário

        if (qnt_registros == 1)
        {
            arquivo = fopen(ARQUIVO_PRIMARIO, "wb"); // limpando o arquivo de índice primário
            fclose(arquivo);
        }
        else
        {
            arquivo = fopen(ARQUIVO_PRIMARIO, "wb"); // limpando o arquivo de índice primário
            fclose(arquivo);

            long int indice = busca_binaria_remover(auxiliar, qnt_registros, id);

            indice_primario *novo_indice = calloc(qnt_registros - 1, sizeof(indice_primario));

            // preenche a nova lista de registros retirando o registro removido
            for (int a = 0; a < qnt_registros - 1; a++)
            {
                if (a < indice)
                    novo_indice[a] = auxiliar[a];
                else
                {
                    novo_indice[a] = auxiliar[a + 1];
                }
            }

            arquivo = fopen(ARQUIVO_PRIMARIO, "ab"); // gravando os registros após a remoção

            for (int b = 0; b < qnt_registros - 1; b++)
            {
                fseek(arquivo, 0, SEEK_END);
                fwrite(&novo_indice[b].id, sizeof(long int), 1, arquivo);
                fwrite(&novo_indice[b].byte_offset, sizeof(long int), 1, arquivo);
                fwrite(DELIMITADOR_INDICES, sizeof(char), 1, arquivo);
            }

            fclose(arquivo);
        }

        printf("Registro removido\n");
    }
}

// função responsável por buscar um id e retornar sua posição do arquivo de dados
long int busca_binaria(indice_primario *vetor, int tamanho, long int id)
{
    int inicio = 0;
    int fim = tamanho - 1;

    if (tamanho == 1)
    {
        if (vetor[0].id == id)
            return vetor[0].byte_offset;
        else
            return -1;
    }

    while (inicio <= fim)
    {
        int meio = (inicio + fim) / 2;

        if (vetor[meio].id == id)
        {
            return vetor[meio].byte_offset;
        }
        else if (vetor[meio].id < id)
        {
            inicio = meio + 1;
        }
        else
        {
            fim = meio - 1;
        }
    }

    return -1; // Indica que o elemento não foi encontrado
}

// função para encontrar a posição do elemento a ser removido na vetor de registros de indices primários
long int busca_binaria_remover(indice_primario *vetor, int tamanho, long int id)
{
    int inicio = 0;
    int fim = tamanho - 1;

    if (tamanho == 1)
    {
        if (vetor[0].id == id)
            return 0;
        else
            return -1;
    }

    while (inicio <= fim)
    {
        int meio = (inicio + fim) / 2;

        if (vetor[meio].id == id)
        {
            return meio; // retorna o indice do elemento a ser removido
        }
        else if (vetor[meio].id < id)
        {
            inicio = meio + 1;
        }
        else
        {
            fim = meio - 1;
        }
    }

    return -1; // Indica que o elemento não foi encontrado
}

// função para verificar se a chave id já esta presente no arquivo de indice primário
bool verificar_existencia_id(long id)
{
    long int aux;
    char aux2;
    FILE *arquivo = fopen(ARQUIVO_PRIMARIO, "rb");
    

    while (fread(&aux, sizeof(long int), 1, arquivo) == 1)
    {
        if (aux == id)
        {
            fclose(arquivo);
            return false; // chave primária ja está presente
        }

        fread(&aux, sizeof(long int), 1, arquivo);
        fread(&aux2, sizeof(char), 1, arquivo);
    }
    fclose(arquivo);

    return true; // chave primária(id) não está presente
}

// função responsável por ordenar o arquivo de indice primário
void ordenar_indices_primarios()
{
    FILE *arquivo_ordenar = fopen(ARQUIVO_PRIMARIO, "rb");
    int qnt_registros = 0;
    long int aux;
    char aux2;

    // percorre o arquivo de índice primário e verifica a quantidade de registros presentes
    while (fread(&aux, sizeof(long int), 1, arquivo_ordenar) == 1)
    {
        fread(&aux, sizeof(long int), 1, arquivo_ordenar);
        fread(&aux2, sizeof(char), 1, arquivo_ordenar);
        qnt_registros++;
    }
    fclose(arquivo_ordenar);

    int a = 0;
    indice_primario *nao_ordenados = calloc(qnt_registros, sizeof(indice_primario));
    arquivo_ordenar = fopen(ARQUIVO_PRIMARIO, "rb");

    // leitura de cada registro em um vetor auxiliar
    while (fread(&aux, sizeof(long int), 1, arquivo_ordenar) == 1)
    {
        nao_ordenados[a].id = aux;
        fread(&aux, sizeof(long int), 1, arquivo_ordenar);
        nao_ordenados[a].byte_offset = aux;
        fread(&aux2, sizeof(char), 1, arquivo_ordenar);
        a++;
    }

    fclose(arquivo_ordenar);

    selection(nao_ordenados, qnt_registros); // ordenando o vetor auxiliar de registros

    arquivo_ordenar = fopen(ARQUIVO_PRIMARIO, "wb"); // limpando o arquivo de dados
    fclose(arquivo_ordenar);

    arquivo_ordenar = fopen(ARQUIVO_PRIMARIO, "ab");
    // escrevendo no arquivo de índice secundário o vetor ordenado
    for (int b = 0; b < qnt_registros; b++)
    {
        fseek(arquivo_ordenar, 0, SEEK_END);
        fwrite(&nao_ordenados[b].id, sizeof(long int), 1, arquivo_ordenar);
        fwrite(&nao_ordenados[b].byte_offset, sizeof(long int), 1, arquivo_ordenar);
        fwrite(DELIMITADOR_INDICES, sizeof(char), 1, arquivo_ordenar);
    }
    fclose(arquivo_ordenar);
}

// algortimo para ordenar os registros a partir da chave (ID)
void selection(indice_primario *vetor, int tamanho)
{
    int auxiliar, b;
    indice_primario auxiliar_troca;

    for (int a = 0; a < tamanho - 1; a++)
    {
        auxiliar = a;
        int print = 0; // Inicializa a variável print como 0

        for (b = a + 1; b < tamanho; b++)
        {
            if (vetor[auxiliar].id > vetor[b].id)
            {
                auxiliar = b;
                print = 1;
            }
        }

        if (print == 1)
        {
            auxiliar_troca = vetor[a];
            vetor[a] = vetor[auxiliar];
            vetor[auxiliar] = auxiliar_troca;
        }
    }
}
