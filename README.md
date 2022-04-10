# Linux应用程序看门狗

原理简介：

使用fork开启子进程，并在子进程使用execvp运行目标程序。当子进程退出后，主进程使用waitpid获取退出方式，如果不是正常退出（默认情况返回0视为正常退出），则再次运行目标程序。

---

功能一览：

* 可以给目标程序传递命令行参数。
* 可重定向目标程序的标准输出和错误输出。
* 可设置重启目标程序的时间间隔。
* 可设置目标程序的正常退出返回值。

---

编译安装：

```bash
cmake -B build
cmake --build build
sudo cmake --install build
```

---

使用方法：

```bash
watchdog [options] executable [arguments]...

# options:
#     --help:      显示帮助信息  
#     --stdout:    设置重定向stdout的文件
#     --stderr:    设置重定向stderr的文件
#     --interval:  设置重启间隔[单位:s][default=1]
#     --exit:      设置正常退出返回值[default=0]
#
# executable:      目标可执行文件
# arguments...:    传递给目标可执行文件的命令行参数
```
