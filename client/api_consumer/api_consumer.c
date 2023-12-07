#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

#include "../models/models.h"
#include "api_consumer.h"

#ifndef SOE_SERVER
#define SOE_SERVER "http://localhost:8080"
#endif

// 20MB
#define MAX_IMAGE_SIZE 20971520

size_t write_data(void* buffer, size_t size, size_t nmemb, void *userp) {
    strncpy((char*)userp, buffer, nmemb);
    return nmemb;
}

struct WriteDataBinaryUserParams {
    void* buffer;
    size_t pos;
};

size_t write_data_binary(void* buffer, size_t size, size_t nmemb, void* userp) {
    struct WriteDataBinaryUserParams* user_params = (struct WriteDataBinaryUserParams*)userp;
    memcpy(user_params->buffer+user_params->pos, buffer, nmemb);
    user_params->pos += nmemb;
    return nmemb;
}

PROVA_DATA* soe_get_provas(int *n) {
    char* buffer = malloc(sizeof(char) * 20480);

    char* url = malloc(sizeof(char) * 100);
    memset(url, 0, sizeof(char) * 100);
    strcat(url, SOE_SERVER "/provas");

    CURL* curl = curl_easy_init();
    CURLcode res;

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)buffer);
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return NULL;
    }

    cJSON* provas_json = cJSON_Parse(buffer);
    *n = get_list_size(provas_json);

    PROVA_DATA* provas = malloc(sizeof(PROVA_DATA)*(*n));

    model_fill_prova_data_list_from_json(provas, *n, provas_json);

    cJSON_Delete(provas_json);
    curl_easy_cleanup(curl);

    return provas;
}

PROVA_DATA* soe_get_prova(char* id) {
    PROVA_DATA* data = malloc(sizeof(PROVA_DATA));
    char* buffer = malloc(sizeof(char) * 5120);

    char* url = malloc(sizeof(char)*100);
    memset(url, 0, sizeof(char)*100);
    strcat(url, SOE_SERVER "/prova/");
    strcat(url, id);

    CURL* curl = curl_easy_init();
    CURLcode res;

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)buffer);
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return NULL;
    }

    cJSON* json = cJSON_Parse(buffer);
    model_fill_prova_data_from_json(data, json);

    cJSON_Delete(json);
    curl_easy_cleanup(curl);

    return data;
}

USUARIO_DATA* soe_get_usuarios_from_prova(char* id, int* n) {
    char* buffer = malloc(sizeof(char) * 10240);

    char* url = malloc(sizeof(char) * 100);
    memset(url, 0, sizeof(char) * 100);
    strcat(url, SOE_SERVER "/prova/");
    strcat(url, id);
    strcat(url, "/usuarios");

    CURL* curl = curl_easy_init();
    CURLcode res;

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)buffer);
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return NULL;
    }

    cJSON* usuarios_json = cJSON_Parse(buffer);
    *n = get_list_size(usuarios_json);

    USUARIO_DATA* usuarios = malloc(sizeof(USUARIO_DATA)*(*n));

    model_fill_usuario_data_list_from_json(usuarios, *n, usuarios_json);

    cJSON_Delete(usuarios_json);
    curl_easy_cleanup(curl);

    return usuarios;
}

void* soe_get_usuario_foto(char* cpf, size_t* buffer_size) {
    void* buffer = malloc(MAX_IMAGE_SIZE);
    memset(buffer, 0, MAX_IMAGE_SIZE);

    struct WriteDataBinaryUserParams params = {
        .buffer = buffer,
        .pos = 0
    };

    char* url = malloc(sizeof(char)*100);
    memset(url, 0, sizeof(char) * 100);
    strcat(url, SOE_SERVER "/usuario/");
    strcat(url, cpf);
    strcat(url, "/foto");

    CURL* curl = curl_easy_init();
    CURLcode res;

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_binary);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&params);
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return NULL;
    }

    curl_easy_cleanup(curl);

    *buffer_size = params.pos;
    return params.buffer;
}
