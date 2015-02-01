#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#define PATH_MAX 4096

typedef int boolean;
#define true 1
#define false 0

boolean check_file_folder(struct stat file_folder_info, uid_t user, gid_t groupe){
    if(file_folder_info.st_uid == user && file_folder_info.st_mode & S_IXUSR) return true;
    else if(file_folder_info.st_gid == groupe && file_folder_info.st_mode & S_IXGRP) return true;
    else if(file_folder_info.st_mode & S_IXOTH && file_folder_info.st_uid != user && file_folder_info.st_gid != groupe) return true;
    return false;
}

char* check_arbo(char* relatif){

    int result = chdir(relatif);
    if (result == 0){
        return getcwd(relatif, PATH_MAX);
    }
    else{
        fprintf(stderr,"./lsx: %s: acces impossible\n",relatif); 
        exit(0);
    }
}

void parc(const char* rep){
    DIR *dir;
    dir = opendir(rep);
    struct dirent *entry;
    struct stat tmp;
    char *chemin;

    while((entry = readdir(dir)) != NULL){
        chemin = malloc(sizeof(char)*(strlen(rep)+strlen(entry->d_name)+2));
        sprintf(chemin, "%s/%s",rep,entry->d_name);
        lstat(chemin, &tmp);
        if(strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name,".." ) == 0){
            continue;
        }
        if(check_file_folder(tmp, getuid(), getgid())){
            if(!S_ISDIR(tmp.st_mode))
                fprintf(stdout,"%s\n",chemin);
            else parc(chemin);
        }
        else {
            if(S_ISDIR(tmp.st_mode))
                fprintf(stderr,"./lsx: %s: acces impossible\n",chemin);
        }
        free(chemin);
    }
    closedir(dir);
    free(entry);
}

int main (int argc, char const *argv[]){
    struct stat tmp;
    if(argc != 2){
        fprintf(stderr,"usage : lsx <reference>\n");
        exit(0);
    }
    if(lstat(argv[1], &tmp) == -1){
        fprintf(stderr,"lsx: %s: reference non valide\n",argv[1]);
        exit(0);
    }
    char * abs = check_arbo((char *)argv[1]);
    parc(abs);
    return 0;
}
