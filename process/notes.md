# 命令
- ps -ef （man ps 中包含了详细信息）
- ps -aux
- top （动态显示）
- cd /proc(对应进程号的详细信息)
- nice -n 2  ./xxx  按指定优先级(-20 ~ +19)运行
- renice -n 2(优先级) xxxx(进程号) 普通用户只能降低优先级
- jobs 查看后台进程 （后台运行进程 ./xxx & ）  fg 1(作业号) 后台进程前台运行   ctrl+z后台挂起  bg 2（后台运行）
# 函数
- 创建进程  #include<unistd.h>   pid_t fork(void)  失败返回 -1 成功时 父进程返回子进程号 ，子进程返回 0
- 获取进程号 pid_t getpid(void)  ; 获取父进程号pid_t getppid(void)
- 子进程几乎复制了父进程的内容 ， 父子进程有自己独立的地址空间 ，互不影响， 若父进程先结束，子进程成为孤儿进程，被init进程收养
- 若子进程先结束时 父进程没有及时回收，子进程变成僵尸进程
- 进程结束 #include<stdlib.h> #include<unistd.h> \<br>void exit(int status)(退出时刷新流的缓冲区)  void \_exit(int status)




