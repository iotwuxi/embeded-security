# 目标
TARGET = dtls-psk-client
# 指定编译器
CC = gcc

# CFLAG包括头文件目录
CFLAGS = -g -Wall -std=gnu99
CFLAGS += -DMBEDTLS_CONFIG_FILE=\"dtls-config.h\"

# 头文件查找路径
INC = -I.

# mbedtls动态链接库
LDFLAGS = 
LDLIBS = -lmbedcrypto -lmbedx509 -lmbedtls
# 源文件
SRC = dtls-psk-client.c

# 源文件编译为目标文件
OBJS = $(SRC:.c=.o)

# 链接为可执行文件
$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

# 清除可执行文件和目标文件
clean:
	rm -f $(OBJS)
	rm -f $(TARGET)

# 编译规则 加入头文件 $@代表目标文件 $< 代表第一个依赖文件
%.o:%.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<