#include <fastlog.h>

int main(int argc,char** argv,char ** envp) {
	fastlog_config conf;
	fastlog_config_init(&conf);
	fastlog_init(&conf);
	for(unsigned int i=0;i<100;i++) {
		fastlog_log(&conf,"did you know that %d+%d=%d",i,i+1,i+i+1);
	}
	fastlog_close(&conf);
	return 0;
}
