/*************************************************************************
	> File Name: mysleep.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月29日 星期一 04时16分20秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void MyHandler(int sig)
{
    (void)sig;
    //什么都不用做
}

int Mysleep(int second)
{
    //1,使用sigaction,注册一个SIGALRM处理函数
    struct sigaction new_action,old_action;
    new_action.sa_handler = MyHandler;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;
    sigaction(SIGALRM,&new_action,&old_action);
    //2,调用alarm函数，N秒之后，系统会自动发送SIGALRM
    alarm(second);
    //3,调用pause
    pause();
    //4,扫尾工作
    sigaction(SIGALRM,&old_action,NULL);
    return alarm(0);//取消闹钟
}

int Mysleep2(int second)
{
    //1,使用sigaction注册一个SIGALRM函数
    struct sigaction new_action,old_action;
    new_action.sa_handler = MyHandler;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;
    sigaction(SIGALRM,&new_action,&old_action);
    //2,将我们的SIGALRM信号加入到信号屏蔽字中
    sigset_t new_block,old_block;
    sigemptyset(&new_block);
    sigaddset(&new_block,SIGALRM);
    sigprocmask(SIG_BLOCK,&new_block,&old_block);
    //3,调用alarm函数，N秒之后，系统会自动发送SIGALRM
    alarm(second);
    //4,不能调用pause,而是使用sigsuspend来代替
    sigset_t suspend_mask = old_block;
    sigdelset(&suspend_mask,SIGALRM);
    sigsuspend(&suspend_mask);
    //5,扫尾工作
    sigaction(SIGALRM,&old_action,NULL);
    sigprocmask(SIG_SETMASK,&old_block,NULL);
    return alarm(0);
}

int main()
{
    while(1)
    {
        printf("hehe\n");
        Mysleep2(1);
    }
    return 0;
}
