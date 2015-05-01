#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

char s1[4] = "xxxx"; // thread1������д��abcd
char s2[4] = "xxxx"; // thread2������д��1234

int len1 = -1; // ��ǰ��д��s1��λ��(����thread1,thread3ͨ��)
int len2 = -1; // ��ǰ��д��s2��λ��(����thread2,thread4ͨ��)

int out = 0; // Ϊ0ʱthread3��д��Ϊ1ʱthread4��д�����߽���д��buf(����thread3,thread4ͨ��)

int buf_pos = 0; // ������������±�
char buf[10] = "xxxxxxxxxx"; // �����������(a1b2c3d4)

void writeChar(char c)
{
	buf[buf_pos++] = c;
	printf("                write char:%c\n", c);
}

void *thread1(void *args)
{
	char s[] = "abcd";
	while(1) {
		
		len1++;
		if(len1 >= 4) {
			break;
		}
		s1[len1] = s[len1];
		printf("thread 1 write(%c)\n", s[len1]);
		
		sleep(1);
	}
	printf("thread 1 finished.\n");
}

void *thread2(void *args)
{
	char s[] = "1234";
	while(1) {
	
		len2++;
		if(len2 >= 4) {
			break;
		}
		s2[len2] = s[len2];
		printf("thread 2 write(%c)\n", s[len2]);
		
		sleep(1);
	}
	printf("thread 2 finished.\n");
}

void *thread3(void *args)
{
	int i;
	char c;
	
	for(i = 0; i < 4; ++i) {
		
		while(i > len1);
		
		c = s1[len1];
		
		printf("thread 3 read(%c)\n", c);
		
		while(out == 1);
		writeChar(c);
		out = 1;
		
		sleep(1);
	}
	printf("thread 3 finished.\n");
}

void *thread4(void *args)
{
	int i;
	char c;
	
	for(i = 0; i < 4; ++i) {
		
		while(i > len2);
		
		c = s2[len2];
		
		printf("thread 4 read(%c)\n", c);
		
		while(out == 0);
		writeChar(c);
		out = 0;
		
		sleep(1);
	}
	printf("thread 4 finished.\n");
}


int main(void)
{
	pthread_t t1, t2, t3, t4;

	printf("creating thread...\n");

	pthread_create(&t1, NULL, thread1, NULL);
	pthread_create(&t2, NULL, thread2, NULL);
	pthread_create(&t3, NULL, thread3, NULL);
	pthread_create(&t4, NULL, thread4, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	pthread_join(t4, NULL);

	printf("all thread finished.\n");

	buf[buf_pos] = 0;
	printf("%s\n", buf);

	return 0;
}
