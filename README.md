# AWTK针对STM32f103ze的移植。

[AWTK](https://github.com/zlgopen/awtk)是针对低端嵌入式设备开发的嵌入式GUI库。awtk-stm32f103ze-raw是AWTK在STM32f103ze上的移植。

第一版以[普中科技STM32F103ZET6开发实验板](https://item.taobao.com/item.htm?spm=a230r.1.14.1.50a130e8TMKYMC&id=558855281660&ns=1&abbucket=5#detail) 为载体移植(该开发板性价比不错，推荐一下:))。

## 编译

1. 将awtk取到当前目录

```
git clone https://github.com/zlgopen/awtk.git
```

2. 用keil打开user/awtk.uvproj

