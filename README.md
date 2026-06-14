# ZYNQ_Projects

Zynq-7020 (Z7-Lite) FPGA 开发工程集合。

## 仓库结构

```
ZYNQ_Projects/
├── .gitignore
├── README.md
├── Hello_World/        # Vivado 硬件工程
│   ├── Hello_Vitis/    # Vitis 裸机应用
│   └── ...
├── Interapte/           # 中断工程（MIO 按键中断控制 LED）
│   └── ...
├── MIO_Key/            # MIO 按键工程
│   └── ...
└── EMIO_Key/           # EMIO 按键工程（通过 PL GPIO）
    ├── EMIO_Vitis/     # Vitis 裸机应用源码
    └── System_wrapper.xsa
```

每个工程目录完整自包含，根目录不存放任何工程文件。

## 工程列表

| # | 工程 | 说明 |
|---|------|------|
| 1 | Hello_World | Vivado 硬件工程（最小 Zynq PS 配置，UART0 MIO14-15，115200） |
| 2 | Hello_Vitis | Vitis 裸机 Hello World 应用，位于 `Hello_World/Hello_Vitis/` |
| 3 | Interapte | Vivado 工程 — MIO 按键中断控制 PS LED（含 Vitis 裸机应用） |
| 4 | MIO_Key | Vivado 工程 — 使用 MIO 按键控制 PS LED |
| 5 | EMIO_Key | Vivado 工程 — 使用 EMIO GPIO 控制 PL LED/按键（P15/P16） |

## 工具链

- Vivado / Vitis 2025.2
- 芯片: xc7z020clg400-2
- 板卡: 微相 Z7-Lite（FT232HL JTAG + CH340E UART）

## 笔记

对应学习笔记参见 [ZYNQ_Notes](https://github.com/GuangLi007/ZYNQ_Notes)。
