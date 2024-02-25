#include <stdint.h>

//PERIPHERAL & BUS BASE ADDRESS
#define PERIPHERAL_BASE_ADDRESS		0x40000000U
#define AHB_BASE_ADDRESS			(PERIPHERAL_BASE_ADDRESS + 0x00020000U)

//RCC BASE ADDRESS
#define RCC_BASE_ADDRESS			(AHB_BASE_ADDRESS + 0x00001000U)
#define RCC_IOPENR_ADDRESS			(RCC_BASE_ADDRESS + 0x0000002CU)

//IOPORT BASE ADDRESS
#define IOPORT_ADDRESS				(PERIPHERAL_BASE_ADDRESS + 0x10000000U)
//GPIO BASE & SPECIFIC ADDRESS
#define GPIOA_BASE_ADDRESS			(IOPORT_ADDRESS + 0x00000000U)
#define GPIOA_MODER_REG				(GPIOA_BASE_ADDRESS + 0x00000000U)
#define GPIOA_ODR_REG				(GPIOA_BASE_ADDRESS + 0x00000014U)

#define GPIOC_BASE_ADDRESS			(IOPORT_ADDRESS + 0x00000800U)
#define GPIOC_MODER_REG				(GPIOC_BASE_ADDRESS + 0x00000000U)
#define GPIOC_ODR_REG				(GPIOC_BASE_ADDRESS + 0x00000014U)
#define GPIOC_IDR_REG				(GPIOC_BASE_ADDRESS + 0x00000010U)
#define GPIOC_PUPD_REG				(GPIOC_BASE_ADDRESS + 0x0000000CU)
#define BIT13						(1 << 13)
#define BIT0						(1 << 0)

void delay_ms(uint16_t n);
int isButtonPressed(uint32_t *ptr_idr, uint32_t bit);

int main(void) {
    uint32_t *ptr_rcc_iopenr = RCC_IOPENR_ADDRESS;  // Pointer variable para iopenr
    uint32_t *ptr_gpioc_moder = GPIOC_MODER_REG;   // Pointer variable to GPIOC, MODER register
    uint32_t *ptr_gpioc_odr = GPIOC_ODR_REG;       // Pointer variable to GPIOC, ODR register
    uint32_t *ptr_gpioc_idr = GPIOC_IDR_REG;       // Pointer variable to GPIOC, ODR register
   // uint32_t *ptr_gpioc_pupd = GPIOC_PUPD_REG;     // Pointer variable to GPIOC, PUPD register


    // GPIOC
    *ptr_rcc_iopenr |= 1 << 2;


    // GPIOC
    // PC9 se define como entrada para el primer boton
    *ptr_gpioc_moder &= ~(1 << 18);
    *ptr_gpioc_moder &= ~(1 << 19);
    // GPIOC
    // PC8 se define como entrada para el segundo boton
    *ptr_gpioc_moder &= ~(1 << 16);
    *ptr_gpioc_moder &= ~(1 << 17);


    // PC0 como la salida para la LED

    *ptr_gpioc_moder &= ~(1 << 1);



    uint8_t contador= 0;
    uint8_t prevButtonState = 0;  // Variable para almacenar el estado anterior del botón

    while (1) {
    	// Verificar si el botón PC8 está presionado (flanco de bajada)
    	uint8_t currentButtonState = (*ptr_gpioc_idr & (1 << 8)) ? 1 : 0;
    	if (currentButtonState == 0 && prevButtonState == 1) {
    	     // Incrementar el contador cuando se detecta el flanco de bajada
    	       contador++;
    	}

        // Verificar si el botón PC13 está presionado
        if (*ptr_gpioc_idr &= ~(1 << 9)) {
            // Parpadear el LED a 200 ms
            *ptr_gpioc_odr ^= (1 << 0);
            delay_ms(200);
        } else {
            // Parpadear el LED a 500 ms
            *ptr_gpioc_odr ^= (1 << 0);
            delay_ms(500);
        }


    }

    return 0;
}

void delay_ms(uint16_t n) {
    uint16_t i;
    for (; n > 0; n--)
        for (i = 0; i < 140; i++)
            ;
}
