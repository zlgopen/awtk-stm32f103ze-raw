# AWTK 针对 STM32f103ze 的移植。

* [AWTK](https://github.com/zlgopen/awtk) 全称 Toolkit AnyWhere，是 [ZLG](http://www.zlg.cn/) 开发的开源 GUI 引擎，旨在为嵌入式系统、WEB、各种小程序、手机和 PC 打造的通用 GUI 引擎，为用户提供一个功能强大、高效可靠、简单易用、可轻松做出炫酷效果的 GUI 引擎。

[awtk-stm32f103ze-raw](https://github.com/zlgopen/awtk-stm32f103ze-raw) 是 AWTK 在 STM32f103ze 上的移植。

本项目以 [普中科技 STM32F103ZET6 开发实验板](https://item.taobao.com/item.htm?spm=a230r.1.14.1.50a130e8TMKYMC&id=558855281660&ns=1&abbucket=5#detail) 为载体移植，其它开发板可能要做些修改，有问题请请创建 issue。

## 编译

1. 获取源码

> 将三个仓库取到同一个目录下：
```
git clone https://github.com/zlgopen/awtk.git
git clone https://github.com/zlgopen/awtk-demo-app.git
git clone https://github.com/zlgopen/awtk-stm32f103ze-raw.git
```

2. 用 keil 打开 awtk.uvproj

## 文档

* [AWTK 在 STM32F103 裸系统上的移植笔记](https://github.com/zlgopen/awtk/blob/master/docs/porting_to_stm32f103ze.md)

* LCD 控制器

> 这块板子老版本硬件和新版本硬件使用不同的 LCD 控制器，请根据自己的实际情况，修改 tftlcd.h，定义 LCD 控制器对应的宏：

新版本（缺省）

```c
#define TFTLCD_R61509VN 
```

旧版本

```c
#define TFTLCD_HX8357D 
```
