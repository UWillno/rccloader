# RccLoader
---

## 简介/Introduction

一个简易的QML/RCC加载器，编译成wasm方便加载。
A simple QML/RCC loader, compiled into wasm for easy loading.

适合纯QML项目，非性能场景，用于展示，比如[我的博客](https://uwillno.com)。
Suitable for pure QML projects, non-performance scenarios, for display, such as [my blog](https://uwillno.com).

可以从文件加载也可以从链接/代码加载。
Can be loaded from file or from link/code.

你可以根据自己的项目增删模块，加入其它cpp逻辑。
You can add or delete modules according to your own project, and add other cpp logic.

当前Qt版本6.10.1，采用单线程套件，具体模块参见[.pro](./rccLoader.pro)文件。
The current Qt version 6.10.1 uses a single-threaded package. For specific modules, see the [.pro](./rccLoader.pro) file.

## 使用/Use

wasm上依靠查询参数加载。
Wasm relies on query parameters to load.

```html
// 从原始代码，你需要使用;换行
// From the original code, you need to use ;line break
https://xxxxx.com/?code=import%20QtQuick;%20Window{%20visible:true;%20color:%20%22red%22;%20width:%20100;%20height:%20100;%20}
// 从base64代码
// From base64 code
https://xxxxx.com/?code_b64=aW1wb3J0IFF0UXVpY2s7CgpXaW5kb3d7CiAgICB2aXNpYmxlOnRydWUKICAgIGNvbG9yOiAicmVkIgogICAgd2lkdGg6IDEwMAogICAgaGVpZ2h0OiAxMDAKfQo=
// 从QMLURL 确保cors
// Ensure cors from QMLURL
https://xxxxx.com/?qml=https://xxxx.com/test.qml
// 从RCCURL 确保cors 可以选entry=qrc:/main.qml指定入口，默认为qrc:/Main.qml
// 确保已编译 rcc -binary my.qrc -o my.rcc
// Ensure cors from RCCURL. You can select entry=qrc:/main.qml to specify the entry. The default is qrc:/Main.qml.
// Make sure rcc is compiled -binary my.qrc -o my.rcc
https://xxxxx.com/?rcc=https://xxxx.com/my.rcc
```

[演示视频](https://www.bilibili.com/video/BV1xxqbBUEVL/)
[Demonstration video](https://www.bilibili.com/video/BV1xxqbBUEVL/)

## 其它/Other

代码仅供参考，未充分测试，只提供一个思路，你可以进行优化，建议是直接复制main.cpp自己改，或将web部分移到自己的静态托管平台避免跨域，还有我的Cloudflare Pages不一定稳定。
The code is for reference only and has not been fully tested. It only provides an idea. You can optimize it. It is recommended to directly copy the main.cpp and modify it yourself, or move the web part to your own static hosting platform to avoid cross-domain. Also, my Cloudflare Pages may not be stable.
