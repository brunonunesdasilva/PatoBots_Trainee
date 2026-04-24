#ifndef MOTOR_H
#define MOTOR_H

#include "driver/gpio.h"
#include "driver/ledc.h"

// ============================================================================
// CONFIGURAÇÕES GERAIS DO PWM
// ============================================================================
// Frequência de 5kHz e resolução de 10 bits (0 a 1023)
#define MOTOR_PWM_FREQ       5000              
#define MOTOR_PWM_RES        LEDC_TIMER_10_BIT 
#define MOTOR_PWM_TIMER      LEDC_TIMER_0      
#define MOTOR_PWM_MODE       LEDC_LOW_SPEED_MODE 

// ============================================================================
// ESTRUTURAS DE DADOS
// ============================================================================
/**
 * @brief Estrutura de configuração para um motor na ponte H TB6612FNG.
 * Guarda as definições dos pinos e o canal PWM dedicado ao motor.
 */
typedef struct {
    gpio_num_t pin_in1;
    gpio_num_t pin_in2;
    gpio_num_t pin_pwm;
    ledc_channel_t pwm_channel;
} motor_config_t;

// ============================================================================
// PROTÓTIPOS DAS FUNÇÕES DA BIBLIOTECA
// ============================================================================

/**
 * @brief Inicializa o pino STBY (Standby) da ponte H.
 * Na TB6612FNG, o STBY deve estar em nível ALTO (1) para os motores funcionarem.
 * Como um chip controla dois motores, inicializamos ele separadamente.
 * * @param pin_stby O pino GPIO conectado ao pino STBY da ponte H.
 */
void motor_init_stby(gpio_num_t pin_stby);

/**
 * @brief Inicializa os pinos de controle e o canal PWM para um motor específico.
 * * @param motor Ponteiro para a estrutura com a configuração do motor.
 */
void motor_init(motor_config_t *motor);

/**
 * @brief Controla a direção e velocidade do motor.
 * * @param motor Ponteiro para o motor que será acionado.
 * @param speed Velocidade desejada de -1023 (ré máxima) até 1023 (frente máxima).
 * Se speed == 0, o motor aplica um "short-brake" (freio motor ativo).
 */
void motor_set_speed(motor_config_t *motor, int speed);

#endif // MOTOR_H