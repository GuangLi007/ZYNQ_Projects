#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xgpiops.h"

#define MIO_LED1 0
#define MIO_KEY1 9

#define input  0
#define output 1

XGpioPs Gpios;

int main()
{
    int Status;
    XGpioPs_Config *ConfigPtr;

    init_platform();

    print("MIO Test! \n\r");

    // SDK 2018.3 中这个宏是存在的，不需要改成0
    ConfigPtr = XGpioPs_LookupConfig(XPAR_XGPIOPS_0_DEVICE_ID);
    Status = XGpioPs_CfgInitialize(&Gpios, ConfigPtr, ConfigPtr->BaseAddr);
    if (Status != XST_SUCCESS){
        return XST_FAILURE;
    }

    XGpioPs_SetDirectionPin(&Gpios, MIO_LED1, output);
    XGpioPs_SetDirectionPin(&Gpios, MIO_KEY1, input);
    XGpioPs_SetOutputEnablePin(&Gpios, MIO_LED1, 1);

    while(1)
    {
        if(XGpioPs_ReadPin(&Gpios, MIO_KEY1))
        {
            XGpioPs_WritePin(&Gpios, MIO_LED1, 1);
        }
        else
        {
            XGpioPs_WritePin(&Gpios, MIO_LED1, 0);
        }
    }

    cleanup_platform();
    return XST_SUCCESS;
}
