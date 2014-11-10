##简单的数据路由协议

* 采用异步方式实现，高并发，实时。
* 请注意，还处于测试阶段

####数据通信格式：[自身名字][对方名字][要发送的数据]

* ./client 0000000001 0000000001 1234567890
* 详细请看client.c 的代码, 上面的命令实现自己给自己发信息，
* 自身名字，对方名字，使用10个字符，不能小，也不能多。


####技术介绍

* 使用TCP长连接，实现client间的实时数据通信.
* 使用epoll,及uthash,达到高并发的目的

####参考的项目

* based on ohscar [epoll-event](https://github.com/ohscar/epoll-event)
* based on ankurs's [Poll-Event](https://github.com/ankurs/Poll-Event)

####下载方法

git clone https://github.com/hulonghui106/epoll-event-simple-routing.git
