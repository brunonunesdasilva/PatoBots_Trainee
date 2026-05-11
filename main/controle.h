#ifndef CONTROLE_H_
#define CONTROLE_H_

#include <stdbool.h>

typedef enum {
    MODO_RC,
    MODO_AUTONOMO
} ModoRobo;

extern ModoRobo modo_atual;

void inicializar_bluetooth(void);
void processar_bluetooth(void);

#endif /* CONTROLE_H_ */