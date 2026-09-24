idf_component_register(
    SRCS "main.c"
    	"../src/hardware/motor/motor.c"
        "../src/robot/robot_task.c"
        "../src/bluetooth/my_platform.c"
    INCLUDE_DIRS "."
    	"../src"
       	"../src/utils"
       	"../src/hardware/motor"
       	"../src/robot"
       	"../src/bluetooth"
       	"../components/bluepad32/external/btstack/port/esp32/components/btstack"
		"../components/bluepad32/external/btstack/port/esp32/components/btstack/include"
	REQUIRES esp_driver_ledc esp_driver_gpio bluepad32 driver
)
