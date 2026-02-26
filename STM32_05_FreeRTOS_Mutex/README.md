# 프로젝트 5: FreeRTOS 뮤텍스(Mutex)를 이용한 공유 자원 보호
> 4번 프로젝트에서 발견한 '우선순위 역전' 문제를 뮤텍스로 해결하며 시스템 안정성 확보

![Image](https://github.com/user-attachments/assets/fa3e94c8-e4a5-4052-adde-3c712d8959bf)

---

## 프로젝트 개요 (Project Overview)
이전 4번 프로젝트에서, 우선순위가 다른 여러 작업이 하나의 공유 자원(UART)에 접근할 때 **우선순위 역전(Priority Inversion)**이라는 문제가 발생하는 것을 직접 확인함. (뒤죽박죽 메세지 출력)

이 문제를 해결하기 위해, FreeRTOS의 핵심 동기화 도구인 **뮤텍스(Mutex)**를 처음으로 도입하여 공유 자원을 안전하게 보호하고, 시스템의 안정성과 예측 가능성을 확보하는 것을 목표로 함.

---

## 개발 환경 및 핵심 개념 (Tech Stack & Core Concepts)
* **개발 환경**: STM32CubeIDE, NUCLEO-F401RE
* **핵심 기술**:
    * **뮤텍스 (Mutex)**: 여러 작업이 동시에 접근하면 안 되는 공유 자원을 보호하기 위한 '자물쇠' 역할을 하는 동기화 객체임.
    * **임계 영역 (Critical Section)**: 공유 자원에 접근하는 코드 구간. 이 구간을 뮤텍스로 보호하여 한 번에 하나의 작업만 진입하도록 보장.
    * **우선순위 상속 (Priority Inheritance)**: 낮은 우선순위의 작업이 가진 뮤텍스를 높은 우선순위의 작업이 기다릴 때, 낮은 작업의 우선순위를 일시적으로 높여 '우선순위 역전' 문제를 해결하는 RTOS의 고급 기능. (`osMutexPrioInherit` 속성 사용)
    * **API**: `osMutexNew()`, `osMutexAcquire()`, `osMutexRelease()`

---

## 주요 구현 내용
* **뮤텍스 생성**: `main()` 함수에서 `osMutexPrioInherit` 속성을 활성화하여 `uartMutexHandle` 뮤텍스를 생성.
* **임계 영역 보호**: `HighPrioTask`, `MediumPrioTask`, `LowPrioTask` 세 작업 모두, UART에 메시지를 출력하는 코드(`HAL_UART_Transmit`) 앞뒤를 `osMutexAcquire`와 `osMutexRelease`로 감싸서 보호.

```c
// 모든 Task에 적용된 뮤텍스 보호 로직 예시
void SomeTask(void *argument)
{
  char *msg = "Some message\r\n";
  for(;;)
  {
    // 1. 뮤텍스(열쇠)를 획득할 때까지 기다림.
    if (osMutexAcquire(uartMutexHandle, osWaitForever) == osOK)
    {
      // --- 임계 영역 시작 ---
      // 이 구간은 다른 작업의 방해를 받지 않음.
      HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 100);
      // --- 임계 영역 끝 ---

      // 2. 작업이 끝나면 반드시 뮤텍스(열쇠)를 반납.
      osMutexRelease(uartMutexHandle);
    }
    osDelay(1000);
  }
}