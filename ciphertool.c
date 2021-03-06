// $ ciphertool for OpenBSD,v 1.7 2016/03/01 wesley Exp $
//
// Copyright (c) 2016 Wesley MOUEDINE ASSABY <milo974@gmail.com>
//
// Permission to use, copy, modify, and distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// Please change the PASSWORD and HASH before compile

#define PASSWORD "YOUR_PASSWORD" // Application password
#define HASH "pass:YOUR_HASH" // OpenSSL password

#define EXT ".enc" // Extension for encrypted files
#define MAXPATHLEN 4096

int
main(int argc, char *argv[])
{

    void usage(void);
    void protect(void);

    pid_t pid;
    int status,e;
    char file[MAXPATHLEN];
    char nfile[MAXPATHLEN];

    FILE *ifile;

    if (pledge("stdio rpath proc exec cpath wpath tty ioctl",NULL) == -1)
        err(1,"pledge");

    if (argc == 1) {
        usage();
        } else {

            for(e=1; e<argc; e++){

                strlcpy(file,argv[e],sizeof(file));
                strlcpy(nfile,file,sizeof(nfile));

                printf("-- File to process : %s\n",file);

                ifile=fopen(file,"r");

                if (ifile==NULL)
                    errx(1,"File error");

                char *ext = strrchr(file,'.');

                if (ext==NULL)
                    ext="null";


                if (strcmp(ext,EXT)==0){
                    protect();
                    printf("\tDecrypting...");
		    fflush(stdout);

                    nfile[strlen(nfile)-4]='\0';

                    if ((pid = fork())<0)
                        errx(1,"fork failure");

                    if (pid == 0)
                        execl("/usr/bin/openssl","enc","-aes-256-cbc","-d","-in",file,"-out",nfile,"-pass",HASH,NULL);

		    wait(&status);
                    if ((WIFEXITED(status)) && ((WEXITSTATUS(status))==0)){
                        printf("OK\n\n");
                        } else {
                            printf("error\n");
			    exit(1);
                    }

                } else {

                    printf("\tEncrypting...");
		    fflush(stdout);

                    strlcat(nfile,EXT,sizeof(nfile));

                    if ((pid = fork())<0)
                        errx(1,"fork failure");

                    if (pid == 0)
                        execl("/usr/bin/openssl","enc","-aes-256-cbc","-salt","-in",file,"-out",nfile,"-pass",HASH,NULL);

		    wait(&status);
                    if ((WIFEXITED(status)) && ((WEXITSTATUS(status))==0)){
                        printf("OK\n\n");
                        status = unlink(file);
                        if (status > 0)
                                errx(1,"delete error");

                        } else {
                            printf("error\n");
                            exit(1);
                    }

            }

        }

    }
return 0;
}


void usage(void)
{
    (void)fprintf(stderr,
    "usage: ciphertool file ...\n");
    exit(1);
}


void protect(void)
{
    char *p;
    static int j = 0;
    j++;
    if (j == 1){
        p=getpass("Please, enter your password and hit ENTER\n");
        if(strcmp(p,PASSWORD)!=0){
            errx(1,"Bad password");
        }
    }
}
