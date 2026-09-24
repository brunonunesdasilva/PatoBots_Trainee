#ifndef CONFIG_H_
#define CONFIG_H_

#include "driver/ledc.h"

// ============================================================================
// CONFIGURAÇÕES GERAIS DO PWM
// ============================================================================
#define MOTOR_PWM_FREQ       5000
#define MOTOR_PWM_RES        LEDC_TIMER_10_BIT
#define MOTOR_PWM_TIMER      LEDC_TIMER_0
#define MOTOR_PWM_MODE       LEDC_LOW_SPEED_MODE

// ============================================================================
// CONFIGURAÇÕES DE PINOS - MOTOR DIREITO
// ============================================================================
#define PIN_MOTOR_RIGHT_PWM     15
#define PIN_MOTOR_RIGHT_IN1     16
#define PIN_MOTOR_RIGHT_IN2     2
#define MOTOR_RIGHT_CANAL       LEDC_CHANNEL_0

// ============================================================================
// CONFIGURAÇÕES DE PINOS - MOTOR ESQUERDO
// ============================================================================
#define PIN_MOTOR_LEFT_PWM      14
#define PIN_MOTOR_LEFT_IN1      26
#define PIN_MOTOR_LEFT_IN2      27
#define MOTOR_LEFT_CANAL        LEDC_CHANNEL_1

// ============================================================================
// CONFIGURAÇÕES GERAIS
// ============================================================================
#define PIN_STBY                17

// ============================================================================
// CONSTANTES DE CONTROLE
// ============================================================================
#define ZONA_MORTA              40

// ============================================================================
// DEPURACAO VIA MONITOR SERIAL
// 1 = imprime no monitor (use para testes de bancada)
// 0 = silencioso (use em competicao para nao perder tempo de CPU)
// ============================================================================
#define DEBUG_PWM       1   // motor.c    -> imprime o PWM de cada motor

#endif /* CONFIG_H_ */
