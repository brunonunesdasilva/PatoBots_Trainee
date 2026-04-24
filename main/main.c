#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include "sensor_distancia.h"
#include "motor.h"

void app_main(void) {
    printf("Iniciando rotina de testes do motor direito...\n");

    // ==========================================
    // 1. DEFINIÇÃO DOS PINOS
    // ==========================================
    gpio_num_t PINO_STBY = GPIO_NUM_26;

    // Configuração apenas do motor Direito
    motor_config_t motor_direito = {
        .pin_in1     = GPIO_NUM_18,
        .pin_in2     = GPIO_NUM_19,
        .pin_pwm     = GPIO_NUM_21,
        .pwm_channel = LEDC_CHANNEL_0
    };

    // ==========================================
    // 2. INICIALIZAÇÃO
    // ==========================================
    // Habilita a ponte H
    motor_init_stby(PINO_STBY);

    // Inicializa o motor direito
    motor_init(&motor_direito);

    printf("Motor inicializado. Começando sequência de movimentos.\n");

    // ==========================================
    // 3. LOOP DE TESTES
    // ==========================================
    while (1) {
        // --- MOVIMENTO 1: FRENTE (Potência Máxima) ---
        printf("Avançando (100%% da força)\n");
        motor_set_speed(&motor_direito, 1023);
        vTaskDelay(pdMS_TO_TICKS(2000)); // Mantém por 2 segundos

        // --- MOVIMENTO 2: FREIO BRUSCO ---
        printf("Freando!\n");
        motor_set_speed(&motor_direito, 0);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Espera 1 segundo

        // --- MOVIMENTO 3: FRENTE (Meia Potência) ---
        printf("Avançando (50%% da força para testar o PWM)\n");
        motor_set_speed(&motor_direito, 512);
        vTaskDelay(pdMS_TO_TICKS(1500)); // Mantém por 1.5 segundos

        // --- MOVIMENTO 4: FREIO ---
        printf("Freando!\n");
        motor_set_speed(&motor_direito, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));

        // --- MOVIMENTO 5: RÉ ---
        printf("Recuando (Ré)\n");
        motor_set_speed(&motor_direito, -800); // Valor negativo inverte a direção
        vTaskDelay(pdMS_TO_TICKS(2000)); // Mantém por 2 segundos

        // --- MOVIMENTO 6: PARADA FINAL ANTES DE REINICIAR ---
        printf("Parada para reiniciar o ciclo...\n");
        motor_set_speed(&motor_direito, 0);
        vTaskDelay(pdMS_TO_TICKS(3000)); // Pausa maior antes de repetir o loop
    }
}
