#ifndef ROBOT_TASK_H_
#define ROBOT_TASK_H_

#include <stdint.h>

/**
*/


void robot_logic_task(void *pvParameters);

/**
 * @brief Inicializa o modo atual
 */
void robot_init(void);

#endif /* ROBOT_TASK_H_ */