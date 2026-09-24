cmake_minimum_required(VERSION 3.16)

# 1. Defina os caminhos dos componentes ANTES do include
set(EXTRA_COMPONENT_DIRS 
    components/bluepad32/src/components/bluepad32
    components/bluepad32/src/components
    components/bluepad32/external/btstack/port/esp32/components
)

# 2. Agora o include que processa esses caminhos
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
# 3. O nome do seu projeto
project(app-template)