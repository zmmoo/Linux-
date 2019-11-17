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
