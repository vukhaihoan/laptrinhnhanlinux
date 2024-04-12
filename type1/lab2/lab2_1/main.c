#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <grp.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

static int err_code;

void handle_error(char* fullname, char* action);
bool test_file(char* pathandname);
bool is_dir(char* pathandname);
const char* ftype_to_str(mode_t mode);
void list_file(char* pathandname, char* name, bool list_long, bool human_readable);
void list_dir(char *dirname, bool list_long, bool list_all, bool recursive, bool human_readable);

#define PATH_MAX 4096 // Max path length in linux
#define USRNAME_MAX 32
#define GRPNAME_MAX 32
#define DATESTR_MAX 15
#define WIDTHSTR_MAX 32

/*
 * You can use the NOT_YET_IMPLEMENTED macro to error out when you reach parts
 * of the code you have not yet finished implementing.
 */
#define NOT_YET_IMPLEMENTED(msg)                  \
    do {                                          \
        printf("Not yet implemented: " msg "\n"); \
        exit(255);                                \
    } while (0)

/*
 * PRINT_ERROR: This can be used to print the cause of an error returned by a
 * system call. It can help with debugging and reporting error causes to
 * the user. Example usage:
 *     if ( error_condition ) {
 *        PRINT_ERROR();
 *     }
 */
#define PRINT_ERROR(progname, what_happened, pathandname)               \
    do {                                                                \
        printf("%s: %s %s: %s\n", progname, what_happened, pathandname, \
               strerror(errno));                                        \
    } while (0)
      
/* PRINT_PERM_CHAR:
 *
 * This will be useful for -l permission printing.  It prints the given
 * 'ch' if the permission exists, or "-" otherwise.
 * Example usage:
 *     PRINT_PERM_CHAR(sb.st_mode, S_IRUSR, "r");
 */
#define PRINT_PERM_CHAR(mode, mask, ch) printf("%s", (mode & mask) ? ch : "-");


static int uname_for_uid(uid_t uid, char* buf, size_t buflen) {
    struct passwd* p = getpwuid(uid);
    if (p == NULL) {
        return 1;
    }
    strncpy(buf, p->pw_name, buflen);
    return 0;
}

static int group_for_gid(gid_t gid, char* buf, size_t buflen) {
    struct group* g = getgrgid(gid);
    if (g == NULL) {
        snprintf(buf,buflen,"%d",gid);
        return 1;
    }
    strncpy(buf, g->gr_name, buflen);
    return 0;
}

static size_t date_string(struct timespec* ts, char* out, size_t len) {
    struct timespec now;
    timespec_get(&now, TIME_UTC);
    struct tm* t = localtime(&ts->tv_sec);
    if (now.tv_sec < ts->tv_sec) {
        // Future time, treat with care.
        return strftime(out, len, "%b %e %Y", t);
    } else {
        time_t difference = now.tv_sec - ts->tv_sec;
        if (difference < 31556952ull) {
            return strftime(out, len, "%b %e %H:%M", t);
        } else {
            return strftime(out, len, "%b %e %Y", t);
        }
    }
}

/*
 * Print help message and exit.
 */
static void help() {
    /* TODO: add to this */
    printf("ls: List files\n");
    printf("\t--help: Print this help\n");
    printf("\t-a ,--all: do not ignore entries starting with .\n");
    printf("\t-l: use a long listing format\n");
    printf("\t-R ,--recursive: list subdirectories recursively\n");
    printf("\t-h ,--human-readable: with -l and -s, print sizes like 1K 234M 2G etc.\n");
    exit(0);
}

void handle_error(char* what_happened, char* fullname) {
    PRINT_ERROR("ls", what_happened, fullname);

    // TODO: your code here: inspect errno and set err_code accordingly.
    return;
}

bool test_file(char* pathandname) {
    struct stat sb;
    if (stat(pathandname, &sb)) {
        handle_error("cannot access", pathandname);
        return false;
    }
    return true;
}

bool is_dir(char* pathandname) {
    /* TODO: fillin */
    struct stat sb;
    if(lstat(pathandname,&sb)==-1){
        PRINT_ERROR("is_dir","couldn't open file or directory",pathandname);
        exit(1);
    }
    return S_ISDIR(sb.st_mode);
}

/* convert the mode field in a struct stat to a file type, for -l printing */
const char* ftype_to_str(mode_t mode) {
    /* TODO: fillin */
    return "?";
}

int maxwidth_sz; 
int maxwidth_usr;
int maxwidth_grp;
// Convert the size in bytes that you want to convert to a string representation
void get_sz_str(size_t size, char *sz_str,size_t len ,bool human_readable){
    if(human_readable){
        char units[]={'\0','K','M','G','T','P','E','Z','Y'};
        int j=0;
        double sz=(double) size;
        while(sz>=1024.0){
            sz/=1024;
            j++;

        }
        snprintf(sz_str,len,"%.1f%c",sz, units[j]);
        
    }else{
        snprintf(sz_str,len,"%ld",size);
    }
}

void list_file(char* pathandname, char* name, bool list_long, bool human_readable) {
    
    if(!list_long){
        printf("%s\n",name);
        return;
    }
    // lstat: retrieves the file's metadata and store in struct stat variable 
    struct stat sb;
    if (lstat(pathandname, &sb) == -1){
        handle_error("couldn't open file or directory", pathandname);
        exit(1);
    }
   
    char permission[11] = "----------";
    permission[1] = S_IRUSR & sb.st_mode ? 'r' : '-';
    permission[2] = S_IWUSR & sb.st_mode ? 'w' : '-';
    permission[3] = S_IXUSR & sb.st_mode ? 'x' : '-';
    permission[4] = S_IRGRP & sb.st_mode ? 'r' : '-';
    permission[5] = S_IWGRP & sb.st_mode ? 'w' : '-';
    permission[6] = S_IXGRP & sb.st_mode ? 'x' : '-';
    permission[7] = S_IROTH & sb.st_mode ? 'r' : '-';
    permission[8] = S_IWOTH & sb.st_mode ? 'w' : '-';
    permission[9] = S_IXOTH & sb.st_mode ? 'x' : '-';
    
    int num_link = sb.st_nlink;
   
    char usrname[USRNAME_MAX];
    uname_for_uid(sb.st_uid, usrname, USRNAME_MAX);
    
    char grpname[GRPNAME_MAX];
    group_for_gid(sb.st_gid, grpname, GRPNAME_MAX);
    
    char datestr[DATESTR_MAX];
    date_string(&sb.st_ctim, datestr, DATESTR_MAX);
    
    long size = sb.st_size;
    char *sz_str=malloc(WIDTHSTR_MAX*sizeof(char));
    get_sz_str(size,sz_str, WIDTHSTR_MAX, human_readable);
    char format_str[30];
    snprintf(format_str, 30, "%%s %%d %%%ds %%%ds %%%ds %%s %%s", maxwidth_usr, maxwidth_grp, maxwidth_sz);
    printf(format_str, permission, num_link, usrname, grpname, sz_str, datestr, name);
    if(S_ISLNK(sb.st_mode)){
        char *link_to = malloc(256*sizeof(char));
        int len;
        if((len=readlink(pathandname,link_to,256))==-1){
            handle_error("read link faild",pathandname);
        }
        link_to[len]='\0';
        printf(" -> %s",link_to);
        free(link_to);
    }
    printf("\n");
    free(sz_str);
    return;
}


void list_dir(char* dirname, bool list_long, bool list_all, bool recursive, bool human_readable) {
    
 
    if(!is_dir(dirname)){
        list_file(dirname,dirname,list_long,human_readable);
        return;
    }
    
    DIR *dirp;
    if((dirp = opendir(dirname)) == NULL) {
        PRINT_ERROR("list_dir","couldn't open file",dirname);
        return;
    }
   
    int i=strlen(dirname);
    if(dirname[i-1]=='/'){
        dirname[i-1]='\0';
    }
    struct stat sb;
    struct dirent *dp;
   
    char *pathandname = (char *)malloc(PATH_MAX * sizeof(char));
    maxwidth_sz=0;
    maxwidth_grp=0;
    maxwidth_usr=0;
    while((dp=readdir(dirp))!=NULL){
        snprintf(pathandname,PATH_MAX,"%s%s%s",dirname,"/",dp->d_name);
        if (lstat(pathandname, &sb) == -1){
            handle_error("couldn't open file or directory", pathandname);
            exit(1);
        }
        int len;
        char *buf = malloc(WIDTHSTR_MAX * sizeof(char));
        
        group_for_gid(sb.st_gid, buf, GRPNAME_MAX);
        len = strlen(buf);
        maxwidth_grp = len > maxwidth_grp ? len : maxwidth_grp;
        
        uname_for_uid(sb.st_uid, buf, USRNAME_MAX);
        len = strlen(buf);
        maxwidth_usr = len > maxwidth_usr ? len : maxwidth_usr;
        
        get_sz_str(sb.st_size, buf, WIDTHSTR_MAX, human_readable);
        len = strlen(buf);
        maxwidth_sz = len > maxwidth_sz ? len : maxwidth_sz;
        free(buf);
    }
    
    if(recursive){
        printf("%s:\n",dirname);
    }
    
    char *fname;
    rewinddir(dirp);
    while((dp=readdir(dirp))!=NULL){
        fname=dp->d_name;
       
        if(fname[0]=='.' && !list_all){
            continue;
        }
        snprintf(pathandname,PATH_MAX,"%s%s%s",dirname,"/",fname);
        list_file(pathandname,fname,list_long,human_readable);
    }
    free(pathandname);
    printf("\n");
    
    if(!recursive){
        closedir(dirp);
        return;
    }
    
    rewinddir(dirp);
    char *nxt_dirname=(char *)malloc(PATH_MAX*sizeof(char));
    while((dp=readdir(dirp))!=NULL){

        fname=dp->d_name;
        
        if(strlen(fname)==2&&fname[0]=='.'&&fname[1]=='.'){
            continue;
        }
        if(strlen(fname)==1&&fname[0]=='.'){
            continue;
        }
        if(fname[0]=='.'&&!list_all){
            continue;
        }

        snprintf(nxt_dirname,PATH_MAX,"%s%s%s",dirname,"/",fname);

        if(!is_dir(nxt_dirname))
            continue;
        
        list_dir(nxt_dirname,list_long,list_all, recursive, human_readable);
    }
    free(nxt_dirname);
    closedir(dirp);
    return;
}

int main(int argc, char* argv[]) {
    
    int opt;
    err_code = 0;
    bool list_long = false, list_all = false, recursive=false, human_readable=false;
   
    struct option opts[] = {
        {.name = "help", .has_arg = no_argument, .flag = NULL, .val = '\a'},
        {.name = "all", .has_arg = no_argument, .flag = NULL, .val ='a'},
        {.name = "human-readable", .has_arg=no_argument,.flag=NULL,.val='h'},
        {.name = "recursive", .has_arg = no_argument, .flag = NULL, .val = 'R'}};

    while ((opt = getopt_long(argc, argv, "1alRh", opts, NULL)) != -1) {
        switch (opt) {
            case '\a':
              
                help();
                break;
            case '1':
                break;
            case 'a':
                list_all = true;
                break;
                
            case 'l':
                list_long = true;
                break;
            case 'R':
                recursive = true;
                break;
            case 'h':
                human_readable = true;
                break;
            default:
                printf("Unimplemented flag %d\n", opt);
                break;
        }
    }
    
    char cwd[PATH_MAX];
    if(getcwd(cwd,PATH_MAX)==NULL){
        PRINT_ERROR("main","get work directory faild","");
        exit(1);
    }
   
    if(optind == argc){
        list_dir(".", list_long, list_all, recursive, human_readable);
    }
    for (int i = optind; i < argc; i++) {
        list_dir(argv[i], list_long, list_all, recursive, human_readable);
    }
    NOT_YET_IMPLEMENTED("handle error");
    exit(err_code);
}
