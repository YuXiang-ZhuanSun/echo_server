# 回射服务器项目简介

使用socket和epoll ET模式，实现了一个回射服务器。->`epoll_et.c`

使用socket实现了一个可以输入字符串，发送到服务器，接受服务器消息的客户端程序。->`client.c`

可同时支持多个客户端

改变`client.c`程序中连接的IP地址和port，可以把这个服务器项目迁移到服务器中运行。

## 核心技术

* 非阻塞(nonblock)边缘触发(edge trigger）IO多路复用epoll技术
* 基于IP/TCP上层socket封装接口

## 演示

编译：

```c
ubuntu@VM-0-9-ubuntu:~/projects/echo_server$ gcc epoll_et.c -o server
ubuntu@VM-0-9-ubuntu:~/projects/echo_server$ gcc client.c -o client
```

运行与结果展示：

客户端：

```shell
ubuntu@VM-0-9-ubuntu:~/projects/echo_server$ ./client
kkkkkkkkkkkkkkkkkkkkkkdfasssssssssssssssssaaaaaaaaaaaaasdf
recv server data : kkkkkkkkkkkkkkkkkkkkkkdfasssssssssssssssssaaaaaaaaaaaaasdf

v bthyjgmfu uhmvtgf
recv server data : v bthyjgmfu uhmvtgf

天花板要到了
recv server data : 天花板要到了
```

服务器：

```shell
ubuntu@VM-0-9-ubuntu:~/projects/echo_server$ ./server
server recv:kkkkkkkkkkkkkkkkkkkkkkdfasssssssssssssssssaaaaaaaaaaaaasdf

read finished
server recv:v bthyjgmfu uhmvtgf

read finished
server recv:天花板要到了

read finished
client closed...
```

### 多客户端演示

客户端1：

```shell
ubuntu@VM-0-9-ubuntu:~/projects/echo_server$ ./client
答复是多少打发顺丰的
recv server data : 答复是多少打发顺丰的

抛物线天花板马师傅
recv server data : 抛物线天花板马师傅

阿斯顿发斯蒂芬
recv server data : 阿斯顿发斯蒂芬

对对对对对对多多多多多多多多多多多多多多多多多多多多多多多
recv server data : 对对对对对对多多多多多多多多多多多多多多多多多多多多多多多
```

客户端2

```shell
ubuntu@VM-0-9-ubuntu:~/projects/echo_server$ ./client
sdaffffffdfasdfasdf
recv server data : sdaffffffdfasdfasdf

ghuhbgh
recv server data : ghuhbgh

wueriotywtuiero
recv server data : wueriotywtuiero
```

服务器端：

```shell
ubuntu@VM-0-9-ubuntu:~/projects/echo_server$ ./server
server recv:答复是多少打发顺丰的

read finished
server recv:sdaffffffdfasdfasdf

read finished
server recv:ghuhbgh

read finished
server recv:抛物线天花板马师傅

read finished
server recv:阿斯顿发斯蒂芬

read finished
server recv:wueriotywtuiero

read finished
client closed...
server recv:对对对对对对多多多多多多多多多多多多多多多多多多多多多多多

read finished
```



