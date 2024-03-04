#include <lib.h>
#include <fs.h>
int flag[256];

void rm_default(char *);
void no_such_file(char *);
void no_is_dir(char *);
void remove_file(char *);
void rm_d_dir(char *);
void no_is_emp_dir(char *);
void rm_r(char *, char *);
char *strcat_r(char *, char *);


char *strcat_r(char *dst, char *src) {
    	char *address = dst;
	char *sep;
	if (dst[0] && dst[strlen(dst)-1]!='/') {
		dst[strlen(dst)] = '/';
	}
    	while (*dst) {
        	dst++;
    	}
	    while ((*dst++ = *src++) != 0) {
    	}
    	return address;
}

void no_such_file(char *path){
	printf("rm:_cannot_remove_'%s':_no_such_file_or_directory\n", path);
}

void no_is_dir(char *path){
	printf("rm:_cannot_remove_'%s':_is_a_directory\n", path);
}

void no_is_emp_dir(char *path) {
	printf("rm:_cannot_remove_'%s':_directory_is_not_empty\n", path);
}

void remove_file(char *path) {
	remove(path);
	printf("rm:_successfully_removed_'%s'\n", path);
}

void rm_r(char *path,char *prefix){
	int r; 
 	struct Stat st;
 	if ((r = stat(path, &st))<0) {
 		no_such_file(path);
             	return;
         }
          if (!st.st_isdir) {
           	remove_file(path);
                return;
         }
	  int fd, n;
	  struct File f;
          //int empty_flag = 1;
          if ((fd = open(path, O_RDONLY)) < 0) {
                  user_panic("open %s: %d", path, fd);
          }
          while ((n = readn(fd, &f, sizeof f)) == sizeof f) {
                  if (f.f_name[0]) {
                          char *pathnext;
			  int j;
			  for (j=0;j<strlen(path);j++) {
				pathnext[j] = path[j];
				 }
				pathnext[j] = '\0';
			  //memcpy(pathnext,path,strlen(path));
			  char *pathnext2 = strcat_r(pathnext,f.f_name);
			  if (f.f_type == FTYPE_DIR) {
				rm_r(pathnext2,pathnext2);	
				} else {
				remove_file(pathnext2);
			}
			  
			  //ls1(prefix, f.f_type == FTYPE_DIR, f.f_size, f.f_name);
                          //empty_flag = 0;
                          //break;
                  }
          }
	  remove_file(path);
}


void rm_d_dir(char *path) {
	int r;
	struct Stat st;
	if ((r = stat(path, &st))<0) {
		no_such_file(path);
		return;
	}
	if (!st.st_isdir) {
		remove_file(path);
		return;
	}
	int fd, n;
	struct File f;
	int empty_flag = 1;

	if ((fd = open(path, O_RDONLY)) < 0) {
		user_panic("open %s: %d", path, fd);
	}
	while ((n = readn(fd, &f, sizeof f)) == sizeof f) {
		if (f.f_name[0]) {
			//ls1(prefix, f.f_type == FTYPE_DIR, f.f_size, f.f_name);
			empty_flag = 0;
			break;
		}
	}
	if (empty_flag) {
		remove_file(path);
		return;		
	} else {
		no_is_emp_dir(path);
		return;
	}

}

void rm_default(char *path) {
	int r;
	struct Stat st;
	if ((r = stat(path, &st)) < 0) {
		no_such_file(path);
		return;
	}
	if (st.st_isdir) {
		no_is_dir(path);
		return;
	}
	remove_file(path);
}

void usage(void) {
	printf("Usage: rm.b [-dhr]... file(s)...\n");
	exit();
}


int main(int argc, char **argv) {
	int i;
	int default_flag = 1;

	ARGBEGIN {
	case 'h':
		usage();
		break;
	case 'd':
	case 'r':
		default_flag = 0;
		flag[(u_char)ARGC()]++;
		break;
	default:
		//printf("in_deafault_case\n");
		default_flag = 1;
		break;
	}
	ARGEND
//	if (argc==1 && argv[0][0]!='-') {
//		for (i=0;i<argc;i++) {
//
	//	}
	//}
	//for (i=0;i<argc;i++)
	//	printf("argv[%d]:%s\n",i,argv[i]);
	if (argc == 0) {
		//ls("/", "");
	} else {
		for (i = 0; i < argc; i++) {
			//ls(argv[i], argv[i]);
			if (default_flag) {
				//printf("argc[%d]:%s\n",i,argv[i]);
				rm_default(argv[i]);
			} else if (flag['d']){
				rm_d_dir(argv[i]);
			} else {
				rm_r(argv[i],argv[i]);
			}
		}
	}
	//printf("\n");
	return 0;
}

