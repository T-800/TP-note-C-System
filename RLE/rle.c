#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>


#define BUFFSIZE 1024


int est_triple(int i, signed char buff[], char c){
    if (i < 2) return 0;
    return c == buff[i-1] && c == buff[i-2];
}

int RLEdecompress(int in, int out){
    int last_ocatet = 0;
    ssize_t lu;
    signed char buf[BUFFSIZE];
    int ERROR = 0;
    // i - last < lu
    while ((lu = read(in, buf, BUFFSIZE))!= 0){
        int i = 0,j;
        if(last_ocatet > 0){
            for (i = 0; i < last_ocatet; i++)
                write(out, buf, 1);
            last_ocatet = 0;
            i=1;
        }
        if(last_ocatet < 0){
            write(out, buf, -last_ocatet);
            i = -last_ocatet;
            last_ocatet = 0;
        }
        while(i < lu){
            int c = buf[i];

            if(c > 0){
                if (i == lu -1){
                    last_ocatet = c;
                    break;
                }
                for(j = 0; j <= c; j++){
                    if(i+1 >= lu){
                        ERROR = 1;
                        break;
                    }
                    write(out, buf+i+1, 1);
                }
                i++;
            }
            else {
                int a = lu - (i+1);
                if (a < -c){
                    if(i+1 >= lu){
                        ERROR = 1;
                        break;
                    }
                    write(out, buf+i+1, a);
                    last_ocatet = c +a;
                    break;
                }
                if(i+1 >= lu){
                        ERROR = 1;
                        break;
                    }
                write(out, buf+1+i, -c);
                i+=-c;
            }
            i++;
        }
        if(ERROR){
            break;
        }
    }
    close(out);
    close(in);
    if(ERROR || last_ocatet != 0){
        return 1;
    }
    return 0;
}


void RLEcompress(int in, int out){
    ssize_t lu;
    signed char bufRead[BUFFSIZE];
    signed char bufWrite[128];
    int poswrite = 0;
    signed char occurence = 0;
    int mode = 1;
    while ((lu = read(in, bufRead, BUFFSIZE))!= 0){
        if (poswrite == 0) mode = -1; // -
        int i;
        for (i=0; i < lu; i++) {
            if( occurence == 128 || occurence == -128 ){
                if(mode == 1){ // +
                    signed char c[1];
                    c[0]= occurence-1;
                    write(out,c, 1);
                    write(out, bufWrite, 1);
                }
                else { // -
                    if(0){
                        // cd]   d]      [d
                        //127  1024     1024
                        
                        
                        
                    }
                    else{
                        signed char c[1];
                        c[0]= -(occurence);
                        write(out,c, 1);
                        write(out, bufWrite, (occurence));
                    }
                }
            }
            // ADD 127
            if(mode == 1){ // +
                if(est_triple(poswrite, bufWrite, bufRead[i])){
                    occurence++;
                }
                else{
                    mode = -1;
                    if (occurence > 2){
                        signed char c[1];
                        c[0]= occurence-1;
                        write(out,c, 1);
                        write(out, bufWrite, 1);
                        bufWrite[0] = bufRead[i];
                    }
                    else{
                        bufWrite[poswrite] = bufRead[i];
                        poswrite++;
                        occurence++;
                        continue;
                    }
                    occurence = 1;
                    poswrite = 1;
                }
            }
            else{ // -
                if(est_triple(poswrite, bufWrite, bufRead[i])){
                    mode = 1;
                    signed char c[1];
                    c[0]= -(occurence-2);
                    write(out,c, 1);
                    write(out, bufWrite, (occurence-2));
                    bufWrite[0] = bufRead[i];
                    bufWrite[1] = bufRead[i];
                    poswrite = 2;
                    occurence = 3;
                }
                else{
                    bufWrite[poswrite] = bufRead[i];
                    poswrite++;
                    occurence++;
                }
            }
        }
    }
    if(mode == 1){ // +
        signed char c[1];
        c[0]= occurence -1;
        write(out,c, 1);
        write(out, bufWrite, 1);
    }
    else{ // -
        signed char c[1];
        c[0]= -(occurence);
        write(out,c, 1);
        write(out, bufWrite, (occurence));
    }
    close(out);
    close(in);
}


int main (int argc, char const *argv[]){
    if(argc != 4){
        printf("usage\n%s [option] fileIn fileOut\n\nOption :\n-c:\tcompresse\n-d:\tdecompresse", argv[0]);
        exit(0);
    }
    if(!(strcmp("-c",argv[1]) != 0 || strcmp("-d",argv[1]) != 0)){
        printf("usage\n%s [option] fileIn fileOut\n\nOption :\n-c:\tcompresse\n-d:\tdecompresse\n", argv[0]);
        exit(-1);
    }
    
    
    int in = open(argv[2], O_RDONLY, 0640);
    int out = open(argv[3], O_WRONLY| O_CREAT | O_TRUNC, 0640);
    if(in == -1){
        printf("Error %s", argv[2]);
        exit(-1);
    }
    if(out == -1){
        printf("Error %s", argv[3]);
        exit(-1);
    }
    int retour = 0;
    if(strcmp("-c",argv[1]) == 0){
        RLEcompress(in, out);
    }
    else {
        retour= RLEdecompress(in, out);
    }
    if (retour){
        unlink(argv[3]);
        printf("Erreur lors de la décompréssion de %s\nle fichier va être supprimé\n", argv[3]);
        exit(-1);
    }
}