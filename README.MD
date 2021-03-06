## iot security 基础工程

### 概述
基础工程将使用mebdtls作为物联网通信安全套件，描述mbedtls在物联网设备上的使用方法及安全通信流程.    
![coaps.png](http://note.youdao.com/yws/public/resource/f443655aca2d1be028dbdab40c3ba9a8/xmlnote/95F6A04A16494F2BB96AAF49C7192B45/11567)    


## 工程目录结构
```
├── app
│   ├── net
│   ├── lwm2m
│   ├── mbedtls
│   └── main.c
├── doc
├── drivers
│   ├── bsp
│   ├── cmsis
│   └── stm32f7xx_hal_driver
├── external
│   ├── coap
│   ├── fatfs
│   ├── libcoap
│   ├── lwip
│   ├── wakaama
│   └── mbedtls
├── freertos
│   ├── license
│   └── source
├── project
└── samples
    ├── posix
    ├── stm32
    └── node
```
`app` - 网络应用及主程序入口      
`doc` - 设计文档    
`drivers` - 驱动代码    
`eternal` - 第三方库文件      
`freertos` - `freertos` 源代码     
`project` - 工程文件(`iar` / `gcc`)     
`samples` - 应用示例(`posix` / `stm32`)     


### 开发环境
- Windows: IAR + STLink 

