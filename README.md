# LFTK针对STM32f103ze的移植。

[LFTK](https://github.com/xianjimli/lftk)是针对低端嵌入式设备开发的嵌入式GUI库。lftk-stm32f103ze-raw是LFTK在STM32f103ze上的移植。

第一版以[普中科技STM32F103ZET6开发实验板](https://item.taobao.com/item.htm?spm=a230r.1.14.1.50a130e8TMKYMC&id=558855281660&ns=1&abbucket=5#detail) 为载体移植(该开发板性价比不错，推荐一下:))。

欢迎有兴趣的朋友移植到各个RTOS，如果在移植时需要开发板支持或有其它疑问，可以找我(QQ:302003333)

## 编译

1. 将lftk取到当前目录

```
git clone https://github.com/xianjimli/lftk.git
```

2. 用keil打开user/lftk.uvproj

