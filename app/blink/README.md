### 最简 freertos 呼吸灯示例工程

```
# 编译
$ cd project/gcc
$ make -f Makefile.blink -s

# 下载到开发板
$ st-flash write blink.bin 0x08000000

# 现象
- 蓝色 LED 一秒闪烁一次.
- 串口每隔一秒输出 "blink task.".
```

