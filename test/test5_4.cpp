#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

static bool stop = false;
/* SIGTERM 信号处理函数，触发时结束主程序中的循环）*/
static void handle_term(int sig)  //信号处理函数 
{
    stop = true;
}

int main(int argc, char* argv[])
{
    signal(SIGTERM, handle_term);

    if(argc <= 3)
    {
        printf("usage: %s ip_address port_number backlog\n", basename( argv[0] ) );
        return 1;
    }

    const char *ip = argv[1];
    int port = atoi(argv[2]);
    int backlog = atoi(argv[3]);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0); //assert 将通过检查表达式 expression 的值来决定是否需要终止执行程序

    /*创建一个ipv4 socket地址*/
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(sock, backlog); //服务端每次accept都会返回一个socket
    assert(ret != -1);

    while(!stop)
    {
        sleep(1);
    }

    close(sock);
    return 0;
}
