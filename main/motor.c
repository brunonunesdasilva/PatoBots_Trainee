/*#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "motor.h"
#include "pinos.h"

#define PWM_FREQ_HZ    20000
#define PWM_RESOLUCAO  LEDC_TIMER_8_BIT
#define PWM_MODO       LEDC_LOW_SPEED_MODE
#define CANAL_DIREITA  LEDC_CHANNEL_0
#define CANAL_ESQUERDA LEDC_CHANNEL_1

static void configurar_gpio_saida(gpio_num_t pino) {
    gpio_reset_pin(pino);
    gpio_set_direction(pino, GPIO_MODE_OUTPUT);
}

static void set_dir_direita(int direcao) {
    if (direcao == 1) {
        gpio_set_level(PIN_MOTOR_RIGHT_IN1, 1);  // <- PIN_ na frente
        gpio_set_level(PIN_MOTOR_RIGHT_IN2, 0);
    } else if (direcao == -1) {
        gpio_set_level(PIN_MOTOR_RIGHT_IN1, 0);
        gpio_set_level(PIN_MOTOR_RIGHT_IN2, 1);
    } else {
        gpio_set_level(PIN_MOTOR_RIGHT_IN1, 0);
        gpio_set_level(PIN_MOTOR_RIGHT_IN2, 0);
    }
}

static void set_dir_esquerda(int direcao) {
    if (direcao == 1) {
        gpio_set_level(PIN_MOTOR_LEFT_IN1, 1);   // <- PIN_ na frente
        gpio_set_level(PIN_MOTOR_LEFT_IN2, 0);
    } else if (direcao == -1) {
        gpio_set_level(PIN_MOTOR_LEFT_IN1, 0);
        gpio_set_level(PIN_MOTOR_LEFT_IN2, 1);
    } else {
        gpio_set_level(PIN_MOTOR_LEFT_IN1, 0);
        gpio_set_level(PIN_MOTOR_LEFT_IN2, 0);
    }
}

void inicializar_motores(void) {
    ledc_timer_config_t timer = {
        .speed_mode      = PWM_MODO,
        .timer_num       = LEDC_TIMER_0,
        .duty_resolution = PWM_RESOLUCAO,
        .freq_hz         = PWM_FREQ_HZ,
        .clk_cfg         = LEDC_AUTO_CLK,
    };
    ledc_timer_config(&timer);

    ledc_channel_config_t ch_dir = {
        .speed_mode = PWM_MODO,
        .channel    = CANAL_DIREITA,
        .timer_sel  = LEDC_TIMER_0,
        .gpio_num   = PIN_MOTOR_RIGHT_PWM,        // <- PIN_ na frente
        .duty       = 0,
        .hpoint     = 0,
    };
    ledc_channel_config(&ch_dir);

    ledc_channel_config_t ch_esq = {
        .speed_mode = PWM_MODO,
        .channel    = CANAL_ESQUERDA,
        .timer_sel  = LEDC_TIMER_0,
        .gpio_num   = PIN_MOTOR_LEFT_PWM,         // <- PIN_ na frente
        .duty       = 0,
        .hpoint     = 0,
    };
    ledc_channel_config(&ch_esq);

    configurar_gpio_saida(PIN_MOTOR_RIGHT_IN1);   // <- PIN_ na frente
    configurar_gpio_saida(PIN_MOTOR_RIGHT_IN2);
    configurar_gpio_saida(PIN_MOTOR_LEFT_IN1);
    configurar_gpio_saida(PIN_MOTOR_LEFT_IN2);

    configurar_gpio_saida(PIN_STBY);
    gpio_set_level(PIN_STBY, 1);
}

void mover_motor_direita(int velocidade, int direcao) {
    if (velocidade < 0)   velocidade = 0;
    if (velocidade > 255) velocidade = 255;
    set_dir_direita(direcao);
    ledc_set_duty(PWM_MODO, CANAL_DIREITA, velocidade);
    ledc_update_duty(PWM_MODO, CANAL_DIREITA);
}

void mover_motor_esquerda(int velocidade, int direcao) {
    if (velocidade < 0)   velocidade = 0;
    if (velocidade > 255) velocidade = 255;
    set_dir_esquerda(direcao);
    ledc_set_duty(PWM_MODO, CANAL_ESQUERDA, velocidade);
    ledc_update_duty(PWM_MODO, CANAL_ESQUERDA);
}

void parar(void) {
    mover_motor_direita(0, 0);
    mover_motor_esquerda(0, 0);
}

void avancar(int velocidade) {
    mover_motor_direita(velocidade, 1);
    mover_motor_esquerda(velocidade, 1);
}

void recuar(int velocidade) {
    mover_motor_direita(velocidade, -1);
    mover_motor_esquerda(velocidade, -1);
}

void girar_direita(int velocidade) {
    mover_motor_direita(velocidade, -1);
    mover_motor_esquerda(velocidade, 1);
}

void girar_esquerda(int velocidade) {
    mover_motor_direita(velocidade, 1);
    mover_motor_esquerda(velocidade, -1);
}*/

#include "motor.h"

// Inicializa o pino STBY global da ponte H
void motor_init_stby(gpio_num_t pin_stby) {
    gpio_set_direction(pin_stby, GPIO_MODE_OUTPUT);
    // Ativa a ponte H (tira do modo de economia de energia)
    gpio_set_level(pin_stby, 1);
}

// Inicializa o hardware (GPIO e LEDC) para um motor
void motor_init(motor_config_t *motor) {
    // 1. Configuração dos pinos de direção como saída
    gpio_set_direction(motor->pin_in1, GPIO_MODE_OUTPUT);
    gpio_set_direction(motor->pin_in2, GPIO_MODE_OUTPUT);

    // Garante que o motor comece parado
    gpio_set_level(motor->pin_in1, 0);
    gpio_set_level(motor->pin_in2, 0);

    // 2. Configuração do Timer do PWM (aplica-se a todos os canais deste timer)
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = MOTOR_PWM_MODE,
        .timer_num        = MOTOR_PWM_TIMER,
        .duty_resolution  = MOTOR_PWM_RES,
        .freq_hz          = MOTOR_PWM_FREQ,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    // ledc_timer_config é seguro de chamar múltiplas vezes com os mesmos parâmetros
    ledc_timer_config(&ledc_timer);

    // 3. Configuração do Canal PWM específico para este motor
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = MOTOR_PWM_MODE,
        .channel        = motor->pwm_channel,
        .timer_sel      = MOTOR_PWM_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = motor->pin_pwm,
        .duty           = 0, // Inicia em 0%
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);
}

// Lógica de controle de velocidade e direção
void motor_set_speed(motor_config_t *motor, int speed) {
    // Trava de segurança para não estourar a resolução de 10 bits
    if (speed > 1023) speed = 1023;
    if (speed < -1023) speed = -1023;

    if (speed > 0) {
        // Rotação no sentido Horário (Frente)
        gpio_set_level(motor->pin_in1, 1);
        gpio_set_level(motor->pin_in2, 0);
        ledc_set_duty(MOTOR_PWM_MODE, motor->pwm_channel, speed);

    } else if (speed < 0) {
        // Rotação no sentido Anti-horário (Ré)
        gpio_set_level(motor->pin_in1, 0);
        gpio_set_level(motor->pin_in2, 1);
        // O duty cycle deve ser sempre positivo, então invertemos o sinal
        ledc_set_duty(MOTOR_PWM_MODE, motor->pwm_channel, -speed);

    } else {
        // Parada com Freio Motor (Short Brake)
        // Na TB6612FNG, IN1=1 e IN2=1 coloca os terminais do motor em curto,
        // fazendo ele parar instantaneamente (ideal para não escorregar pela inércia).
        gpio_set_level(motor->pin_in1, 1);
        gpio_set_level(motor->pin_in2, 1);
        ledc_set_duty(MOTOR_PWM_MODE, motor->pwm_channel, 0);
    }

    // Aplica as alterações de PWM feitas acima
    ledc_update_duty(MOTOR_PWM_MODE, motor->pwm_channel);
}