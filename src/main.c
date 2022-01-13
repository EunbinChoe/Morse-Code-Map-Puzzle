// Sample code for ECE 198

// Written by Bernie Roehl, August 2021

// This file contains code for a number of different examples.
// Each one is surrounded by an #ifdef ... #endif block inside of main().

// To run a particular example, you should remove the comment (//) in
// front of exactly ONE of the following lines:

#define MAIN
//#define testLED

#include <stdbool.h> // booleans, i.e. true and false
#include <stdio.h>   // sprintf() function
#include <stdlib.h>  // srand() and random() functions

#include "ece198.h"

uint32_t const t_dash = 400; // dash length = 0.4sec
int const dot = 1;           // value in array
int const dash = 2;          // ''

char random_letter(int n);
int dot_dash(uint32_t t_i, uint32_t t_f);
char which_letter(int letter[5]);

int main(void)
{
    HAL_Init(); // initialize the Hardware Abstraction Layer

    // Peripherals (including GPIOs) are disabled by default to save power, so we
    // use the Reset and Clock Control registers to enable the GPIO peripherals that we're using.

    __HAL_RCC_GPIOA_CLK_ENABLE(); // enable port A (for the on-board LED, for example)
    __HAL_RCC_GPIOB_CLK_ENABLE(); // enable port B (for the rotary encoder inputs, for example)
    __HAL_RCC_GPIOC_CLK_ENABLE(); // enable port C (for the on-board blue pushbutton, for example)

    // initialize the pins to be input, output, alternate function, etc...

    // InitializePin(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); // on-board LED
    InitializePin(GPIOA, GPIO_PIN_0, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); // Austrailia
    InitializePin(GPIOA, GPIO_PIN_1, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); // Russia
    InitializePin(GPIOA, GPIO_PIN_4, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); // Denmark
    InitializePin(GPIOB, GPIO_PIN_0, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); // Egypt
    InitializePin(GPIOC, GPIO_PIN_1, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); // Brazil
    InitializePin(GPIOC, GPIO_PIN_0, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); // US
    InitializePin(GPIOA, GPIO_PIN_8, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); // Canada

    InitializePin(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); // initiation light

    // note: the on-board pushbutton is fine with the default values (no internal pull-up resistor
    // is required, since there's one on the board)

    // set up for serial communication to the host computer
    // (anything we write to the serial port will appear in the terminal (i.e. serial monitor) in VSCode)

    SerialSetup(9600);

    // as mentioned above, only one of the following code sections will be used
    // (depending on which of the #define statements at the top of this file has been uncommented)

#ifdef MAIN
    // while (initiation button);
    while (true)
    {
        int i = 0;
        int letter[5] = {5, 5, 5, 5, 5};

        uint32_t t_i = 0;
        uint32_t t_f = 0;
        uint32_t pause = 0;

        // srand(HAL_GetTick());
        int num = (rand() % (7) + 1);
        char output_letter = random_letter(num);

        while (i < 5)
        {
            // check pause time period between last button pressed and until the next press
            while (t_f != 0 && pause <= 3000)
            {
                if (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))
                {
                    break;
                }
                else
                {
                    pause = HAL_GetTick() - t_f;
                }
            }

            if (pause > 1500) // pause time is longer than 1.5sec
            {
                break;
            }

            while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))
                ;
            t_i = HAL_GetTick();

            while (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))
                ;
            t_f = HAL_GetTick();

            letter[i] = dot_dash(t_i, t_f);
            ++i;
        }

        // rest of the array elements
        for (; i < 5; ++i)
        {
            letter[i] = 0;
        }

        if (output_letter == which_letter(letter))
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0x02);
            break;
        }
        else
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0x04);
        }
    }

    // turn off all LED
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, false);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, false);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, false);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, false);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, false);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, false);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, false);

    HAL_Delay(500);

    // turn on READ LEDs' for next hint
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, true);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, true);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, true);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, true);

#endif

#ifdef testLED

    while (true)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, true);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, true);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, true);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, true);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, true);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, true);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, true);
    }

#endif

    return 0;
}

// randomly output LEDs'
char random_letter(int n)
{
    // turn off all LEDs'
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, false);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, false);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, false);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, false);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, false);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, false);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, false);

    if (n == 1)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, true);
        return 'A';
    }
    else if (n == 2)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, true);
        return 'R';
    }
    else if (n == 3)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, true);
        return 'D';
    }
    else if (n == 4)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, true);
        return 'E';
    }
    else if (n == 5)
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, true);
        return 'B';
    }
    else if (n == 6)
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, true);
        return 'U';
    }
    else if (n == 7)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, true);
        return 'C';
    }
    return 'X';
}

// return true when the button pressed is dash false when dot
int dot_dash(uint32_t t_i, uint32_t t_f)
{
    uint32_t delta_t = t_f - t_i;
    char buff[100];

    if (delta_t < t_dash)
    {
        sprintf(buff, "SHORT \n");
        SerialPuts(buff);
        return dot;
    }
    else
    {
        sprintf(buff, "LONG \n");
        SerialPuts(buff);
        return dash;
    }
}

// match input with its alphabet
char which_letter(int letter[5])
{
    // A ._
    if (letter[0] == dot && letter[1] == dash && letter[2] == 0)
    {
        return 'A';
    }
    // B _...
    else if (letter[0] == dash && letter[1] == dot && letter[2] == dot && letter[3] == dot && letter[4] == 0)
    {
        return 'B';
    }
    // C _._.
    else if (letter[0] == dash && letter[1] == dot && letter[2] == dash && letter[3] == dot && letter[4] == 0)
    {
        return 'C';
    }
    // D _..
    else if (letter[0] == dash && letter[1] == dot && letter[2] == dot && letter[3] == 0)
    {
        return 'D';
    }
    // E .
    else if (letter[0] == dot && letter[1] == 0)
    {
        return 'E';
    }
    // R ._.
    else if (letter[0] == dot && letter[1] == dash && letter[2] == dot && letter[3] == 0)
    {
        return 'R';
    }
    // U .._
    else if (letter[0] == dot && letter[1] == dot && letter[2] == dash && letter[3] == 0)
    {
        return 'U';
    }
    // incorrect
    else
    {
        return 'X';
    }
}

// This function is called by the HAL once every millisecond
void SysTick_Handler(void)
{
    HAL_IncTick(); // tell HAL that a new tick has happened
    // we can do other things in here too if we need to, but be careful
}
