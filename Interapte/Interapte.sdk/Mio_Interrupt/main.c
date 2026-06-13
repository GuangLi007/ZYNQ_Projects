#include "xparameters.h"      // 硬件参数定义（器件ID、基地址等）
#include "xgpiops.h"          // PS端GPIO驱动
#include "xscugic.h"          // 中断控制器驱动（SCU GIC）
#include "xil_exception.h"    // 异常处理（中断使能等）
#include "xplatform_info.h"   // 平台信息
#include <xil_printf.h>       // 串口打印
#include "sleep.h"            // 延时函数（usleep）

// 正确的宏定义（注意是 XGPIOPS，不是 XGPI0PS）
#define GPIO_DEVICE_ID      XPAR_XGPIOPS_0_DEVICE_ID      // PS端GPIO器件ID
#define INTC_DEVICE_ID      XPAR_SCUGIC_SINGLE_DEVICE_ID  // 通用中断控制器ID
#define GPIO_INTERRUPT_ID   XPAR_XGPIOPS_0_INTR           // PS端GPIO中断ID

#define KEY1                9        // KEY1连接到MIO9
#define LED1                0        // LED1连接到MIO0（根据您的定义）

// 全局变量
XGpioPs Gpiops;      // GPIO实例
XScuGic intc;        // 中断控制器实例
u32 key_flag;        // 按键中断标志
u32 key_val;         // 控制LED亮灭（0=灭，1=亮）

// 函数声明
static void intr_handler(void *callback_ref);
int init_interrupt(XScuGic *gic_ins_ptr, XGpioPs *gpio, u16 GpioIntrId);

// ==================== 主函数 ====================
int main(void)
{
    int status;
    XGpioPs_Config *ConfigPtr;

    xil_printf("mio interrupt \r\n");

    // 步骤1：通过查找GPIO驱动器ID加载驱动器并初始化GPIO
    ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
    if (ConfigPtr == NULL) {
        return XST_FAILURE;
    }
    XGpioPs_CfgInitialize(&Gpiops, ConfigPtr, ConfigPtr->BaseAddr);

    // 步骤2：设置KEY所连接的MIO方向为输入，LED所连接的MIO方向为输出并使能输出
    XGpioPs_SetDirectionPin(&Gpiops, KEY1, 0);          // 0=输入
    XGpioPs_SetDirectionPin(&Gpiops, LED1, 1);          // 1=输出
    XGpioPs_SetOutputEnablePin(&Gpiops, LED1, 1);       // 使能LED输出
    XGpioPs_WritePin(&Gpiops, LED1, 1);                 // 初始LED点亮

    // 步骤3-6：初始化中断控制器、设置并使能中断、中断处理函数、中断类型和触发方式
    status = init_interrupt(&intc, &Gpiops, GPIO_INTERRUPT_ID);
    if (status != XST_SUCCESS) {
        xil_printf("Setup interrupt system failed\r\n");
        return XST_FAILURE;
    }

    // 主循环：等待中断发生并处理
    while (1) {
        // 步骤7：处理中断事件，反转LED电平
        if (key_flag) {                     // 检测到中断标志
            usleep(20000);                  // 延时消抖
            if (XGpioPs_ReadPin(&Gpiops, KEY1) == 0) {   // 确认按键按下（低电平有效）
                key_val = ~key_val;         // 反转LED状态
                XGpioPs_WritePin(&Gpiops, LED1, key_val);
            }
            key_flag = 0;                   // 清除中断标志
            XGpioPs_IntrClearPin(&Gpiops, KEY1);    // 清除GPIO中断标志
            XGpioPs_IntrEnablePin(&Gpiops, KEY1);   // 重新使能KEY中断
        }
    }
    return XST_SUCCESS;
}

// ==================== 中断处理函数 ====================
// 步骤7：处理中断事件（读取中断状态，设置标志，屏蔽中断）
static void intr_handler(void *callback_ref)
{
    XGpioPs *gpio = (XGpioPs *)callback_ref;

    // 检查KEY1引脚的中断状态
    if (XGpioPs_IntrGetStatusPin(gpio, KEY1)) {
        key_flag = 1;                       // 设置中断标志，通知主循环处理
        XGpioPs_IntrDisablePin(gpio, KEY1); // 临时屏蔽KEY中断，避免重复触发
    }
}

// ==================== 中断系统初始化函数 ====================
// 步骤3：查找并初始化中断控制器
// 步骤4：设置并使能中断
// 步骤5：设置中断处理函数
// 步骤6：设置中断类型为GPIO中断，触发方式为下降沿中断，使能中断源
int init_interrupt(XScuGic *gic_ins_ptr, XGpioPs *gpio, u16 GpioIntId)
{
    int status;
    XScuGic_Config *IntcConfig;

    // 步骤3：查找中断控制器配置信息并初始化中断控制器
    IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
    if (NULL == IntcConfig) {
        return XST_FAILURE;
    }
    status = XScuGic_CfgInitialize(gic_ins_ptr, IntcConfig, IntcConfig->CpuBaseAddress);
    if (status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    // 步骤4：设置并使能中断（注册异常处理函数，使能ARM处理器中断）
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
                                 (Xil_ExceptionHandler)XScuGic_InterruptHandler,
                                 gic_ins_ptr);
    Xil_ExceptionEnable();

    // 步骤5：设置中断处理函数（将GPIO中断连接到自定义的intr_handler）
    status = XScuGic_Connect(gic_ins_ptr, GpioIntId,
                             (Xil_ExceptionHandler)intr_handler,
                             (void *)gpio);
    if (status != XST_SUCCESS) {
        return status;
    }

    // 步骤4（续）：使能中断控制器中对应的中断ID
    XScuGic_Enable(gic_ins_ptr, GpioIntId);

    // 步骤6：设置中断类型为GPIO中断，触发方式为下降沿中断，使能中断源
    XGpioPs_SetIntrTypePin(gpio, KEY1, XGPIOPS_IRQ_TYPE_EDGE_FALLING);
    XGpioPs_IntrEnablePin(gpio, KEY1);    // 使能KEY1的GPIO中断源

    return XST_SUCCESS;
}
