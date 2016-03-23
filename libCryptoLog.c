#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

// libCryptoLog 0.1 <yjesus@security-projects.com>

int filedesyes[2] = {3, 10};

char *command = "perl /usr/local/bin/rsacrypt.pl \"";
char *endcommand ="\"";

int isvalueinarray(int val, int *arr, int size){
	int i;
	for (i=0; i < size; i++) {
		if (arr[i] == val)
		return (1);
	}
	return (0);
}


// from http://www.ioncannon.net/programming/34/howto-base64-encode-with-cc-and-openssl/

char *base64(const char *input, int length)
{
	BIO *bmem, *b64;
	BUF_MEM *bptr;

	b64 = BIO_new(BIO_f_base64());
	bmem = BIO_new(BIO_s_mem());
	b64 = BIO_push(b64, bmem);
	BIO_write(b64, input, length);
	BIO_flush(b64);
	BIO_get_mem_ptr(b64, &bptr);

	char *buff = (char *)malloc(bptr->length +1);
	memcpy(buff, bptr->data, bptr->length);
	buff[bptr->length] = 0;

	BIO_free_all(b64);

	return buff;
}

//

ssize_t write(int fildes, const void *buf, size_t nbyte) {
	
	
	int shouldcrypt ;

	shouldcrypt = isvalueinarray(fildes, filedesyes, 2) ;
	
	if (shouldcrypt == 1) {
		
		const char * buffer = (const char *)buf ;
		
		char *based = base64(buffer, nbyte);		
		
		char toexecute[strlen(based)+strlen(command)] ;
		memset(toexecute, '\0', strlen(based)+strlen(command));
	
		FILE *fp;
		
		char retcommand[2048];
		memset(retcommand, '\0', 2048);
		
		strncat(toexecute, command, strlen(command));
		strncat(toexecute, based, strlen(based));
		strncat(toexecute, endcommand, 2);
		
	
		int *(*original_write)(int fildes, const void *buf, size_t nbyte);
	
		original_write = dlsym(RTLD_NEXT, "write");
	
		fp = popen(toexecute, "r");
		if (fp == NULL) {
			printf("Failed to run command\n" );
			exit(1);
		}
	
		while (fgets(retcommand, sizeof(retcommand)-1, fp) != NULL) {
			
			original_write(fildes, retcommand, strlen(retcommand)) ;

		}
	

		pclose(fp);
	
		return(nbyte) ;
		
	}
	
	else {
		
		int *(*original_write)(int fildes, const void *buf, size_t nbyte);
	
		original_write = dlsym(RTLD_NEXT, "write");
		
		original_write(fildes, buf, nbyte) ;
		
		
		return(nbyte);
	}
	
}

int fprintf(FILE *stream, const char *format, ...) {
	
	va_list list;
        char *parg;
	
	va_start(list, format);
	
        vasprintf(&parg, format, list);
        va_end(list);
	
	char *based = base64(parg, strlen(parg));
	
	char toexecute[strlen(based)+1000] ;
	
	snprintf(toexecute, strlen(command)+strlen(based)+10, "%s %s\"", command, based);
	
	int *(*original_fprintf)(FILE *stream, const char *format, ...);
	
	original_fprintf = dlsym(RTLD_NEXT, "fprintf");
	
	FILE *fp;

        char retcommand[2048];

        fp = popen(toexecute, "r");
        if (fp == NULL) {
                printf("Failed to run command\n" );
                exit(1);
        }

	
        while (fgets(retcommand, sizeof(retcommand)-1, fp) != NULL) {
		
		original_fprintf(stream, "%s", retcommand) ;
        }
	
        pclose(fp);
	
	return(0) ;
      
}
	
