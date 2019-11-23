# 协议
## 网络接口和物理层  ：屏蔽硬件差异
- MAC地址 48位网络设备的身份标识
- arp\rarp 
- arp: ip地址----->MAC地址
- rarp：MAC地址--->IP地址
- ppp协议：拨号协议（GPRS\3G\4G）
## 网络层： 端到端的传输
- ip：协议  IPV4 IPV6 
- ICMP  ping
- IGMP  广播，组播
## 传输层： 数据应该交给哪一个任务处理
- TCP 可靠传输，提供面向连接，一对一的可靠传输协议
- UDP 不可靠传输  无连接的尽力传输
- SCTP 
## 应用层： http dns smtp ftp telnet ntp snmp
- rtp\rtsp  用于传输音频视频（安防监控）

- 以太网头(14byte)-->IP header(20byte)-->TCP header(20bytes)-->data-->以太网尾(4byte)
- MTU：最大传输单元
- mss：最大的端空间
# 网络编程
## SOCKET 
- 是一种特殊的文件描述符
- 代表着网络编程的一种资源
- 流式套接字（SOCK_STREAM） 对应着TCP
- 数据报套接字（SOCK_DGRAM） 对应UDP
- 原始套接字（SOCK_RAW） 跨过传输层，可以对较低的层次协议直接访问 IP ICMP
## IP
- 局域网IP： 192.xxx.xxx.xxx 10.xxx.xxx.xxx
- 广播IP：xxx.xxx.xxx.255 255.255.255(广播地址)
- 组播IP： 224.xxx.xxx.xxx~239.xxx.xxx.xxx
## 端口号
- 16为数字（1--65535）不可用（1--1023）  FTP 21 SHH 22 HTTP 80 HTTPS 469
- 1024-5000（建议不使用） 
- TCP和UDP 端口独立
- 网络里的通讯是 IP地址+端口号来决定的
## 字节序
- 字节序是指不同的CPU访问内存中的多字节的数据时候的问题
- 小端模式  低端内存放低端数据 （X86 \ARM）
- 大端模式  低端内存存放高端数据(powerpc/mips/ARM作为路由器时)
- 网络传输的时候采用大端模式  
- 本地字节序、网络字节序(htonl,ntohl,htons,ntohs)
- iP地址转换函数   `in_addr_t inet_addr(const char \*cp)`
cp :点分式转成32位整数（包含了字节序的转换，默认为网络字节序） 仅适用于IPV4 不能用于 255.255.255.255
- `int inet_pton(int af, const *src, void *dst)`  适用于IPV4\6 正确处理 4个255的问题
- af:地址协议族（AF_INET AF_INET6） src: 点分形式的IP地址 dst:转换的结果

## TCP编程
- 连接时3次握手　　退出时４次握手
- c/s模式 server :socket-->bind-->listen-->accept-->文件操作
- client:socket-->connect-->文件操作
- `#include <sys/types.h> #include <sys/socket.h>`
- `int socket(int domain, int type, int protocol)`  成功返回sockfd,失败-1
- domain:AF_INET AF_INET6 AF_LOCAL
- type: SOCK_STREAM(TCP)  SOCK_DGRAM(UDP)  SOCK_RAW
- protocol:一般为0 原始套接字编程时需要

- `int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)`
- addr: struct sockaddr{sa_family_t sa_family_t; char sa_data\[14]};绑定时用
- struct sockaddr_in{sa_family_t sin_family; in_port_t sin_port;struct in_addr sin_addr};  struct in_addr{uint32_t s_addr;};
-  addrlen：地址长度

- `int listen(int sockfd, int backlog)` 成功返回0 失败-1
- backlog: 一般填5  同时允许几路客服端和服务器进行正在的连接过程
- `int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)` 
- 阻塞等待客户连接   失败返回-1  成功返回已经建立好连接的sockfd
- addr:客户端信息（IP地址和端口号）
-  `int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)` 成功返回0 失败-1
- connect与bind相似
- `ssize_t send(int sockfd, const void *buf, size_t len, int flags)`
- flags: 一般为0 此时和write一样   为MSG_DONTWAIT时 不阻塞 MSG_OOB
- `ssize_t recv(int sockfd, void *buf, size_t len, int flags)`
- flags: 一般为0 此时和read一样   为MSG_DONTWAIT时 不阻塞 MSG_OOB MSG_PEEK（读取时流的位置不移动）

## UDP编程（无连接的尽力传输）
- server:socket-->bind-->recvfrom(阻塞等待客户端数据)-->sendto
- client: socket-->sendto-->recvfrom-->close
- `ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,const struct sockaddr *dest_addr, socklen_t addrlen);`不阻塞
- dest_addr:需要发送的地址
- ` ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,struct sockaddr *src_addr, socklen_t *addrlen);`
- src_addr：需要接受的数据的地址
## IO模型
### 阻塞IO
- read(没有数据时阻塞)  
- recv recvfrom  write send accept connect
### 非阻塞IO
- 使用fcntl实现非阻塞  `flag=fcntl(sockfd, F_GETEL, 0) flag|=O_NONBLOCK`
- `int b_on = 1ioctl(sockfd, FIONBIO,&b_on)`
### 多路复用IO
- 针对的不止时套接字，所有的文件描述符
- 1 把关心的文件描述符加入到fd_set中
- 2 调用select()/poll()函数去监控集合fd_set中的文件描述符，阻塞等待一个或多个文件描述符有数据
- 3 当有数据时，退出select（）阻塞
- 4 依次判断哪个文件描述符有数据
- 5 依次处理有数据的文件描述符
- `void FD_ZERO(fd_set *fdset)`  集清0合
- `void FD_SET(int fd, fd_set *fdset)` 加入集合
- `void FD_CLR(int fd, fd_set *fdset)` 从集合清除
- `void FD_ISSET(int fd, fd_set *fdset)`  判断fd是否包含fdset
- `int select(int nfds, fd_set *readfds, fd_set*writefds, fd_set *exceptfds, struct timeval *timeout)`
- nfds:最大文件描述符加一
- 一般readfds writefds为NULL 异常数据一般为NULL 
- timeout  `struct timeval{long tv_sec(秒); long tv_usec（微妙）;}`
- select退出后：集合表示有数据的集合
- 网络编程中read返回值为0时　表示对方关闭

## 网络信息检索
- `#include <netdb.h> extern in h_errno;`
- `struct hostent *gehostbyname(const char *name)` 域名解析 得到３２位网络字节序 只能使用于IPV4
- `struct hostent {`
              ` char  *h_name;            /* official name of host */`
              ` char **h_aliases;         /* alias list */`
              ` int    h_addrtype;        /* host address type */`
              ` int    h_length;          /* length of address */`
               `char **h_addr_list;       /* list of addresses */`
          ` }`
- `void endhost(void)`
## 网络属性设置
- `int getsockopt(int sockfd, int level, int optname, void **optval, socklen_t *optlen)` 获取网络属性
- level: SOL_SOCKET : 通用套接字（应用层）IPPROTO_TCP TCP选项 (传输层)　IPPROTO_IP IP选项（网络层）
- optname: man 7 socket 中的sock options
- `int b_br = 1 setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,&b_br, sizeof(int))` 允许地址快速重用
- `int b_br = 1 setsockopt(fd, SOL_SOCKET, SO_BROADCAST,&b_br, sizeof(int))` 允许F发送广播
- `struct timeval out setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO,&b_br, sizeof(struct timeval))` 设置接受超时
## 网络超时
- 1.使用setsockopt recv和recvfrom 等待超时时间　不阻塞
- 2.select()函数设置超时参数
- alarm(5)  使用signal捕获信号
- 函数　`void setKeepAlive(int sockfd, int attr_on, socklen_t idle_time, socklen_t interval, socklen_t cnt)'
-`{setsockopt(sockfd, SOL_SOCKET,SO_KEEPALIVE, (const char*)&attr_on, sizeof(attr_on);setsockopt(sockfd, SOL_TCP,KEEPIDLE,(const char*)&attr_on, sizeof(idle_time, sizeof(idle_time));setsockopt(sockfd, SOL_TCP,SO_KEEPINTVAL, (const char*)&interval, sizeof(interval);setsockopt(sockfd, SOL_SOCKET,SO_KEEPCNT, (const char*)&cnt, sizeof(cnt);}`
- 使用`int keepAlive = 1; int keepIdle=5; int keepInterval=5; int keepCount=3; setKeepAlive(newfd, keepAlive, keepIdle, keepInterval, keepCount)`











