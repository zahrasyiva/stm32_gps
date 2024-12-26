#include "main.h"
#include <string.h>

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
char gpsBuffer[100];
char gpsData[100];
int relayState = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
void parseGPSData(char* buffer);
void toggleRelay(int state);

/* Main program */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART1_UART_Init();

  /* Infinite loop */
  while (1)
  {
    if (HAL_UART_Receive(&huart1, (uint8_t *)gpsBuffer, sizeof(gpsBuffer), 1000) == HAL_OK)
    {
      // Copy received data to gpsData
      strcpy(gpsData, gpsBuffer);
      // Parse GPS data
      parseGPSData(gpsData);
    }
  }
}

/* Function to parse GPS data */
void parseGPSData(char* buffer)
{
  if (strstr(buffer, "$GPGGA"))
  {
    // Example: Check if GPS data is valid (this is very basic)
    if (strstr(buffer, ",1,"))
    {
      // Valid GPS fix, turn on the relay
      toggleRelay(1);
    }
    else
    {
      // No valid GPS fix, turn off the relay
      toggleRelay(0);
    }
  }
}

/* Function to toggle relay */
void toggleRelay(int state)
{
  if (state != relayState)
  {
    relayState = state;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
  }
}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* GPIO init function */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);

  /* Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* System Clock Configuration */
void SystemClock_Config(void)
{
  // Configuration code (depends on your specific MCU and clock settings)
}

/* Error Handler */
void Error_Handler(void)
{
  while (1)
  {
    // Stay here
  }
}