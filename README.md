Alchemy 炼金术师
==========

龙岩一中 eSp工作室（[@eSp工作室](http://weibo.com/lyyzesp)，[@没错我是好健康](http://weibo.com/hjkcai)）

简介
----------

Alchemy是一款Android平台上的元素组合游戏，（[传送门](http://www.coolapk.com/game/2812/)）

本项目是此游戏的电脑版，使用C++编写，使用Qt的界面库。

可执行文件会在游戏发布后，在龙岩一中贴吧中公布下载。

编译
----------

> 1. [下载Qt 4.8.4库](http://qt-project.org/downloads)，Windows下的编译还需安装含有gcc 4.4的MinGW
> 2. `qmake Alchemy.pro`
> 3. `make`
> 4. `make clean`（可选）

注：

1. 编译后，保留res文件夹中的全部文件和可执行文件即可，其余文件均可删除
2. 由于我暂时还不能够编写Makefile，所以在OS X平台下，请将res文件夹中的内容手动复制到`Alchemy.app/Contents/Resources`中，制成单个的app文件