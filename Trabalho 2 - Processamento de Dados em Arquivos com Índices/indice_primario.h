#ifndef INDICE_PRIMARIO_H
#define INDICE_PRIMARIO_H
#include "indice_secundario.h"
#include "registro.h"

typedef struct indice_primario
{
    long int id;
    long int byte_offset;

} indice_primario;

indice_secundario *inserir_registro_indicePrimario(registro entradaDado); // função responsável por inserir um registro no indice secundario e no arquivo de dados
void pesquisar_registro_indicePrimario(long int id);                      // função responsável por busca os registros no arquivo de dados que apresentem a chave ID
void remover_registro_indicePrimario(long int id);                        // função responsável por remover o registro no arquivo de indice secundário que apresente a chave ID

#endif