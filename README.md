
简单的数据通信协议
===============
采用异步方式实现，高性能，实时。

数据通信格式：[自身名字][对方名字][要发送的数据]
------------------

详细请看client.c 的代码。client.c 实现发送数据到自己的功能，类似echo服务。
自身名字，对方名字，使用10个字符，不能小，也不能多。

技术介绍
-------
.使用TCP长连接，实现client间的数据通信.
.使用epoll,及uthash,达到实时响应的目的

参考的项目
--------
based on ohscar
https://github.com/ohscar/epoll-event

based on ankurs's Poll-Event
https://github.com/ankurs/Poll-Event

下载方法
-------
git clone https://github.com/hulonghui106/epoll-event-simple-routing.git


