
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include<sys/ioctl.h>
#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)


void dump(char *str, char *buf, int size)
{
	int i;

	if (size <= 0)
		return;

	printf("%s", str);
	for (i = 0; i < size; i++)
		printf("%02x ", buf[i]);
	printf("\n");
}
//CHECKSUM
int getchecksum (char *buf, int size)
{
	int i;
	int checksum=0;

	if (size <= 0)
		return 0;
	checksum=buf[0];
	for (i = 1; i < size; i++){
		checksum=checksum+(checksum^buf[i]);
		//performing bitwise XOR and adding it to the checksum
	}
		
	return checksum;
}

int main(int argc, char *argv[])
{
	int fd;
	char buf[] = "Pratyush HarsH";
	int n, c;
	int ret;

	if (argc < 2) {
		fprintf(stderr, "usage: %s <dev>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	ret = open(argv[1], O_RDWR);
	if (ret < 0) {
		perror("open");
		exit(EXIT_FAILURE);
	}
	printf("file %s opened\n", argv[1]);
	fd = ret;

	for (c = 0; c < sizeof(buf); c += n) {
		ret = write(fd, buf + c, sizeof(buf) - c);
		if (ret < 0) {
			perror("write");
			exit(EXIT_FAILURE);
		}
		n = ret;

		printf("wrote %d bytes into file %s\n", n, argv[1]);
		dump("data written are: ", buf + c, n);
	}

	close(fd);
	
	ret = open(argv[1], O_RDWR);
	if (ret < 0) {
		perror("open");
		exit(EXIT_FAILURE);
	}
	printf("file %s reopened\n", argv[1]);
	fd = ret;

	for (c = 0; c < sizeof(buf); c += n) {
		ret = read(fd, buf, sizeof(buf));
		if (ret == 0) {
			printf("read EOF\n");
			break;
		} else if (ret < 0) {
			perror("read");
			exit(EXIT_FAILURE);
		}
		n = ret;

		printf("read %d bytes from file %s\n", n, argv[1]);
		dump("data read are: ", buf, n);
	}
	
	close(fd);
	printf("The String Read is:\n");
	printf("%s\n",buf);
	//Proc file operations
	printf("\n======PROCFILE OPERATIONS======\n");
	char buf2[100];
	int fd2 = open("/proc/mydev", O_RDWR);
	read(fd2, buf2, 100);

	int checksum=getchecksum(buf,n);
	printf("THE CALCULATED CHECKSUM =%d\n",checksum);
	int temp=checksum;
	int len=0;
	while(temp>0){
		len+=1;
		temp/=10;
		}
	printf("The calculated length of checksum integer %d\n",len);
	char str_checksum[len];
	sprintf(str_checksum, "%d",checksum );
	printf("Parameters=%s\n",str_checksum);
	printf("Reading From the procfile:\n");
	lseek(fd, 0 , SEEK_SET);
	write(fd2, str_checksum, len);
	
	lseek(fd2, 0 , SEEK_SET);
	
	read(fd2, buf2, 100);
	puts(buf2);
	printf("\n");
	//IOCTL operations
	int fd3;
        int32_t value, number;
        printf("\n=====IOCTL OPERATIONS=====\n");      
        fd3 = open("/dev/etx_device", O_RDWR);
        if(fd3 < 0) {
                printf("Cannot open device file...\n");
                return 0;
        }

        number=checksum;
	
        printf("\nWriting checksum Value to device file using ioctl call\n");
        ioctl(fd3, WR_VALUE, (int32_t*) &number); 

        printf("\nReading Checksum Value from device file using ioctl\n");
        ioctl(fd3, RD_VALUE, (int32_t*) &value);
        printf("\nValue is %d\n", value);

        printf("\nClosing Driver\n");
        close(fd3);
	return 0;
}
