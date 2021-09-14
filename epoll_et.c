#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE 1024

int main()
{
    //create a sockfd : lfd
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd==-1){
        perror("socket");
        exit(-1);
    }

    //set sockfd nonblock
    int flag = fcntl(lfd, F_GETFL, 0);
    fcntl(lfd, F_SETFL, flag|O_NONBLOCK);

    //bind ip and port
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(9999);
    int ret = bind(lfd,(struct sockaddr *)&addr,sizeof(addr));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }

    //listen
    listen(lfd, 1024);

    //create epoll fd
    int epfd = epoll_create(1);

    //add sockfd to epoll
    struct epoll_event epev;
    epev.events = EPOLLIN;
    epev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &epev);

    struct epoll_event epevs[10000];
    
    while (1)
    {
        //epoll_wait until new message comes
        int rdlist_len = epoll_wait(epfd, epevs, 10000, -1);
        for(int i = 0;i < rdlist_len;i++)
        {
            int curfd = epevs[i].data.fd;
            if(curfd == lfd){
                int cfd;
                struct sockaddr_in caddr;
                int lll = sizeof(caddr);
                // accept new client
                while ((cfd = accept(lfd, (struct sockaddr *)&caddr, &lll))>0)
                {
                    //set sockfd(cfd) nonblock
                    int flag = fcntl(cfd, F_GETFL, 0);
                    fcntl(cfd, F_SETFL, flag|O_NONBLOCK);
                    epev.data.fd = cfd;
                    epev.events = EPOLLIN;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
                }
            }else{
                char buf[BUF_SIZE];
                bzero(buf, BUF_SIZE);
                //read to end
                int read_len;
                while ((read_len = read(curfd,buf,BUF_SIZE))>0)
                {
                    printf("server recv:%s\n",buf);
                    write(curfd,buf,strlen(buf));
                    bzero(buf, BUF_SIZE);
                }
                if(read_len == 0){
                    printf("client closed...\n");
                    epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
                    close(curfd);
                }else if(read_len == -1){
                    if((errno==EAGAIN)||(errno == EWOULDBLOCK)){
                        printf("read finished\n");
                    }else{
                        perror("read");
                        exit(-1);
                    }
                }
            }
        }
    }
    epoll_ctl(epfd, EPOLL_CTL_DEL, lfd, NULL);
    close(lfd);
    close(epfd);
    return -1;
}