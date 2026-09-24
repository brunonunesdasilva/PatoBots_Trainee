#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

#include <btstack_port_esp32.h>
#include <btstack_run_loop.h>
#include <uni.h>

#include "motor.h"
#include "robot_task.h"
#include "bt_control.h"
#include "config.h"

void app_main(void) {
    // 1. Inicializa Memória (Essencial para o Bluetooth salvar o pareamento)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }

    // 2. Inicializa o hardware dos motores
    motor_init_stby(PIN_STBY);
    motor_init(get_motor_direito());
    motor_init(get_motor_esquerdo());
    // 3. Inicializa Bluetooth
    btstack_init();
    uni_platform_set_custom(get_my_platform());
    uni_init(0, NULL);

    printf("--- Bluetooth Iniciado. Aguardando Xbox... ---\n");

    // 4. Entra no loop do BTstack
    btstack_run_loop_execute();
}