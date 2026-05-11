/*#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "controle.h"
#include "motor.h"
#include "pinos.h"

// Headers corretos do bluepad32 v4.2
#include "uni.h"
#include "controller/uni_gamepad.h"
#include "controller/uni_controller.h"
#include "platform/uni_platform_custom.h"

ModoRobo modo_atual = MODO_RC;

extern motor_config_t motor_direito;
extern motor_config_t motor_esquerdo;

#define ZONA_MORTA 40

static int limitar(int valor, int min, int max) {
    if (valor < min) return min;
    if (valor > max) return max;
    return valor;
}

// Callback chamado quando um controle conecta
static void on_connected(uni_hid_device_t *d) {
    printf("[BT] Controle conectado!\n");
}

// Callback chamado quando um controle desconecta
static void on_disconnected(uni_hid_device_t *d) {
    printf("[BT] Controle desconectado — parando motores\n");
    motor_set_speed(&motor_direito,  0);
    motor_set_speed(&motor_esquerdo, 0);
}

// Callback principal — chamado a cada atualização do controle
static void on_controller_data(uni_hid_device_t *d, uni_controller_t *ctl) {
    // Só processa gamepads
    if (ctl->klass != UNI_CONTROLLER_CLASS_GAMEPAD) return;

    uni_gamepad_t *gp = &ctl->gamepad;

    // Botão B — alterna modo
    static bool botao_b_anterior = false;
    bool botao_b = (gp->buttons & BUTTON_B) != 0;
    if (botao_b && !botao_b_anterior) {
        if (modo_atual == MODO_RC) {
            modo_atual = MODO_AUTONOMO;
            motor_set_speed(&motor_direito,  0);
            motor_set_speed(&motor_esquerdo, 0);
            printf("[MODO] Autonomo\n");
        } else {
            modo_atual = MODO_RC;
            printf("[MODO] RC\n");
        }
    }
    botao_b_anterior = botao_b;

    if (modo_atual != MODO_RC) return;

    // Gatilhos: 0 a 1023
    int throttle = gp->throttle; // gatilho direito — frente
    int brake    = gp->brake;    // gatilho esquerdo — ré

    // Analógico esquerdo X: -512 a +511
    int axis_x = gp->axis_x;
    if (axis_x > -ZONA_MORTA && axis_x < ZONA_MORTA) axis_x = 0;

    // Mistura arcade
    int velocidade_base = throttle - brake;
    int direcao         = axis_x / 2;

    int vel_dir = limitar(velocidade_base - direcao, -1023, 1023);
    int vel_esq = limitar(velocidade_base + direcao, -1023, 1023);

    printf("T:%4d B:%4d X:%4d | MD:%5d ME:%5d\n",
           throttle, brake, axis_x, vel_dir, vel_esq);

    motor_set_speed(&motor_direito,  vel_dir);
    motor_set_speed(&motor_esquerdo, vel_esq);
}

// Estrutura de callbacks da plataforma customizada
static uni_platform_custom_t plataforma = {
    .on_connected    = on_connected,
    .on_disconnected = on_disconnected,
    .on_controller_data = on_controller_data,
};

void inicializar_bluetooth(void) {
    uni_platform_custom_register(&plataforma);
    printf("[BT] Aguardando controle Xbox...\n");
}

void processar_bluetooth(void) {
    // No v4.2 o bluepad32 roda em task própria
    // não precisa de update manual
}*/