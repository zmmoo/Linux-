# 命令
- ps -ef （man ps 中包含了详细信息）
- ps -aux
- top （动态显示）
- cd /proc(对应进程号的详细信息)
- nice -n 2  ./xxx  按指定优先级(-20 ~ +19)运行
- renice -n 2(优先级) xxxx(进程号) 普通用户只能降低优先级
- jobs 查看后台进程 （后台运行进程 ./xxx & ）  fg 1(作业号) 后台进程前台运行   ctrl+z后台挂起  bg 2（后台运行）
# 函数
- 创建进程  `#include<unistd.h>   pid_t fork(void)`  失败返回 -1 成功时 父进程返回子进程号 ，子进程返回 0
- 获取进程号 ```pid_t getpid(void)``` ; 获取父进程号`pid_t getppid(void)`
- 子进程几乎复制了父进程的内容 ， \ 父子进程有自己独立的地址空间 ，互不影响， 若父进程先结束，子进程成为孤儿进程，被init进程收养
- 若子进程先结束时 父进程没有及时回收，子进程变成僵尸进程
- 进程结束 `#include<stdlib.h> #include<unistd.h>  void exit(int status)`(退出时刷新流的缓冲区)  `void \_exit(int status)`
## exec函数族
### 进程
- `#include <unistd.h>int execl(const char \*path, const char \*arg,....)` 失败返回 -1
- `#include <unistd.h>int execlp(const char \*file, const char \*arg,..)`失败返回 -1
path: 执行程序的名称 包含路径  arg:最后一个为NULL  file:程序名称 直接在PATH中查找
- eg: execl("/bin/ls","ls","-a","-l","/etc",NULL) 
- eg: execlp("ls","ls","-a","-l","/etc",NULL)

- `#include<unistd.h> int execv(const char*path, char *const argv[])` 失败返回-1
- `#include<unistd.h> int execvp(const char*file, char *const argv[])` 失败返回-1
- eg: `char \argv[]={"ls","-a", "-l", "/etc",NULL}; execv("/bin/ls",arg)`
- `#include <stdlib.h> int system(const char *command)` 失败返回 -1
## 进程回收 
- `#include<unistd.h> pid_t wait(int *status)`  成功时返回回收进程号  失败返回-1  若子进程没有结束，父进程一直阻塞  那个子进程先结束就先回收  返回子进程返回值和结束方式 status:man wait中有详细的宏定义
- `#include <unistd.h> pid_t waitpid(pid_t pid, int*staus, int option)`失败时返回-1 option: 0（阻塞） / WNOHANG（非阻塞） 返回0时 表示进程没有结束  pid: -1 表示任意子进程













