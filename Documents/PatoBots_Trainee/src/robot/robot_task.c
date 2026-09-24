#include "robot_task.h"
#include "config.h"

static motor_config_t motor_direito = {
    .pin_in1 = PIN_MOTOR_RIGHT_IN1,
    .pin_in2 = PIN_MOTOR_RIGHT_IN2,
    .pin_pwm = PIN_MOTOR_RIGHT_PWM,
    .pwm_channel = MOTOR_RIGHT_CANAL,
};

static motor_config_t motor_esquerdo = {
    .pin_in1 = PIN_MOTOR_LEFT_IN1,
    .pin_in2 = PIN_MOTOR_LEFT_IN2,
    .pin_pwm = PIN_MOTOR_LEFT_PWM,
    .pwm_channel = MOTOR_LEFT_CANAL,
};

motor_config_t *get_motor_direito(void) {
    return &motor_direito;
}

motor_config_t *get_motor_esquerdo(void) {
    return &motor_esquerdo;
}
