#include "indice_primario.h"
#include "indice_secundario.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ARQUIVO_SECUNDARIO "arquivoIndiceSecundario.bin"
#define DELIMITADOR_INDICES "#"
#define DELIMITADOR_CAMPOS "|"

void ordena_indices_secundarios();
void selection_indiceSecundario(indice_secundario *vetor, int tamanho);
void inserir_registro_indiceSecundario();

// função responsável pela leitura/tratamento dos dados e realização das operações do input
int leitura_de_dados()
{
    registro auxiliar;
    char *input = calloc(1000, sizeof(char));
    scanf(" %[^(\r|\n)]", input);
    int leitura = 0;
    char *aux2;

    char *adicionar = calloc(4, sizeof(char));
    char *buscar = calloc(10, sizeof(char));
    char *remover = calloc(10, sizeof(char));

    strncpy(adicionar, &input[0], 3);
    adicionar[3] = '\0';
    strncpy(buscar, &input[0], 9);
    adicionar[9] = '\0';
    strncpy(remover, &input[0], 9);
    adicionar[9] = '\0';

    // irá inserir um novo registro
    if (strcmp(adicionar, "ADD") == 0)
    {
        for (int a = 0; a < strlen(input); a++)
        {
            int indiceFinal = 0;

            if (input[a] == '\'')
            {
                for (int b = a + 1; input[b] != '\''; b++)
                {
                    indiceFinal = b;
                }

                switch (leitura)
                {
                case 0:
                    aux2 = calloc(indiceFinal - a, sizeof(char));
                    strncpy(aux2, &input[a + 1], indiceFinal - a);
                    aux2[indiceFinal - a] = '\0';
                    auxiliar.id = atoi(aux2);
                    leitura++;
                    free(aux2);
                    break;
                case 1:
                    strncpy(auxiliar.titulo, &input[a + 1], indiceFinal - a);
                    auxiliar.titulo[indiceFinal - a] = '\0';
                    leitura++;
                    break;
                case 2:
                    strncpy(auxiliar.autor, &input[a + 1], indiceFinal - a);
                    auxiliar.autor[indiceFinal - a] = '\0';
                    leitura++;
                    break;

                default:
                    break;
                }
                a = indiceFinal + 1; // pula para a  segunda ocorrencia do  caracter
            }
        }
        printf("----------------------------------------------------------\n");
        inserir_registro_indiceSecundario(auxiliar);
        return 1;
    }

    // pesquisa por um registro por meio da chave ID
    if (strcmp(buscar, "SEARCH id") == 0)
    {
        for (int a = 0; a < strlen(input); a++)
        {
            int indiceFinal = 0;

            if (input[a] == '\'')
            {
                for (int b = a + 1; input[b] != '\''; b++)
                {
                    indiceFinal = b;
                }
                aux2 = calloc(indiceFinal - a, sizeof(char));
                strncpy(aux2, &input[a + 1], indiceFinal - a);
                aux2[indiceFinal - a] = '\0';
                auxiliar.id = atoi(aux2);
                free(aux2);
                break;
            }
        }
        printf("----------------------------------------------------------\n");
        pesquisar_registro_indicePrimario((long int)auxiliar.id);
        return 1;
    }
    else // pesquisa por um registro por meio da chave Autor
    {
        if (strcmp(buscar, "SEARCH au") == 0)
        {
            for (int a = 0; a < strlen(input); a++)
            {
                int indiceFinal = 0;

                if (input[a] == '\'')
                {
                    for (int b = a + 1; input[b] != '\''; b++)
                    {
                        indiceFinal = b;
                    }

                    strncpy(auxiliar.autor, &input[a + 1], indiceFinal - a);
                    auxiliar.autor[indiceFinal - a] = '\0';
                    break;
                }
            }
            printf("----------------------------------------------------------\n");
            pesquisar_registro_indiceSecundario(auxiliar.autor);
            return 1;
        }
    }

    // remove o registro por meio da chave ID
    if (strcmp(remover, "REMOVE id") == 0)
    {
        for (int a = 0; a < strlen(input); a++)
        {
            int indiceFinal = 0;

            if (input[a] == '\'')
            {
                for (int b = a + 1; input[b] != '\''; b++)
                {
                    indiceFinal = b;
                }
                aux2 = calloc(indiceFinal - a, sizeof(char));
                strncpy(aux2, &input[a + 1], indiceFinal - a);
                aux2[indiceFinal - a] = '\0';
                auxiliar.id = atoi(aux2);
                free(aux2);
                break;
            }
        }
        printf("----------------------------------------------------------\n");
        remover_registro_indicePrimario((long int)auxiliar.id);
        return 1;
    }
    else // remove o registro por meio da chave Autor
    {
        if (strcmp(remover, "REMOVE au") == 0)
        {
            for (int a = 0; a < strlen(input); a++)
            {
                int indiceFinal = 0;

                if (input[a] == '\'')
                {
                    for (int b = a + 1; input[b] != '\''; b++)
                    {
                        indiceFinal = b;
                    }

                    strncpy(auxiliar.autor, &input[a + 1], indiceFinal - a);
                    auxiliar.autor[indiceFinal - a] = '\0';
                    break;
                }
            }
            printf("----------------------------------------------------------\n");
            remover_registro_indiceSecundario(auxiliar.autor);
            return 1;
        }
    }

    return 0;
}

// função responsável por adicionar um registro no arquivo de dados ,indice primário e de indice secundário
void inserir_registro_indiceSecundario(registro entradaDado)
{

    indice_secundario *registro = inserir_registro_indicePrimario(entradaDado); // insere um novo registro no arquivo de indice primário
    if (registro != NULL)
    { // insere um novo registro no arquivo de indice secundario
        FILE *arquivo = fopen(ARQUIVO_SECUNDARIO, "ab");
        fwrite(&(registro[0].autor), sizeof(char), strlen(registro[0].autor), arquivo);
        fwrite(DELIMITADOR_CAMPOS, sizeof(char), 1, arquivo);
        fwrite(&(registro[0].id), sizeof(long int), 1, arquivo);
        fwrite(DELIMITADOR_INDICES, sizeof(char), 1, arquivo);
        fclose(arquivo);
        ordena_indices_secundarios();
        printf("Registro inserido\n");
    }
    else
        printf("Erro ao inserir registro, chave primária duplicada\n");
}

// função responsável por pesguisar os registros que apresentem a chave autor
void pesquisar_registro_indiceSecundario(char *autor)
{
    FILE *arquivo = fopen(ARQUIVO_SECUNDARIO, "rb");
    if (arquivo == NULL)
    {
        printf("Não encontrado\n");
        return;
    }
    int quantidade_autores = 0, a = 0;
    char autor1[100], caracter;
    long int id;

    // pesquisa se o autor está presente no registro e quantas vezes ele aparece no arquivo de indice secundário
    while (fread(&caracter, sizeof(char), 1, arquivo) == 1)
    {
        if (caracter == '|')
        {
            autor1[a] = '\0';
            if (strcmp(autor1, autor) == 0)
                quantidade_autores++;

            fread(&id, sizeof(long int), 1, arquivo);
            fread(&caracter, sizeof(char), 1, arquivo);

            a = 0;
            continue;
        }
        else
        {
            autor1[a] = caracter;
            a++;
        }
    }

    fseek(arquivo, 0, SEEK_SET);

    if (quantidade_autores != 0)
    {
        indice_secundario *auxiliar = calloc(quantidade_autores, sizeof(indice_secundario));
        int i = 0;

        // leitura dos autores na variável auxiliar
        while (fread(&caracter, sizeof(char), 1, arquivo) == 1)
        {

            if (caracter == '|') // leitura de um autor completa
            {
                autor1[a] = '\0';
                if (strcmp(autor1, autor) == 0)
                {
                    strcpy(auxiliar[i].autor, autor1);
                    fread(&id, sizeof(long int), 1, arquivo);
                    auxiliar[i].id = id;
                    fread(&caracter, sizeof(char), 1, arquivo);
                    i++;
                    a = 0;
                    continue;
                }

                fread(&id, sizeof(long int), 1, arquivo);
                fread(&caracter, sizeof(char), 1, arquivo);
                a = 0;
                continue;
            }
            else
            {
                autor1[a] = caracter;
                a++;
            }
        }
        fclose(arquivo);

        for (int b = 0; b < quantidade_autores; b++)
            pesquisar_registro_indicePrimario(auxiliar[b].id);
    }
    else
    {
        fclose(arquivo);
        printf("Não encontrado\n");
    }
}

// função responsável por remover no arquivo de indice secundário os registros com a chave autor
void remover_registro_indiceSecundario(char *autor)
{
    FILE *arquivo = fopen(ARQUIVO_SECUNDARIO, "rb");
    if (arquivo == NULL)
    {
        printf("Erro ao remover\n");
        return;
    }
    int quantidade_autores = 0, quantidade_registros = 0, a = 0;
    char autor1[100], caracter;
    long int id;

    // pesquisa se o autor está presente no registro e quantas vezes ele aparece no arquivo de indice secundário e também quantos registros estão presentes no arquivo
    while (fread(&caracter, sizeof(char), 1, arquivo) == 1)
    {
        if (caracter == '|')
        {
            autor1[a] = '\0';
            if (strcmp(autor1, autor) == 0)
                quantidade_autores++;

            fread(&id, sizeof(long int), 1, arquivo);
            fread(&caracter, sizeof(char), 1, arquivo);
            quantidade_registros++;
            a = 0;
            continue;
        }
        else
        {
            autor1[a] = caracter;
            a++;
        }
    }
    // cria um novo vetor que irá ser salvo no arquivo de indice secundário com os registros atualizados
    indice_secundario *vetor_novo = calloc(quantidade_registros - quantidade_autores, sizeof(indice_secundario));

    fseek(arquivo, 0, SEEK_SET);
    int i = 0;

    if (quantidade_autores > 0)
    {
        // remove no arquivo de indice primário e arquivo de dados os registros que apresentam o a chave autor do arquivo de indice secundário
        while (fread(&caracter, sizeof(char), 1, arquivo) == 1)
        {
            if (caracter == '|')
            {
                autor1[a] = '\0';
                if (strcmp(autor1, autor) == 0)
                { // removendo o autor do arquivo de indice primário e secundário
                    fread(&id, sizeof(long int), 1, arquivo);
                    remover_registro_indicePrimario(id);
                    fread(&caracter, sizeof(char), 1, arquivo);
                    a = 0;
                    continue;
                }
                strcpy(vetor_novo[i].autor, autor1);
                fread(&id, sizeof(long int), 1, arquivo);
                vetor_novo[i].id = id;
                fread(&caracter, sizeof(char), 1, arquivo);
                i++;
                a = 0;
                continue;
            }
            else
            {
                autor1[a] = caracter;
                a++;
            }
        }

        fclose(arquivo);

        arquivo = fopen(ARQUIVO_SECUNDARIO, "wb"); // limpa o arquivo de indice secundário
        fclose(arquivo);

        arquivo = fopen(ARQUIVO_SECUNDARIO, "ab");

        // escreve no arquivo de indice secundário os registros atualizados após a remoção
        for (int a = 0; a < quantidade_registros - quantidade_autores; a++)
        {
            fseek(arquivo, 0, SEEK_END);
            fwrite(&(vetor_novo[a].autor), sizeof(char), strlen(vetor_novo[a].autor), arquivo);
            fwrite(DELIMITADOR_CAMPOS, sizeof(char), 1, arquivo);
            fwrite(&(vetor_novo[a].id), sizeof(long int), 1, arquivo);
            fwrite(DELIMITADOR_INDICES, sizeof(char), 1, arquivo);
        }
        fclose(arquivo);
    }
    else
        printf("Erro ao remover\n");
}

// função responsável por remover o a chave autor que apresenta o idCHAVE
void remover_registro_indiceSecundario_id(long int idChave)
{
    FILE *arquivo = fopen(ARQUIVO_SECUNDARIO, "rb");
    int quantidade_registros = 0, a = 0;
    char autor1[100], caracter;
    long int id;

    // pesquisa quantos registros estão presentes no arquivo
    while (fread(&caracter, sizeof(char), 1, arquivo) == 1)
    {
        if (caracter == '|')
        {
            autor1[a] = '\0';
            fread(&id, sizeof(long int), 1, arquivo);
            fread(&caracter, sizeof(char), 1, arquivo);
            quantidade_registros++;
            a = 0;
            continue;
        }
        else
        {
            autor1[a] = caracter;
            a++;
        }
    }

    // cria um vetor auxiliar que irá ser escrito no arquivo atualizando ele(retira o autor que possui o id removido)
    indice_secundario *vetor_novo_id = calloc(quantidade_registros - 1, sizeof(indice_secundario));
    fseek(arquivo, 0, SEEK_SET);
    int i = 0;

    while (fread(&caracter, sizeof(char), 1, arquivo) == 1)
    {
        if (caracter == '|')
        {
            autor1[a] = '\0';
            fread(&id, sizeof(long int), 1, arquivo);
            if (id == idChave) // não inclui o autor e o id no novo arquivo de indice secundário
            {
                fread(&caracter, sizeof(char), 1, arquivo);
                a = 0;
                continue;
            }

            fread(&caracter, sizeof(char), 1, arquivo);
            strcpy(vetor_novo_id[i].autor, autor1);
            vetor_novo_id[i].id = id;
            i++;
            a = 0;
            continue;
        }
        else
        {
            autor1[a] = caracter;
            a++;
        }
    }

    arquivo = fopen(ARQUIVO_SECUNDARIO, "wb"); // limpa o arquivo de indice secundário
    fclose(arquivo);

    arquivo = fopen(ARQUIVO_SECUNDARIO, "ab");

    // escreve no arquivo de indice secundário os registros atualizados após a remoção
    for (int a = 0; a < quantidade_registros - 1; a++)
    {
        fseek(arquivo, 0, SEEK_END);
        fwrite(&(vetor_novo_id[a].autor), sizeof(char), strlen(vetor_novo_id[a].autor), arquivo);
        fwrite(DELIMITADOR_CAMPOS, sizeof(char), 1, arquivo);
        fwrite(&(vetor_novo_id[a].id), sizeof(long int), 1, arquivo);
        fwrite(DELIMITADOR_INDICES, sizeof(char), 1, arquivo);
    }
    fclose(arquivo);
}
// função responsável por ordenar o arquivo de indice secundário
void ordena_indices_secundarios()
{
    int qnt_registros = 0;
    long int id;
    char autor[100], caracter;

    FILE *arquivo = fopen(ARQUIVO_SECUNDARIO, "rb");
    // percorre o arquivo de índice secundário e verifica a quantidade de registros presentes
    while (fread(&caracter, sizeof(char), 1, arquivo) == 1)
    {
        if (caracter == '|')
        {
            fread(&id, sizeof(long int), 1, arquivo);
            fread(&caracter, sizeof(char), 1, arquivo);
            qnt_registros++;
        }
    }

    fclose(arquivo);

    arquivo = fopen(ARQUIVO_SECUNDARIO, "rb");
    indice_secundario *registro = calloc(qnt_registros, sizeof(indice_secundario));
    int i = 0, h = 0;

    // salva os registros do arquivo de indice secundário no vetor registro(que irá ser ordenado e atualizado)

    while (fread(&caracter, sizeof(char), 1, arquivo) == 1)
    {
        if (caracter == '|')
        {
            autor[h] = '\0';
            strcpy(registro[i].autor, autor);
            fread(&id, sizeof(long int), 1, arquivo);
            registro[i].id = id;
            fread(&caracter, sizeof(char), 1, arquivo);
            h = 0;
            i++;
            continue;
        }
        else
        {
            autor[h] = caracter;
            h++;
        }
    }

    fclose(arquivo);

    selection_indiceSecundario(registro, qnt_registros); // ordena o arquivo de indice secundário

    arquivo = fopen(ARQUIVO_SECUNDARIO, "wb"); // limpa o arquivo de indice secundário
    fclose(arquivo);

    arquivo = fopen(ARQUIVO_SECUNDARIO, "ab");
    // escrita dos registros ornenados no arquivo secundário
    for (int i = 0; i < qnt_registros; i++)
    {
        fseek(arquivo, 0, SEEK_END);
        fwrite(registro[i].autor, sizeof(char), strlen(registro[i].autor), arquivo);
        fwrite(DELIMITADOR_CAMPOS, sizeof(char), 1, arquivo);
        fwrite(&registro[i].id, sizeof(long int), 1, arquivo);
        fwrite(DELIMITADOR_INDICES, sizeof(char), 1, arquivo);
    }

    fclose(arquivo);
}

// algortimo responsável por ordenar o vetor de registros secundários
void selection_indiceSecundario(indice_secundario *vetor, int tamanho)
{
    int auxiliar, b;
    indice_secundario auxiliar_troca;

    for (int a = 0; a < tamanho - 1; a++)
    {
        auxiliar = a;

        for (b = a + 1; b < tamanho; b++)
        {
            if (strcmp(vetor[auxiliar].autor, vetor[b].autor) > 0)
            {
                auxiliar = b;
            }

            if (strcmp(vetor[auxiliar].autor, vetor[b].autor) == 0)
            { // ordena pla chave id
                if (vetor[auxiliar].id > vetor[b].id)
                    auxiliar = b;
            }
        }

        if (auxiliar != a)
        {
            auxiliar_troca = vetor[a];
            vetor[a] = vetor[auxiliar];
            vetor[auxiliar] = auxiliar_troca;
        }
    }
}
