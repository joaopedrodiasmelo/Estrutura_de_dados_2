#ifndef INDICE_SECUNDARIO_H
#define INDICE_SECUNDARIO_H
#include "registro.h"
typedef struct indice_secundario
{
    long int id;
    char autor[100];

} indice_secundario;

int leitura_de_dados();
void inserir_registro_indiceSecundario(registro entradaDado); // função responsável por inserir um novo registro nos arquivos(primario,secundário e dados)
void pesquisar_registro_indiceSecundario(char *autor);        // função responsável por busca os registros no arquivo de dados que apresentem a chave autor
void remover_registro_indiceSecundario(char *autor);          // função responsável por remover o a chave autor dos arquivos
void remover_registro_indiceSecundario_id(long int idChave);

#endif
