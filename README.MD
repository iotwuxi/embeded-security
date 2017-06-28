## iot security 基础工程

### 概述
基础工程将使用mebdtls作为物联网通信安全套件，描述mbedtls在物联网设备上的使用方法及安全通信流程.    
![coaps.png](http://note.youdao.com/yws/public/resource/f443655aca2d1be028dbdab40c3ba9a8/xmlnote/95F6A04A16494F2BB96AAF49C7192B45/11567)    


## 工程目录结构
```
├── app
│   ├── blink
│   └── http-server
├── doc
│   └── README.md
├── drivers
│   ├── bsp
│   ├── cmsis
│   └── stm32f7xx_hal_driver
├── external
│   ├── fatfs
│   ├── lwip
│   └── mbedtls
├── freertos
│   ├── license
│   └── source
├── project
│   ├── gcc
│   └── iar
├── README.md
└── tests
	└── README.md
```
- app - 应用示例
- doc - 设计文档
- drivers - 驱动代码
- eternal - 第三方库文件
- freertos - rtos源代码
- project - 工程文件(IAR / GCC)
- tests - 测试用例，包含posix 示例代码


### 开发环境
- Linux: GDB + STLink 
- Windows: IAR + STLink  

### 软件库
- [Mbedtls](https://github.com/ARMmbed/mbedtls)
- Lwip
- coap
- cJson
