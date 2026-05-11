# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS "main.c" "sensor_distancia.c" "motor.c" "controle.c" "my_platform.c"
    INCLUDE_DIRS "."
        	"../components/bluepad32/external/btstack/port/esp32/components/btstack"
            "../components/bluepad32/external/btstack/port/esp32/components/btstack/include"
    REQUIRES esp_adc esp_driver_ledc esp_driver_gpio bluepad32
)
