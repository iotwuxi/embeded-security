## iot security sample on stm32f767 platform   
********************************************
初始版本工程，示例代码为一个HTTP Server工程实例，由STM32F7Cube生成，重新整理了目录结构，加入了Linux 下 gcc工程，IAR/GCC 可编译通过，未使用开发板调试. 
工程支持Windows/Linux下开发：
- Linux —— GDB + STLink
- Windows —— IAR(7.30以上) + STLink

## 目录结构
```
.
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
├── include
│   └── README.md
├── project
│   ├── gcc
│   └── iar
├── README.md
└── tests
	└── README.md
```
### Windows
```
使用IAR工具打开 /project/iar/security.eww 工程文件
```
## Linux
```
$ cd project/gcc
$ make -s
	security.elf  :
	section                size         addr
	.isr_vector           0x1f8    0x8000000
	.text               0x1711c    0x8000200
	.rodata              0x9960    0x8017320
	.ARM                    0x8    0x8020c80
	.init_array             0x8    0x8020c88
	.fini_array             0x4    0x8020c90
	.data                 0x9cc   0x20000000
	.bss                 0xcdc4   0x200009cc
	._user_heap_stack    0x1800   0x2000d790
	.ARM.attributes        0x30          0x0
	.debug_frame         0x8b00          0x0
	.debug_info         0x261ff          0x0
	.debug_abbrev        0x5c70          0x0
	.debug_loc          0x10049          0x0
	.debug_aranges       0x1e30          0x0
	.debug_ranges        0x1bf0          0x0
	.debug_line         0x104e1          0x0
	.debug_str           0x9f44          0x0
	.comment               0x6e          0x0
	.RxDecripSection       0x80   0x20020000
	.TxDescripSection      0x80   0x20020080
	.RxarraySection      0x17d0   0x2007c000
	.TxarraySection      0x17d0   0x2007d7d0
	Total               0x95553

```

