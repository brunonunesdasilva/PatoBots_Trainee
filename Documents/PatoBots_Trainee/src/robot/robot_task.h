#ifndef ROBOT_TASK_H_
#define ROBOT_TASK_H_

#include "motor.h"

/**
 * @brief Obtém a configuração do motor direito.
 */
motor_config_t *get_motor_direito(void);

/**
 * @brief Obtém a configuração do motor esquerdo.
 */
motor_config_t *get_motor_esquerdo(void);

#endif /* ROBOT_TASK_H_ */
