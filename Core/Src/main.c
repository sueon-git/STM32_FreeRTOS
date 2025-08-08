/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>  // sprintf 함수를 사용하기 위해 추가
#include <string.h>  // strlen 함수를 사용하기 위해 추가
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE BEGIN PV */
osMessageQueueId_t ledCmdQueueHandle; // LED 제어 명령을 위한 큐 핸들
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void StartDefaultTask(void *argument);

/* USER CODE BEGIN PFP */
void StartDefaultTask(void *argument); // 이 줄은 그대로 두거나, CubeMX가 관리합니다.
// void LedBlinkTask(void *argument); // 주석 처리
// void SerialPrintTask(void *argument); // 주석 처리
void LedControlTask(void *argument);  // LED 제어 작업 함수 원형
void DataSenderTask(void *argument);  // 데이터 전송 작업 함수 원형
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// 데이터 전송 작업 함수 (새로 추가)
void DataSenderTask(void *argument)
{
  uint32_t command_to_send = 0; // 전송할 명령 (0 또는 1)
  osStatus_t status;

  for(;;)
  {
    command_to_send = 1 - command_to_send; // 0과 1을 번갈아 보냄
    // ledCmdQueueHandle 큐에 command_to_send 값을 넣습니다.
    // 0U: 메시지 우선순위 없음
    // 100: 100ms 타임아웃 (큐가 꽉 차 있으면 100ms 기다림)
    status = osMessageQueuePut(ledCmdQueueHandle, &command_to_send, 0U, 100);

    if (status != osOK)
    {
      // 큐에 메시지를 넣지 못했을 경우 (예: 큐가 꽉 참) 오류 처리 (지금은 생략)
    }
    osDelay(2000); // 2초마다 새로운 명령 전송
  }
}
// LED 제어 작업 함수 (기존 LedBlinkTask에서 이름 변경 및 내용 수정)
void LedControlTask(void *argument)
{
  uint32_t received_command; // 큐에서 받은 명령을 저장할 변수
  osStatus_t status;
  uint32_t current_blink_delay = 500; // 기본 깜빡임 지연 시간 (0.5초)

  for(;;)
  {
    // ledCmdQueueHandle 큐에서 메시지를 기다립니다.
    // &received_command: 받은 메시지를 저장할 변수의 주소
    // NULL: 메시지 우선순위는 신경 쓰지 않음
    // osWaitForever: 메시지가 올 때까지 무한정 기다림
    status = osMessageQueueGet(ledCmdQueueHandle, &received_command, NULL, 0U);

    if (status == osOK) // 메시지를 성공적으로 받았다면
    {
      if (received_command == 1)
      {
        current_blink_delay = 100; // 명령이 1이면 0.1초 (빠르게)
      }
      else // 명령이 0이면
      {
        current_blink_delay = 1000; // 1초 (느리게)
      }
    }
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); // LD2 LED 토글 (기존 코드와 동일)
    osDelay(current_blink_delay); // 계산된 지연 시간만큼 대기
  }
}



/*
// LED 깜빡이는 작업을 수행할 함수
void LedBlinkTask(void *argument)
{
  for(;;) // 무한 루프
  {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); // NUCLEO-F401RE의 LD2 LED (녹색) 토글
    osDelay(100);                         // 0.1초 동안 작업 지연 (RTOS 방식)
  }
}
*/

// 시리얼 포트로 메시지를 출력하는 작업을 수행할 함수

/*
// void SerialPrintTask(void *argument)
{
  uint32_t counter = 0;
  char msg_buffer[50];

  for(;;) // 무한 루프
  {
    sprintf(msg_buffer, "Task 2! Count: %lu\r\n", counter++); // 메시지 만들기

    // UART(USART2)를 통해 메시지 전송
    // huart2 핸들은 main.c 상단 또는 main.h에 선언되어 있어야 하며, MX_USART2_UART_Init()에서 초기화됩니다.
    // CubeMX 설정 시 USART2를 활성화했다면 자동으로 생성됩니다.
    HAL_UART_Transmit(&huart2, (uint8_t*)msg_buffer, strlen(msg_buffer), 100); // 100ms 타임아웃

    osDelay(1000); // 1초(1000ms) 간격으로 메시지 전송
  }
}
*/

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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  // 큐 생성: 최대 5개의 uint32_t 타입 메시지를 담을 수 있는 큐
  ledCmdQueueHandle = osMessageQueueNew(5, sizeof(uint32_t), NULL);
  if (ledCmdQueueHandle == NULL)
  {
    Error_Handler(); // 큐 생성 실패 시 오류 처리
  }
    /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */

  // LedBlinkTask를 위한 속성 정의 및 생성
  const osThreadAttr_t ledControlTask_attributes = {
    .name = "LedControl",
    .stack_size = 128 * 4, // 512 바이트
    .priority = (osPriority_t) osPriorityNormal,
  };
  osThreadNew(LedControlTask, NULL, &ledControlTask_attributes);

  // DataSenderTask를 위한 속성 정의 및 생성
  const osThreadAttr_t dataSenderTask_attributes = {
    .name = "DataSender",
    .stack_size = 128 * 4, // sprintf 등을 사용하므로 충분히 할당 (512 바이트)
    .priority = (osPriority_t) osPriorityNormal,
  };
  osThreadNew(DataSenderTask, NULL, &dataSenderTask_attributes);

  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

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
