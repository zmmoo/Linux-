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
- c/s模式 
- `#include <sys/types.h> #include <sys/socket.h>`
- `int socket(int domain, int type, int protocol)`  成功返回sockfd,失败-1
- domain:AF_INET AF_INET6 AF_LOCAL
- type: SOCK_STREAM(TCP)  SOCK_DGRAM(UDP)  SOCK_RAW
- protocol:一般为0 原始套接字编程时需要

- `int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)`
- addr: struct sockaddr{sa_family_t sa_family_t; char sa_data\[14]};绑定时用
- struct sockaddr_in{sa_family_t sin_family; in_port_t sin_port;struct in_addr sin_addr};  struct in_addr{uint32_t s_addr;};
-  addrlen：地址长度







