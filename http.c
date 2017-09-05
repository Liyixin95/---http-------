#include "http.h"

void sendrequst(const char *host,const char *filename,const int size,char *msg)
{
	snprintf(msg,1024,"GET /%s HTTP/1.1\r\n"
					  "Range: bytes=%d-\r\n"
					  "Host: %s\r\n\r\n",filename,size,host);
					  
	//printf("%s\n",msg);				  
}

int parseanswer(const char *httphead)
{
	char *tmp = calloc(1,1024);
	memcpy(tmp,httphead,strlen(httphead));
	int length = -1;
	char *l = NULL;
	
	if(strstr(tmp, "20") && !strstr(tmp, "206"))
	{
		fprintf(stderr, "对端服务器不支持断点续传，");
		fprintf(stderr, "本文件将重新下载... ...\n");
		return -1;
	}

	if(strstr(tmp, "30"))
	{
		fprintf(stderr, "重定向错误.\n");
	}
	if(strstr(tmp, "400"))
	{
		fprintf(stderr, "请求无效.\n");
	}
	if(strstr(tmp, "401"))
	{
		fprintf(stderr, "未授权.\n");
	}
	if(strstr(tmp, "403"))
	{
		fprintf(stderr, "禁止访问.\n");
	}
	if(strstr(tmp, "404"))
	{
		fprintf(stderr, "无法找到文件.\n");
	}
	if(strstr(tmp, "405"))
	{
		fprintf(stderr, "资源被禁止.\n");
	}
	if(strstr(tmp, "407"))
	{
		fprintf(stderr, "要求代理身份验证.\n");
	}
	if(strstr(tmp, "410"))
	{
		fprintf(stderr, "永远不可用.\n");
	}
	if(strstr(tmp, "414"))
	{
		fprintf(stderr, "请求URI太长.\n");
	}
	if(strstr(tmp, "50"))
	{
		fprintf(stderr, "服务器错误.\n");
	}
	
	if((l = strstr(tmp,"Content-Length: ")) == NULL)
	{
		fprintf(stderr, "未找到文件大小.\n");
	}
	else
	{
		l = l + strlen("Content-Length: ");
		length = atoi(l);
	}
	
	free(tmp);
	return length;
}



























