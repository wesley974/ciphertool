// $ ciphertool for MAC OS,v 1.5 2016/02/06 milo974 Exp $
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
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/param.h>

// BEFORE COMPILE THIS FILE, PLEASE FILL ALL 'XXX'

#define PASSWORD "XXX" // The password that protect the use of this tool
#define HASH "XXX" // This is used to encrypt/decrypt your files
#define HOSTNAME "XXX" // The hostname where ciphertool lives
#define LOGIN "XXX" // Your login name (username)
#define EXT ".enc" // The extension for encrypted files
#define OPENSSL "/usr/bin/openssl" // The path for openssl
#define OPT1 "enc -aes-256-cbc -salt -in" // Option 1 : Encrypt!
#define OPT2 "enc -aes-256-cbc -d -in" // Option 2 : Decrypt !

int prterr();

int main (){

char commandline[4096];
char rm[4096];
char file[MAXPATHLEN],newfile[MAXPATHLEN];
char hostname[128],*myname,*response;
FILE *pFile;

gethostname(hostname, sizeof hostname);
myname=getenv("LOGNAME");

// test hostname and username 

if(strcmp(hostname,HOSTNAME)!=0 ||strcmp(myname,LOGIN)!=0){
prterr();
}

// ask password

printf("-- Cypher Tool by Wesley MOUEDINE ASSABY © 2014\n");
response=getpass("Please, enter your password and hit ENTER");
if(strcmp(response,PASSWORD)!=0){
prterr();
}

// ask for file

printf("\nYou can drag-and-drop your file or type the full path of your file*\n");
printf("File: ");
fgets(file, sizeof(file), stdin);
if (file[strlen(file) - 1] == '\n') {
file[strlen(file) - 1] = '\0';
}

// test if the filename contain a space at the end

if (file[strlen(file) - 1] == ' ') {
printf("Removing space in filename...\n");
file[strlen(file) - 1] = '\0';
}

// test if the file exists

pFile=fopen(file,"r");
if (pFile==NULL){
prterr();
}

// test if the file has no extension

char *ext = strrchr(file,'.');
if (!ext){
prterr();
}
else{
printf("Extension : %s\n",ext);
}

// compare extension with EXT

if (strcmp(ext,EXT)==0){

// match EXT
// decrypt

printf("Decrypting process ...\n");
strcpy(newfile,file);
newfile[strlen(newfile)-4]='\0';
printf("The new file is : %s\n",newfile);

snprintf(commandline,4096,"%s %s %s -out %s -pass pass:%s > /dev/null 1>&1",OPENSSL,OPT2,file,newfile,HASH);
system(commandline);
printf("%s generated.\n",newfile);
printf("Done.\n");

}
else{

// encrypt

printf("Encrypting process...\n");
strcpy(newfile,file);
snprintf(commandline,4096,"%s %s %s -out %s%s -pass pass:%s > /dev/null 1>&1",OPENSSL,OPT1,file,newfile,EXT,HASH);
system(commandline);
printf("%s%s generated.\n",newfile,EXT);
snprintf(rm,4096,"rm -f %s",file);
system(rm);
printf("The decrypted file %s has been deleted.\n",file);
printf("Done.\n");

}


//EOF
}


// function : prterr

int prterr(){
printf("Wrong way.\n");
exit (1);
}
