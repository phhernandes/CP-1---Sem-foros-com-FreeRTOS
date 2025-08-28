#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

// Definindo semáforos binários
SemaphoreHandle_t xSemaphore1;
SemaphoreHandle_t xSemaphore2;
SemaphoreHandle_t xSemaphore3;

// Nome do aluno
#define NOME_ALUNO "Pedro"

// Função da Tarefa 1
void Tarefa1(void *param) {
    while (1) {
        // Espera até que o semáforo seja dado
        if (xSemaphoreTake(xSemaphore1, portMAX_DELAY)) {
            printf("[Tarefa 1] Executou - %s\n", NOME_ALUNO);
            vTaskDelay(1000 / portTICK_PERIOD_MS);  // Atraso de 1 segundo
            // Dá o semáforo para a próxima tarefa (Tarefa 2)
            xSemaphoreGive(xSemaphore2);
        }
    }
}

// Função da Tarefa 2
void Tarefa2(void *param) {
    while (1) {
        if (xSemaphoreTake(xSemaphore2, portMAX_DELAY)) {
            printf("[Tarefa 2] Executou - %s\n", NOME_ALUNO);
            vTaskDelay(1000 / portTICK_PERIOD_MS);  // Atraso de 1 segundo
            // Dá o semáforo para a próxima tarefa (Tarefa 3)
            xSemaphoreGive(xSemaphore3);
        }
    }
}

// Função da Tarefa 3
void Tarefa3(void *param) {
    while (1) {
        if (xSemaphoreTake(xSemaphore3, portMAX_DELAY)) {
            printf("[Tarefa 3] Executou - %s\n", NOME_ALUNO);
            vTaskDelay(1000 / portTICK_PERIOD_MS);  // Atraso de 1 segundo
            // Dá o semáforo para a próxima tarefa (Tarefa 1)
            xSemaphoreGive(xSemaphore1);
        }
    }
}

// Função principal (app_main)
void app_main(void) {
    // Criando os semáforos binários
    xSemaphore1 = xSemaphoreCreateBinary();
    xSemaphore2 = xSemaphoreCreateBinary();
    xSemaphore3 = xSemaphoreCreateBinary();
    
    if (xSemaphore1 == NULL || xSemaphore2 == NULL || xSemaphore3 == NULL) {
        printf("Falha ao criar os semáforos\n");
        return;
    }

    // Inicializando o semáforo da Tarefa 1 com 1, para começar com ela
    xSemaphoreGive(xSemaphore1);

    // Criando as tarefas
    xTaskCreate(Tarefa1, "Tarefa1", 2048, NULL, 1, NULL);
    xTaskCreate(Tarefa2, "Tarefa2", 2048, NULL, 1, NULL);
    xTaskCreate(Tarefa3, "Tarefa3", 2048, NULL, 1, NULL);
}
