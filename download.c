#include <unistd.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include "http.h"

void usage(int argc, char **argv)
{
	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s <URL>\n", argv[0]);
		exit(0);
	}
}

static void URLparse(char url[],char **host,char **filename)
{
	if(url[strlen(url) - 1] == '/')
	{
		printf("url error\n");
		exit(0);
	}
	char *h,*f;
	h = f = url;
	
	char *http  = "http://";
	char *https = "https://";
	if((strstr(url,http)) != NULL)
	{
		h = h + 7;
	}
	else if((strstr(url,https)) != NULL)
	{
		h = h + 8;
	}
	
	if((f = strstr(h,"/")) != NULL)
	{
		f++;
	}
	else
	{
		printf("url error\n");
		exit(0);
	}
	
	memcpy(*host,h,f-h-1);
	memcpy(*filename,f,strlen(f));
}

static int connect_server(const char *host)
{
	struct hostent * p = gethostbyname(host);
	
	int fd = socket(AF_INET,SOCK_STREAM,0);
	
	struct sockaddr_in cliaddr;
	socklen_t len = sizeof(cliaddr);
	bzero(&cliaddr,len);
	
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port   = htons(80);
	cliaddr.sin_addr = *((struct in_addr *)p->h_addr_list[0]);
	//cliaddr.sin_addr.s_addr = inet_addr("112.124.219.218");
	
	if(connect(fd,(struct sockaddr *)&cliaddr,len) == -1)
	{
		printf("Connect fail\n");
	}
	printf("Connect success\n");
	
	return fd;
}
static int check_file(const char *filename,int *length)
{
	char path[128];
	snprintf(path,128,"./%s",filename);
	
	int fd;
	*length = 0;
	struct stat buf;
	if(access(path,F_OK) == 0)
	{
		fd = open(path,O_WRONLY|O_APPEND);		
		stat(path,&buf);
		*length = buf.st_size;
		printf("fd = %d\n",fd);
	}
	else
	{
		fd = open(path,O_WRONLY|O_CREAT|O_TRUNC);
	}
	if(fd == -1)
	{
		printf("open fail\n");
		exit(0);
	}
	
	return fd;
}
static void progess(int length)
{
	char str[50] = {0};
	int i;
	for(i = 0;i<length;i++)
	{
		str[i] = '=';
	}
	printf("[%-50s]\r",str);
	fflush(stdout);
}

int main(int argc,char **argv)
{
	usage(argc,argv);
	
	int fd;
	
	char *host = calloc(1,1024);
	char *filename = calloc(1,1024);
	char *msg = calloc(1,1024);
	URLparse(argv[1],&host,&filename);
	printf("host = %s,filename = %s\n",host,filename);
	
	int start = 0;
	int fd_file = check_file(filename,&start);
	
	fd = connect_server(host);
	sendrequst(host,filename,start,msg);
	write(fd,msg,strlen(msg));
	free(msg);
	
	char httphead[1024];
	bzero(httphead,1024);
	int i = 0;
	while(1)
	{
		read(fd,httphead+i,1);
		i++;
		if(strstr(httphead,"\r\n\r\n") != NULL)
			break;
	}
	printf("%s",httphead);
	
	int length = 0;
	length = parseanswer(httphead);
	printf("%d",length);
	if(length == 0)
	{
		printf("文件已下载\n");
		close(fd);
		close(fd_file);
		exit(0);
	}
	
	
	i = 0;
	int n = 0;
	int precent = 0;
	char *tmp = calloc(1,1024);
	while(1)
	{
		bzero(tmp,strlen(tmp));
		n = read(fd,tmp,1024);
		if(n == 0)
			break;
		
		write(fd_file,tmp,n);
		i = i + n;
		if(i >= length)
			break;
		precent = (i+start)*50/(length+start);
		progess(precent);
	}
	close(fd);
	close(fd_file);
	free(tmp);
}





















