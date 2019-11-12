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






