# ZYNQ_Projects

Zynq-7020 (Z7-Lite) FPGA 开发工程集合。

## 工程列表

| # | 工程 | 说明 |
|---|------|------|
| 1 | Hello_World | Vivado 硬件工程（最小 Zynq PS 配置，UART0 MIO14-15，115200） |
| 2 | Hello_Vitis | Vitis 裸机 Hello World 应用（含 uart_init 手动配置 MIO 修复） |
| 3 | MIO_Key | Vivado 工程 — 使用 MIO 按键控制 PS LED |

## 工具链

- Vivado / Vitis 2025.2
- 芯片: xc7z020clg400-2
- 板卡: 微相 Z7-Lite（FT232HL JTAG + CH340E UART）

## 笔记

对应学习笔记参见 [ZYNQ_Notes](https://github.com/GuangLi007/ZYNQ_Notes)。
