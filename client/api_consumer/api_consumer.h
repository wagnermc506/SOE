#ifndef _API_CONSUMER_H_
#define _API_CONSUMER_H_

#include "../models/models.h"

PROVA_DATA* soe_get_prova(char* id);
PROVA_DATA* soe_get_provas(int *n);
USUARIO_DATA* soe_get_usuarios_from_prova(char* id, int* n);
void* soe_get_usuario_foto(char* cpf, size_t* buffer_size);

#endif
