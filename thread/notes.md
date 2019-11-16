# 进程
- linux为每个进程创建task_struct
- 进程在切换时系统的开销比较大

# 线程
- 轻量级进程LWP
- 同一进程的线程共享相同的地址空间
- 通常线程指的是共享的相同地址空间的多个任务
## linux线程库
### 创建线程 
- `#include<pathread.h> int pthread_create(pthread_t \*thread, const pthread_attr_t *attr,void*(*routine)(void*),void *arg)`
成功返回0 失败返回错误码 thread线程对象 attr线程属性，NULL代表默认属性  routine线程执行的函数 传递给线程执行函数的参数
### 线程回收
- `#include<pthread.h> int pthread_join(pthread_t thread, void **retval)`
- 成功返回0 失败返回错误码 thread要回收的线程对象  调用线程阻塞直到thread结束 *retval接受线程thread的返回值
### 结束线程
- `#include <pthread.h> void phtread_exit(void * retval)`
- retval可以被其他进程通过pthread_join获取
- 编译gcc -o test test.c -lpthread
## 线程间通信
- 使用全局变量交换数据  （多个线程访问共享数据时需要同步或互斥机制）
### 同步
- 同步指的是多个任务按约定的先后次序相互配合完成一件事
### 信号量 
- 代表某一类资源，其中表示系统中资源的数量  只能通过三种操作来访问
- 1.初始化
- 2.p操作（申请资源）
- 判断信号量的值大于0 申请资源的任务运行 信号量减一  否则申请资源的任务阻塞
- 3.V操作（释放资源）
- 信号量的值加一 如果有任务等待资源  唤醒任务
#### posix信号量
- 无名信号量（基于内存的信号量），一般用于线程之间通讯
- 有名信号量  既能由于线程也能由于进程通讯
## 函数
- pthread库常用的信号量操作
- `#include<semaphore.h>`  
- `int sem_init(sem_t *sem, int pshared, unsigned int value)` 成功返回0 失败 -1
- sem 指向要初始化的信号量对象  pshared 0(线程间) 1（进程间）value信号量初值
- `int sem_wait(sem_t *sem)` P操作  成功返回0 失败 -1  
- `int sem_post(sem_t *sem)` V操作  成功返回0 失败 -1
## 互斥 
- 临界资源：一次只允许一个任务（进程、线程）访问的共享资源
- 临界区：访问临界区的代码
### 互斥机制
- mutex互斥锁 
- 任务访问临界资源前申请锁，访问完后释放锁
### 互斥锁函数
- `#include <pthread.h>`
- `int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);` 成功时返回0 失败返回错误码  mutex 初始化互斥锁对象 attr互斥锁属性，NULL表示缺省属性
- `int pthread_mutex_lock(pthread_mutex_t *mutex)` 如果无法获得锁 就会阻塞
- `int pthread_mutex_unlock(pthread_mutex_t *mutex)` 执行完临界区要及时释放锁
# 进程间通信
## 早期UNIX进程间通信
### 无名管道（pipe）
- 只能用于有亲缘关系的进程
- 单工的通讯模式，具有固定的读端和写端
- 1.写端存在  读无名管道  有数据:read返回实际读取的字节数  无数据:进程阻塞
- 2.写端不存在  读管道   有数据:read返回实际读取的字节数  无数据:read返回0
- 3.读端存在  写管道     有空间：write返回写入的字节数  无空间：进程阻塞  （默认管道64K）
- 4.读端不存在  写管道   管道断裂 
- kill -l(查看信号列表)
#### 函数 
-  `#include <unistd.h>`
- `int pipe(int pfd[2])` 成功返回0 失败返回-1（EOF） pfd:包含两个元素的整形数组 
- pfd\[0]由于读管道 pfd\[1]由于些管道 
### 有名管道（fifo）
- 对应管道文件，可以用于任意进程之间进程通信
- 打开管道是可以指定读写方式
- 通过文件IO操作，内容放在内存中
- 读写端关闭时，内容清除
#### 函数
- `#include <unistd.h> #include <fcntl.h>`
- `int mkfifo(const char * path, mode_t mode)`
- path:管道路径
- mode:管道文件权限 0666
- 只有读端和写端都打开了 open()才不会阻塞
### 信号（signal）
- SIGUP  关闭终端时产生
- SIGINT  = ctrl+c 
- SIGQUIT = ctrl+\
- SIGILL 一个进程企图执行一条非法指令时产生
- SIGSEV  非法访问内存时   
- SIGPIPE 往一个没有读端的管道写操作时产生
- SIGKILL 结束进程 不能捕捉和忽略
- SIGSTOP  暂停
- SIGTSTO ctrl+Z暂停
- SIGCONT  让进程进入运行态
- SIGALRM 通知进程定时器时间已到
- SIGUSER1/2  保留给程序使用   
#### 命令
- kill(向进程发信号) \[-signal] pid  （kill默认发送 15（SIGTERM））
- killall \[-u user|prog ]    prog:指定程序名 user:指定用户名
#### 函数
- `#include <unistd.h> #include<signal.h>`
- `int kill(pid_t pid, int sig)`  成功返回0 失败-1
- `int raise(int sig)`  给自己发信号
- pid: 接收信号的进程号， 0代表同组， -1 代表所有进程
- sig：信号类型
- `int alarm(unsigned int seconds)` 设置闹钟 成功时返回上一个定时器的剩余时间  失败返回-1
- second：定时器的时间   一个进程中只有一个定时器  时间到时产生SIGALRM
- `int pause(void)`
- 进程一直阻塞，直到信号中断
- 信号中断后返回-1 errno为EINTR

#### 设置信号响应方式
- `#include <unistd.h>  #include <signal.h>`
- `void (*signal(int signo,void(*handler)(int)))(int)` 不能设置SIGKILL\SIGSTOP
- 成功时返回原先的信号处理函数  失败返回SIG_ERR
- signo 要设置的信号
- handler 指定信号处理函数  SIG_DFL 代表缺省方式  SIG_IGN 代表忽略信号
## System V IPC
- 每个IPC对象有唯一的ID
- IPC对象创建后一直存在  直到被显示的删除
- 每个IPC对象有一个关联的key
- ipcs(查看systme V对象)/ipcrm（删除systme V对象）
#### 生成IPC对象的KEY值
- `#include <sys/types.h> #include <sys/ipc.h>`
- `key_t ftok(const char *path, int proj_id)`  成功时返回key值 失败返回-1
- path：存在且可以访问的文件路径， proj_id:不为0
### 内存共享(share memory)
#### 共享内存的使用步骤
- 1.创建\打开共享内存
- `#include <sys/ipc.h> #include <sys/shm.h`
- `int shmget(key_t key, int size, int shmflg)`  成功时返回共享内存ID 失败返回-1
- key 和共享内存关联的key， IPC_PRIVATE或ftok生成
- shmflg 共享内存标志位 IPC_CREAT|0666
- 2. 映射共享内存，即把指定的共享内存映射到进程的地址空间用于访问
- `#include <sys/ipc.h>#include <sys/shm.h>`
- `void *shmat(int shmid, const void *shmaddr,int shflg)`成功返回映射后的地址 ，失败-1
- shmid 要映射的共享内存id
- shmaddr 映射后的地址， NULL表示由系统自动映射
- shmflg 标志位 0 表示可读写， SHM_RDONLY表示只读
- 3. 共享内存撤销 
- `#inlcude <sys/ipc.h> #include <sys/shm.h>`
- `int shmdt(void *shmaddr)`  成功返回0  失败 -1
- 进程结束时自动撤销
- 4 共享内存控制
- `#inlcude <sys/ipc.h> #include <sys/shm.h>`
- `int shmctl(int shmid, int cmd, struct shmid_ds *buf)`
- shmid:要操作的共享内存IDq
- cmd 要操作IPC_STAT(获取共享内存的属性) IPC_SET（设置属性从buf）IPC_RMID（删除id buf:NULL)
- buf 如果使用IPC_STAT IPC_SET 保存属性  
- ipcs -l (查看IPC对象的设置)
- cat /proc/sys/kernel/shmmax 修改参数
- 共享内存删除  shmctl（）添加删除标记   nattach变成0时真正删除
### 消息队列(message queue)
- 消息队列由ID来唯一标识
- 用户可以读写
- 消息队列可以按照类型来发送/接收消息
- 1.打开/创建消息队列 
- `#include <sys/ipc.h> #include <sys/msg.h>`
- `int msgget(key_t key, int msgflg)` 成功返回队列id，失败-1
- key:和消息队列关联的key IPC_PRIVATE或ftok
- msgflg 标志位IPC_CREAT|0666
- 2.发送消息
- `#include <sys/ipc.h> #include <sys/msg.h>`
- `int msgsnd(int msgid, const void*msgp, size_t size, int msgflg)` 成功时返回0 失败-1
- msgid 消息队列id
- msgp 消息缓冲区地址
- size 消息正文长度
- msgflg 标志位0(消息发送成功时返回) 或IPC_NOWAIT
- 3 消息接收 
- `#include <sys/ipc.h> #include <sys/msg.h>`
- `int msgrcv(int msgid, void *msgp, size_t size, long msgtype, int msgflg)` 成功时返回消息长度，失败-1
- msgid: 消息队列id 
- msgp 消息缓冲区地址
- size 指定接收消息长度
- msgtypd 指定接收消息的类型（0 最早的消息）
- msgflg 标志位0(消息发送成功时返回) 或IPC_NOWAIT
- 4.控制消息队列
- `#include <sys/ipc.h> #include <sys/msg.h>`
- `int msgctl(int msgid, int cmd, struct msqid_ds *buf)` 成功时返回0 失败-1
- cmd 要操作IPC_STAT(获取共享内存的属性) IPC_SET（设置属性从buf）IPC_RMID（删除id buf:NULL)
- buf 如果使用IPC_STAT IPC_SET 保存属性 
#### 消息格式
- 首成员必须为long 消息的类型  使用时可以任意值（不可为0 或负数）
- 正文 （自由定义类型）  通讯时双方要相同的 类型（首成员值 相同）
### 信号灯(semaphore set)
- 信号灯也叫信号量 用于进程\线程同步或互斥的机制
- posix 无名信号灯  有名信号灯    System V信号灯
- System V信号灯是一个或多个计数信号灯的集合
- 可同时操作集合中的多个信号灯
- 申请资源时避免死锁  
#### 函数 
- `#include <sys/ipc.h> #include <sys/sem.h>`
- 打开\创建信号灯
- `int semget(key_t key, int nsems, int semflg)` 成功返回信号灯Id 失败返回-1
- nsems  集合中包含计数信号灯的个数
- semflg 标志位 IPC_CREAT|0666|IPC_EXCL
- 信号的初始化
- `int semctl(int semid, int semnum, int cmd,...)`成功返回信号灯Id 失败返回-1
- semid: 要操作那个信号灯集合id
- semnum： 要操作的集合中的信号的编号
- cmd 要执行的操作 SETVAL IPC_RMID
- union semun 取决于cmd（SETVAL时）（自己定义这个共用体 man semctl中有介绍）
- P\V操作
- `int semop(int semid, struct sembuf *sops, unsigned nsops)`成功返回信号灯Id 失败返回-1
- sops: 描述对信号的操作的结构体
- nsops 要操作的信号的个数
- `struct sembu{short semnum ;short sem_op; short sem_flg }`  
- semnum 信号灯编号
- sem_op -1:P操作 1：V操作
- sem_flg  0\IPC_NOWAIT

- 删除信号灯
## 套接字（socket）





