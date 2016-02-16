// $ ciphertool for OpenBSD,v 1.6 2016/02/16 milo974 Exp $
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

// Please change the PASSWORD and HASH before compile

#define PASSWORD "test" // Application password
#define HASH "XXX" // OpenSSL password

#define EXT ".enc" // Extension for encrypted files
#define OPENSSL "/usr/bin/openssl" // OpenSSL path
#define OPT1 "enc -aes-256-cbc -salt -in" // Option 1 : Encrypt
#define OPT2 "enc -aes-256-cbc -d -in" // Option 2 : Decrypt

int
main(int argc, char *argv[])
{
    char *nfile;
    char openssl_cmd[4096];
    char rm_cmd [4096];
    char *p;
    FILE *ifile;
    int ef;

    if (argc == 1) {
        usage();
    } else {
        p=getpass("Please, enter your password and hit ENTER\n");

        if(strcmp(p,PASSWORD)!=0){
        errx(1,"Bad password");
    }

    for(ef=1; ef<argc; ef++){

        printf("-- File to process : %s\n",argv[ef]);

        ifile=fopen(argv[ef],"r");
        if (ifile==NULL){
                errx(1,"File error");
        }

        char *ext = strrchr(argv[ef],'.');

        if (ext==NULL){
                ext="null";
        }

        if (strcmp(ext,EXT)==0){
            printf("Decrypting...");
            nfile=argv[ef];
            nfile[strlen(nfile)-4]='\0';
            snprintf(openssl_cmd,4096,"%s %s %s%s -out %s -pass pass:%s > /dev/null 1>&1",OPENSSL,OPT2,nfile,EXT,nfile,HASH);
            system(openssl_cmd);
	    printf("OK\n");

        } else {
            printf("Encrypting...");
            snprintf(openssl_cmd,4096,"%s %s %s -out %s%s -pass pass:%s > /dev/null 1>&1",OPENSSL,OPT1,argv[ef],argv[ef],EXT,HASH);
            system(openssl_cmd);
            snprintf(rm_cmd,4096,"rm -f %s",argv[ef]);
            system(rm_cmd);
	    printf("OK\n");
        }


    }

    return 0;
    }
}

usage(void)
{
    (void)fprintf(stderr,
    "usage: ciphertool file ...\n");
    exit(1);
}
