## 信号（signal）
> 信号是unix和linux操作系统经典的消息机制，主要用途是处置进程（利用信号杀死 挂起进程 ） 也可以在开发中使用信号机制 例如信号捕捉 让信号绑定一个行为（捕捉函数） 信号递达 系统自动调用对应函数
* 系统支持的信号 kill -l #ubuntu系统支持的所有信号列表
> ![374091862fb2ccca251946bfbc2841ed.png](../_resources/374091862fb2ccca251946bfbc2841ed.png)
* 两类信号
	* 1-31 unix 经典信号
	* 32-33被隐藏了 不提供给用户使用 保留给NPTL线程库使用 用户无法直接使用 
	* 34-64 自定义信号
### 系统中信号的集中触发方式
> 在终端bash中 前台进程是唯一的 +标识 S+ R+

1. 终端组合按键触发信号
		1. ctl+c 系统触发SIGINT(2)杀死前台进程
		2. ctl+\ 系统触发SIGQUIT(3) 杀死前台进程
		3. ctl+z 系统出发SIGTSTP(20)，挂起前台进程
* 终端组合件触发信号 对后台进程无效
![ee06d95bcff1ab71ae8398e69e9c5615.png](../_resources/ee06d95bcff1ab71ae8398e69e9c5615.png)
终端组合按键如何被解析和处理
> 线路规划识别终端组合按键 完成后续操作
![670b5d058d515a58066adea00ae23364.png](../_resources/670b5d058d515a58066adea00ae23364.png)
2. 命令触发信号
```bash
	kill -signo pid(-pgid) #向任意进程或任意组 发送信号杀死进程 可能会因为权限不足而失败 必须对目标进程或组有足够的权限
```
3. 函数触发信号
```bash
#include<signal.h>
kill(pid_t pid,pid_t signo);//向任意进程或组发送任意信号 函数版本
raise(pid_t signo); //向当前进程发送任意信号
void abort(void); //向当前进程发送固定的SIG_ABRT信号
sigqueue(pid_t pid ,pid_t signo,union sigval val);//向任意进程发送任意信号，并且携带union信息包 支持ipc
```
4. 硬件异常产生信号
* 段错误，进程中内存访问异常 例如对只读内存进行写操作， 会触发段错误 系统发送SIGSEGV(11)杀死进程
![68074e4b5944b9cdf43cd7dfed4df04d.png](../_resources/68074e4b5944b9cdf43cd7dfed4df04d.png)
* 总线错误 内存的越界访问 访问野地址 系统会发送SIGBUS(7),杀死目标进程
 ![7db0918ddffd3cf5fc37d4423e82335e.png](../_resources/7db0918ddffd3cf5fc37d4423e82335e.png)
* 浮点数例外 使用cpu计算机 产生计算异常 系统发送SIGFPE(8)杀死目标进程
![96905544a2f24f393511d30b27212a88.png](../_resources/96905544a2f24f393511d30b27212a88.png)
5. 软条件产生信号
	管道读端关闭 写端写管道 满足软条件 系统发送SIGPIPE(13)杀死写端进程
	使用alrm定时器 如果定时到时 系统向进程发送	SIGLRM(14) 杀死定时进程
```c
	unsigned int alarm(unsigned int seconds);//参数为定时的秒数 返回值为未定时够的秒数 返回0代表成功 返回>0代表定时失败 有未定时够的时长
```
![9f5aa7864b34939abfd2b861d7cbb67b.png](../_resources/9f5aa7864b34939abfd2b861d7cbb67b.png)
![46d5a35fade12b231bff414ee2922de7.png](../_resources/46d5a35fade12b231bff414ee2922de7.png)
![547859da59befa70537f29923f99cd58.png](../_resources/547859da59befa70537f29923f99cd58.png)