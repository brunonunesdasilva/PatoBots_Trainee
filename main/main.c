/**
 * main.c — Simulador de Ações do Robô por Máquina de Estados (LE-7701)
 */
/*
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#define PIN_IR_TSOP     27  

// Configurações de tempo sincronizadas com o seu Tsop.hpp do STM32
#define SONY_DELTA          150  
#define SONY_START_PULSE   2400  
#define SONY_BIT1_PULSE    1200  
#define SONY_BIT0_PULSE     600  

static int state_ = 0;
static int32_t temp_data_ = -1;
static int32_t data_ = -1;
static int64_t last_edge_time = 0;
static int32_t pulse_time_ = 0;

static volatile uint32_t comando_recebido = 0;
static volatile bool     comando_pronto   = false;

static bool isStartSignal(int32_t pulse) {
    return (pulse > (SONY_START_PULSE - SONY_DELTA) && pulse < (SONY_START_PULSE + SONY_DELTA));
}

static bool isBit1(int32_t pulse) {
    return (pulse > (SONY_BIT1_PULSE - SONY_DELTA) && pulse < (SONY_BIT1_PULSE + SONY_DELTA));
}

static bool isBit0(int32_t pulse) {
    return (pulse > (SONY_BIT0_PULSE - SONY_DELTA) && pulse < (SONY_BIT0_PULSE + SONY_DELTA));
}

static void resetState(void) {
    state_ = 0;
    temp_data_ = -1;
}

static void processar_comando(uint32_t comando) 
{
    if (!comando_pronto) {
        comando_recebido = comando;
        comando_pronto   = true;
    }
}

static void decodeProtocol(int32_t pulse)
{
    switch (state_)
    {
    case 0: 
        if (isStartSignal(pulse)) {
            state_ = 1;
            temp_data_ = 0;
        }
        break;

    case 1: 
    case 2: 
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        if (isBit0(pulse)) {
            temp_data_ <<= 1;
            state_++;
        }
        else if (isBit1(pulse)) {
            temp_data_ = (temp_data_ << 1) | 1;
            state_++;
        }
        else {
            resetState();
        }
        break;

    case 11: 
        if (isBit0(pulse)) {
            temp_data_ <<= 1;
            data_ = temp_data_;
            processar_comando(data_); 
            resetState();
        }
        else if (isBit1(pulse)) {
            temp_data_ = (temp_data_ << 1) | 1;
            data_ = temp_data_;
            processar_comando(data_); 
            resetState();
        }
        else {
            resetState();
        }
        break;

    default:
        resetState();
        break;
    }
}

static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    int64_t now = esp_timer_get_time();
    int32_t delta_time = (int32_t)(now - last_edge_time);
    last_edge_time = now;

    int pino_nivel = gpio_get_level(PIN_IR_TSOP);

    if (pino_nivel == 1) {
        pulse_time_ = delta_time;
    } 
    else {
        if (delta_time > 15000) { 
            resetState();
        } else {
            decodeProtocol(pulse_time_);
        }
    }
}

void app_main(void)
{
    printf("=== SIMULADOR DE COMBATE IR PRONTO ===\n");

    gpio_config_t io = {
        .pin_bit_mask = (1ULL << PIN_IR_TSOP),
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_ANYEDGE, 
    };
    gpio_config(&io);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(PIN_IR_TSOP, gpio_isr_handler, NULL);

    printf("Aperte os botoes de controle (1, 2, 3 ou 8)...\n\n");

    while (1) {
        if (comando_pronto) {
            printf("\n==========================================\n");
            printf("  SINAL DE COMANDO RECONHECIDO (ID: %lu)\n", (unsigned long)comando_recebido);
            
            // Traduz o ID decimal para a ação em tempo real do robô
            switch (comando_recebido) {
                case 8:
                    printf("  [BOTAO 1 DECTECTADO] -> AÇAO: Preparar piscar led!\n");
                    break;
                case 1032:
                    printf("  [BOTAO 2 DECTECTADO] -> AÇAO: Mudar estratégia para FRENTE TOTAL!\n");
                    break;
                case 520:
                    printf("  [BOTAO 3 DECTECTADO] -> AÇAO: Mudar estratégia para PERCORRER BORDA!\n");
                    break;
                case 1800:
                    printf("  [BOTAO 8 DECTECTADO] -> AÇAO: !!! EMERGENCY STOP !!! (Desliga motores imediatamente)\n");
                    break;
                default:
                    printf("  [BOTAO DESCONHECIDO] -> Nenhuma ação configurada para este ID.\n");
                    break;
            }
            printf("==========================================\n\n");
            
            comando_pronto = false; 
        }
        vTaskDelay(pdMS_TO_TICKS(10)); 
    }
}*/

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#define PIN_IR_TSOP     27  

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define DELTA 16800
#define HI_INI 201600 	//nível alto de inicialização
#define LOW	50400 		// nível baixo dos sinais
#define BIT_0 50400		// nível lógico alto do bit 0
#define BIT_1 100800		// nível lógico alto do bit 1

// Razoes ciclicas do motor
#define d0  00
#define d50 70
#define d70 100//500
#define d100 100//999
#define dVirarParaAtaque 800

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
void Temporizador_ms(unsigned int);		//Configura Timer
void parar(void);
void atacar(void);
void procurar(void);
void re_achou(void);
void virar_D_eixo(void);
void virar_D_pra_atacar(void);
void virar_E_eixo(void);
void virar_E_pra_atacar(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t sf_d,sf_e;
uint16_t ADC_buffer[1]={0};
//Variáveis do receptor

uint32_t riseTime = 0, fallTime = 0;
uint32_t highTime = 0, lowTime = 0;
uint32_t period = 0;
uint16_t duty=50400;

uint16_t estado=0; 		//estado de decodificação
int16_t tmp_data=-1;		// dado lido do controle remoto
int16_t data=-1;

// Dados do autômato (Não pode ser declarado dentro da função main por ser const)
#define NTRANS 33	// Número de Transições
#define NESTADOS 15	// Número de Estados
#define BUFFER 10	// Máximo Número de Eventos no Buffer

const unsigned int event[NTRANS]={12,12,12,13,2,4,13,4,2,11,11,11,8,9,10,4,2,7,6,1,0,13,13,3,5,3,4,7,6,5,2,7,6};
const unsigned int in_state[NTRANS]={5,6,7,9,13,14,9,14,13,4,9,3,0,1,2,14,13,11,10,4,3,3,4,14,13,9,12,11,10,9,12,11,10};
const unsigned int rfirst[NESTADOS] = {1,2,3,6,9,10,11,12,15,21,22,23,25,29,33};
const unsigned int rnext[NTRANS] = {0,0,0,0,4,5,0,7,8,0,0,0,0,13,14,0,16,17,18,19,20,0,0,0,24,0,26,27,28,0,30,31,32};


// Mapeamento de eventos não controláveis como entradas
#define LD 0	// Entrada 0
#define LE 1	// Entrada 1
#define PD_achou 2	// Entrada 2
#define PD_perdeu 3	// Entrada 3
#define PE_achou 4	// Entrada 4
#define PE_perdeu 5	// Entrada 5
#define SF_D 6	// Entrada 6
#define SF_E 7	// Entrada 7

// Mapeamento de eventos controláveis
#define est_1 8
#define est_2 9
#define est_3 10
#define iniciar 11
#define prepara 12
#define tempo 13

// Defini��o dos eventos de sa�da
#define BI_E_ON  HAL_GPIO_WritePin(BI_E_GPIO_Port,BI_E_Pin,GPIO_PIN_SET);	//Saida 0 ON AIN
#define BI_E_OFF  HAL_GPIO_WritePin(BI_E_GPIO_Port,BI_E_Pin,GPIO_PIN_RESET);	//Saida 0 OFF
#define FI_E_ON  HAL_GPIO_WritePin(FI_E_GPIO_Port,FI_E_Pin,GPIO_PIN_SET);	//Saida 0 ON
#define FI_E_OFF  HAL_GPIO_WritePin(FI_E_GPIO_Port,FI_E_Pin,GPIO_PIN_RESET);	//Saida 0 OFF
#define BI_D_ON  HAL_GPIO_WritePin(BI_D_GPIO_Port,BI_D_Pin,GPIO_PIN_SET);	//Saida 0 ON
#define BI_D_OFF  HAL_GPIO_WritePin(BI_D_GPIO_Port,BI_D_Pin,GPIO_PIN_RESET);
#define FI_D_ON  HAL_GPIO_WritePin(FI_D_GPIO_Port,FI_D_Pin,GPIO_PIN_SET);	//Saida 0 ON
#define FI_D_OFF  HAL_GPIO_WritePin(FI_D_GPIO_Port,FI_D_Pin,GPIO_PIN_RESET);
#define LED1_ON  HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);	//Saida 0 ON
#define LED1_OFF HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET);	//Saida 0 OFF
#define LED2_ON  HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET);	//Saida 0 ON
#define LED2_OFF HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_RESET);	//Saida 0 OFF
#define LED3_ON  HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_SET);	//Saida 0 ON
#define LED3_OFF HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_RESET);	//Saida 0 OFF
#define LED4_ON  HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,GPIO_PIN_SET);	//Saida 0 ON
#define LED4_OFF HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,GPIO_PIN_RESET);	//Saida 0 OFF


//Declara��o de vari�veis globais
unsigned char buffer[BUFFER];		//Buffer para armazenar a fila de enventos externos
unsigned char n_buffer=0;		//N�mero de eventos no Buffer

unsigned int k;
int occur_event;			//Evento ocorrido
unsigned int current_state = 8;	//Estado atual inicializado com estado inicial
char g=0; 			//Flag para gerador aleatório de eventos
char gerar_evento=1;			//Flag para habilitar a temporização de eventos controláveis
char moore_output = 0;		//Inicializa saída periférica

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1); //main channel (captura do duty cycle do receptor)
  HAL_TIM_IC_Start(&htim2, TIM_CHANNEL_2); //indirect channel

  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,0);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,0);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,0);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4,0);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_buffer,1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(HAL_GPIO_ReadPin(SF_D_GPIO_Port, SF_D_Pin)) sf_d=1;
	  else sf_d=0;
	  if(HAL_GPIO_ReadPin(SF_E_GPIO_Port, SF_E_Pin)) sf_e=1;
	  else sf_e=0;

	  if(data==520)
		  {
		  	  current_state=8; //estado incial
		  	  moore_output=1;
		  }
	  if(ADC_buffer[0]<2931)
		  {
		  	  current_state=8; //estado incial
		  	  moore_output=1;
		  	  LED4_ON;
		  }
	  else{
		  LED4_OFF;
	  }
		if(n_buffer == 0)//se n�o existir evento no buffer ent�o gerar um evento interno(evento control�vel)
		{
			if(__HAL_TIM_GET_FLAG(&htim1, TIM_FLAG_UPDATE) != RESET)	//Se o timer estourar, habilita a gera��o de eventos
			{
				gerar_evento=1;
			}
			if(gerar_evento==1)
			{
				switch(g)	//Aqui � implementado um gerador autom�tico de eventos control�veis
				{
					case(0):
						occur_event=tempo;
						g=0;
						break;
				}
			}
			switch (data)
			{
				case(776):
						occur_event=est_1;
						data=-1;
				break;
				case(1800):
						occur_event=est_2;
						data=-1;
				break;
				case(136):
						occur_event=est_3;
						data=-1;
				break;
				case(8):
						occur_event=prepara;
						data=-1;
				break;
				case(1032):
						occur_event=iniciar;
						data=-1;
				break;
			}
		}

		else 	//se existir evento n�o control�vel pegar do buffer
		{
			occur_event = buffer[0];
			n_buffer--;
			k = 0;
			while(k<n_buffer)
			{
				buffer[k] = buffer[k+1];
				k+=1;
			}
		}



		//Jogador de aut�mato
		k = rfirst[current_state];
		if(k==0)
		{
			return 1;     //Dead Lock!!!
		}
		else
		{
			while(k>0)
			{
				k-=1;
				if(event[k] == occur_event)
				{
					current_state = in_state[k];
					moore_output = 1;
					break;
				}
				k = rnext[k];
			}
		}

		if(moore_output) //Se o evento ocorrido for v�lido, ent�o imprimir sa�da f�sica
		{
			gerar_evento=1;
			switch(current_state)
			{
				case(0):	//Adicionar Ação para o Estado Espera1;
						LED1_ON;
					break;
				case(1):	//Adicionar Ação para o Estado Espera2;
						LED1_ON;
					break;
				case(2):	//Adicionar Ação para o Estado Espera3;
						LED1_ON;
					break;
				case(3):	//Adicionar Ação para o Estado Gira_d_ini;
						LED1_OFF;
						LED2_OFF;
						virar_D_eixo();
						Temporizador_ms(1000);
						gerar_evento=0;
					break;
				case(4):	//Adicionar Ação para o Estado Gira_e_ini;
								LED1_OFF;
								LED2_OFF;
								virar_E_eixo();
								Temporizador_ms(1000);
								gerar_evento=0;
					break;
				case(5):	//Adicionar Ação para o Estado Ini1;
						LED2_ON;
					break;
				case(6):	//Adicionar Ação para o Estado Ini2;
						LED2_ON;
					break;
				case(7):	//Adicionar Ação para o Estado Ini3;
						LED2_ON;
					break;
				case(8):	//Adicionar Ação para o Estado Inicio;
						LED1_OFF;
						LED2_OFF;
						parar();
					break;
				case(9):	//Adicionar Ação para o Estado Procurar;
						LED1_OFF;
						LED2_OFF;
						procurar();
					break;
				case(10):	//Adicionar Ação para o Estado Re_d_100;
						re_achou();
						Temporizador_ms(600);
						gerar_evento=0;
					break;
				case(11):	//Adicionar Ação para o Estado Re_e_100;
						re_achou();
						Temporizador_ms(600);
						gerar_evento=0;
					break;
				case(12):	//Adicionar Ação para o Estado atacar;
						atacar();
					break;
				case(13):	//Adicionar Ação para o Estado virar_D_pra_atacar;
						virar_D_pra_atacar();
					break;
				case(14):	//Adicionar Ação para o Estado virar_E_pra_atacar;
						virar_E_pra_atacar();
					break;
			}//fim switch
			moore_output = 0;
			occur_event = -1;
		}//fim if(moore_output)

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 8399;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sSlaveConfig.TriggerPrescaler = TIM_ICPSC_DIV1;
  sSlaveConfig.TriggerFilter = 0;
  if (HAL_TIM_SlaveConfigSynchro(&htim2, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 840-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_PLACA_GPIO_Port, LED_PLACA_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED1_Pin|LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED3_Pin|LED4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_PLACA_Pin */
  GPIO_InitStruct.Pin = LED_PLACA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_PLACA_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SF_D_Pin SP_LD_Pin */
  GPIO_InitStruct.Pin = SF_D_Pin|SP_LD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : BOT_PLACA_Pin */
  GPIO_InitStruct.Pin = BOT_PLACA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOT_PLACA_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SP_FD_Pin SP_FE_Pin */
  GPIO_InitStruct.Pin = SP_FD_Pin|SP_FE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SP_LE_Pin SF_E_Pin */
  GPIO_InitStruct.Pin = SP_LE_Pin|SF_E_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED1_Pin LED2_Pin */
  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LED3_Pin LED4_Pin */
  GPIO_InitStruct.Pin = LED3_Pin|LED4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : M_START_Pin */
  GPIO_InitStruct.Pin = M_START_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(M_START_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

//Tratamento das interrupções
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == SP_LD_Pin)  // Sensor de faixa esquerda
	{
		if(HAL_GPIO_ReadPin(SP_LD_GPIO_Port, SP_LD_Pin))
		{
			buffer[n_buffer] = LD;  // Atribuir evento à borda de subida do sensor de faixa esquerda (FE_Pin)
			n_buffer++;
		}
	}
	if (GPIO_Pin == SP_LE_Pin)  // Sensor de faixa esquerda
	{
		if(HAL_GPIO_ReadPin(SP_LE_GPIO_Port, SP_LE_Pin))
		{
			buffer[n_buffer] = LE;  // Atribuir evento à borda de subida do sensor de faixa esquerda (FE_Pin)
			n_buffer++;
		}
	}
	if (GPIO_Pin == SP_FD_Pin)  // Sensor de faixa esquerda
	{
		if(HAL_GPIO_ReadPin(SP_FD_GPIO_Port, SP_FD_Pin))
		{
			buffer[n_buffer] = PD_achou;  // Atribuir evento à borda de subida do sensor de faixa esquerda (FE_Pin)
			n_buffer++;
		}
		else
		{
			buffer[n_buffer] = PD_perdeu;  // Atribuir evento à borda de subida do sensor de faixa esquerda (FE_Pin)
			n_buffer++;
		}

	}
	if (GPIO_Pin == SP_FE_Pin)  // Sensor de faixa esquerda
	{
		if(HAL_GPIO_ReadPin(SP_FE_GPIO_Port, SP_FE_Pin))//ESSA LOGICA É FAIXA DE SUBIDA E DESCIDA PARA MUDAR A ESTRATEGIA, EM SENSOR ANALOGICO PODEMOS USAR VALORES
		{
			buffer[n_buffer] = PE_achou;  // Atribuir evento à borda de subida do sensor de faixa esquerda (FE_Pin)
			n_buffer++;
		}
		else
		{
			buffer[n_buffer] = PE_perdeu;  // Atribuir evento à borda de subida do sensor de faixa esquerda (FE_Pin)
			n_buffer++;
		}

	}

	if (GPIO_Pin == SF_E_Pin)  // Sensor de faixa esquerda
	{
		if(HAL_GPIO_ReadPin(SF_D_GPIO_Port, SF_D_Pin))
		{
			buffer[n_buffer] = SF_D;  // Atribuir evento à borda de subida do sensor de faixa esquerda (FE_Pin)
			n_buffer++;
		}
	}
	if (GPIO_Pin == SF_D_Pin)  // Sensor de faixa direita
	{
		if(HAL_GPIO_ReadPin(SF_E_GPIO_Port, SF_E_Pin))
		{
			buffer[n_buffer] = SF_E;  // Atribuir evento à borda de subida do sensor de faixa direita (FD_Pin)
			n_buffer++;
		}
	}

}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2) // se a interrupção é disparada pelo canal 1
	{
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			riseTime = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); //captura a primeira borda
			if (riseTime != 0) // se é diferente de zero então percorreu um periodo
			{
				period=riseTime;
				// Captura o tempo de borda de descida (final do nível alto, início do nível baixo)
				highTime = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
				// Calcula o tempo de nível baixo (low time)
				lowTime = period - highTime;

				//Neste ponto precisa verificar o Protocolo
				switch (estado)
				{
				   case 0: // não tem nada decodificado está a espera dos bits de início
					   if ((highTime < (HI_INI+DELTA)) && (highTime > (HI_INI-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=1; //achou o periodo de inicialização
					   }
				   break;

				   case 1:
					   if ((highTime < (BIT_0+DELTA)) && (highTime > (BIT_0-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=2; //achou bit 0
						   tmp_data=0;
					   }
					   else if((highTime < (BIT_1+DELTA)) && (highTime > (BIT_1-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=2; //achou bit 1
						   tmp_data=1;
					   }
					   else
					   {
						   estado=0; //estado de erro
						   tmp_data=-1; // Não tem nenhum dado
					   }
				   break;
				   case 2:
					   if ((highTime < (BIT_0+DELTA)) && (highTime > (BIT_0-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=3; //achou bit 0
						   tmp_data=tmp_data<<1;
					   }
					   else if((highTime < (BIT_1+DELTA)) && (highTime > (BIT_1-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=3; //achou bit 1
						   tmp_data=tmp_data<<1;
						   tmp_data|=1;
					   }
					   else
					   {
						   estado=0; //estado de erro
						   tmp_data=-1; // Não tem nenhum dado
					   }
				   break;
				   case 3:
					   if ((highTime < (BIT_0+DELTA)) && (highTime > (BIT_0-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=4; //achou bit 0
						   tmp_data=tmp_data<<1;
					   }
					   else if((highTime < (BIT_1+DELTA)) && (highTime > (BIT_1-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=4; //achou bit 1
						   tmp_data=tmp_data<<1;
						   tmp_data|=1;
					   }
					   else
					   {
						   estado=0; //estado de erro
						   tmp_data=-1; // Não tem nenhum dado
					   }
				   break;
				   case 4:
					   if ((highTime < (BIT_0+DELTA)) && (highTime > (BIT_0-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=5; //achou bit 0
						   tmp_data=tmp_data<<1;
					   }
					   else if((highTime < (BIT_1+DELTA)) && (highTime > (BIT_1-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=5; //achou bit 1
						   tmp_data=tmp_data<<1;
						   tmp_data|=1;
					   }
					   else
					   {
						   estado=0; //estado de erro
						   tmp_data=-1; // Não tem nenhum dado
					   }
				   break;
				   case 5:
					   if ((highTime < (BIT_0+DELTA)) && (highTime > (BIT_0-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=6; //achou bit 0
						   tmp_data=tmp_data<<1;
					   }
					   else if((highTime < (BIT_1+DELTA)) && (highTime > (BIT_1-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=6; //achou bit 1
						   tmp_data=tmp_data<<1;
						   tmp_data|=1;
					   }
					   else
					   {
						   estado=0; //estado de erro
						   tmp_data=-1; // Não tem nenhum dado
					   }
				   break;
				   case 6:
					   if ((highTime < (BIT_0+DELTA)) && (highTime > (BIT_0-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=7; //achou bit 0
						   tmp_data=tmp_data<<1;
					   }
					   else if((highTime < (BIT_1+DELTA)) && (highTime > (BIT_1-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=7; //achou bit 1
						   tmp_data=tmp_data<<1;
						   tmp_data|=1;
					   }
					   else
					   {
						   estado=0; //estado de erro
						   tmp_data=-1; // Não tem nenhum dado
					   }
				   break;
				   case 7:
					   if ((highTime < (BIT_0+DELTA)) && (highTime > (BIT_0-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=8; //achou bit 0
						   tmp_data=tmp_data<<1;
					   }
					   else if((highTime < (BIT_1+DELTA)) && (highTime > (BIT_1-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=8; //achou bit 1
						   tmp_data=tmp_data<<1;
						   tmp_data|=1;
					   }
					   else
					   {
						   estado=0; //estado de erro
						   tmp_data=-1; // Não tem nenhum dado
					   }
				   break;
				   case 8:
					   if ((highTime < (BIT_0+DELTA)) && (highTime > (BIT_0-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=9; //achou bit 0
						   tmp_data=tmp_data<<1;
					   }
					   else if((highTime < (BIT_1+DELTA)) && (highTime > (BIT_1-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=9; //achou bit 1
						   tmp_data=tmp_data<<1;
						   tmp_data|=1;
					   }
					   else
					   {
						   estado=0; //estado de erro
						   tmp_data=-1; // Não tem nenhum dado
					   }
				   break;
				   case 9:
					   if ((highTime < (BIT_0+DELTA)) && (highTime > (BIT_0-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=10; //achou bit 0
						   tmp_data=tmp_data<<1;
					   }
					   else if((highTime < (BIT_1+DELTA)) && (highTime > (BIT_1-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=10; //achou bit 1
						   tmp_data=tmp_data<<1;
						   tmp_data|=1;
					   }
					   else
					   {
						   estado=0; //estado de erro
						   tmp_data=-1; // Não tem nenhum dado
					   }
				   break;
				   case 10:
					   if ((highTime < (BIT_0+DELTA)) && (highTime > (BIT_0-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=11; //achou bit 0
						   tmp_data=tmp_data<<1;
					   }
					   else if((highTime < (BIT_1+DELTA)) && (highTime > (BIT_1-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=11; //achou bit 1
						   tmp_data=tmp_data<<1;
						   tmp_data|=1;
					   }
					   else
					   {
						   estado=0; //estado de erro
						   tmp_data=-1; // Não tem nenhum dado
					   }
				   break;
				   case 11:
					   if ((highTime < (BIT_0+DELTA)) && (highTime > (BIT_0-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=0; //achou bit 0
						   tmp_data=tmp_data<<1;
						   data=tmp_data;
					   }
					   else if((highTime < (BIT_1+DELTA)) && (highTime > (BIT_1-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=0; //achou bit 1
						   tmp_data=tmp_data<<1;
						   tmp_data|=1;
						   data=tmp_data;
					   }
					   else
					   {
						   estado=0; //estado de erro
						   tmp_data=-1; // Não tem nenhum dado
					   }
				   break;
				   /*
				   case 12:
					   if ((highTime < (BIT_0+DELTA)) && (highTime > (BIT_0-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=0; //achou bit 0
						   tmp_data=tmp_data<<1;
						   data=tmp_data;
					   }
					   else if((highTime < (BIT_1+DELTA)) && (highTime > (BIT_1-DELTA)) && (lowTime<(LOW+DELTA)) && (lowTime>(LOW-DELTA)))
					   {
						   estado=0; //achou bit 1
						   tmp_data=tmp_data<<1;
						   tmp_data|=1;
						   data=tmp_data;
					   }
					   else
					   {
						   estado=0; //estado de erro
						   tmp_data=-1; // Não tem nenhum dado
					   }
				   break;
				   */
				   default:
				     //Instruções;
				}
			}

		}

	}
}

//Configura��o de perif�ricos
//--------------------------------------------------------------
//--------------------------------------------------------------
void parar(){
    ///BI_D_OFF;
    //BI_E_OFF;
    //FI_D_OFF;
    //FI_E_OFF;
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4,0);
}
void atacar(){
	//FI_D_ON;
	//FI_E_ON;
	//BI_D_OFF;
	//BI_E_OFF;
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,0);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,999);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,999);

}
void procurar(){//Alterar
	//FI_D_ON;
	//FI_E_ON;
	//BI_D_OFF;
	//BI_E_OFF;
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,0);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,100); //esquerda
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,70);//direita

}
void re_achou(){//Alterar
	//BI_D_ON;
	//BI_E_ON;
	//FI_D_OFF;
	//FI_E_OFF;
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,0);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0);	
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,999);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,999);
}

void virar_D_eixo(){
	//BI_D_ON;
	//BI_E_OFF;
	//FI_D_OFF;
	//FI_E_ON;
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,0);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,300);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,300);
}
void virar_D_pra_atacar(){
	//BI_D_OFF;
	//BI_E_OFF;
	//FI_D_ON;
	//FI_E_ON;
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,0);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,999);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0);
}
void virar_E_eixo(){
	//BI_D_OFF;
	//BI_E_ON;
	//FI_D_ON;
	//FI_E_OFF;
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4,0);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,300);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,300);
}
void virar_E_pra_atacar(){
	//BI_D_OFF;
	//BI_E_OFF;
	//FI_D_ON;
	//FI_E_ON;
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,0);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,0);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,999);
}

void Temporizador_ms(unsigned int TEMPO)
{
	__HAL_TIM_CLEAR_IT(&htim1, TIM_IT_UPDATE);
	__HAL_TIM_SET_AUTORELOAD(&htim1, TEMPO*5);
	HAL_TIM_Base_Start_IT(&htim1);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */