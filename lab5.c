#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

char* formatdate(char* str, time_t val)
{
        strftime(str, 36, "%d.%m.%Y %H:%M:%S", localtime(&val));
        return str;
}

int main(int argc, char *argv[])
{
	
    DIR *mydir;
    struct dirent *myfile;
    struct stat mystat;
	
	struct passwd *pw;
	struct group  *gr;

	if (argc != 2) {
	   fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
	   exit(EXIT_FAILURE);
	}

	if (stat(argv[1], &mystat) == -1) {
	   perror("stat");
	   exit(EXIT_FAILURE);
	}
	
	if(strstr(argv[1], ".") != NULL) {
		//printf("It's a file.\n");
		printf("File type:                ");

		switch (mystat.st_mode & S_IFMT) {
		case S_IFBLK:  printf("block device\n");            break;
		case S_IFCHR:  printf("character device\n");        break;
		case S_IFDIR:  printf("directory\n");               break;
		case S_IFIFO:  printf("FIFO/pipe\n");               break;
		case S_IFLNK:  printf("symlink\n");                 break;
		case S_IFREG:  printf("regular file\n");            break;
		case S_IFSOCK: printf("socket\n");                  break;
		default:       printf("unknown?\n");                break;
		}

		struct passwd *pw = getpwuid(mystat.st_uid);
		struct group  *gr = getgrgid(mystat.st_gid);

		printf("File name:                %s\n", argv[1]);
		printf("I-node number:            %ld\n", (long) mystat.st_ino);

		printf("Mode:                     %lo (octal)\n",
			   (unsigned long) mystat.st_mode);

		printf("Link count:               %ld\n", (long) mystat.st_nlink);
		printf("Ownership:                UID=%ld   GID=%ld\n",
			   (long) mystat.st_uid, (long) mystat.st_gid);
		printf("Ownership:                UID=%s   GID=%s\n",
			   pw->pw_name, gr->gr_name);
		printf("Preferred I/O block size: %ld bytes\n",
			   (long) mystat.st_blksize);
		printf("File size:                %lld bytes\n",
			   (long long) mystat.st_size);
		printf("Blocks allocated:         %lld\n",
			   (long long) mystat.st_blocks);

		printf("Last status change:       %s", ctime(&mystat.st_ctime));
		printf("Last file access:         %s", ctime(&mystat.st_atime));
		printf("Last file modification:   %s", ctime(&mystat.st_mtime));
	}else{
		char argnew[2];
		
		char date[36];
		char buf[512];
		char saved_buf[512];
		char saved_buf1[512];
		char saved_buf2[512];
		char saved_buf3[512];
		char saved_buf4[512];
		char saved_buf5[512];
		saved_buf[0] = 0;
		saved_buf1[0] = 0;
		saved_buf2[0] = 0;
		saved_buf3[0] = 0;
		saved_buf4[0] = 0;
		saved_buf5[0] = 0;
		int dir_count = 0, i = 0;
		mydir = opendir(argv[1]);
		//printf("File Type\tMode(octal)\tLinks\tUID\t\tGID\t\tSize\tBlock\tDate\tName\n");
		printf("File or Folder: %s\n", argv[1]);
		printf("File Type\tMode(octal)\tLinks\tUID\t\tGID\t\tSize\tBlock\tLast Modified\t\tName\n");
		while((myfile = readdir(mydir)) != NULL)
		{
			sprintf(buf, "%s/%s", argv[1], myfile->d_name);
			stat(buf, &mystat);
			//printf("File type:                ");

			switch (mystat.st_mode & S_IFMT) {
			case S_IFBLK:  printf("block device\t");            break;
			case S_IFCHR:  printf("character device\t");        break;
			case S_IFDIR:  printf("directory\t");               break;
			case S_IFIFO:  printf("FIFO/pipe\t");               break;
			case S_IFLNK:  printf("symlink\t");                 break;
			case S_IFREG:  printf("regular file\t");            break;
			case S_IFSOCK: printf("socket\t");                  break;
			default:       printf("unknown?\t");                break;
			}
			
			pw = getpwuid(mystat.st_uid);
			gr = getgrgid(mystat.st_gid);
			printf("%lo\t", (unsigned long) mystat.st_mode);
			printf("\t%ld\t", (long) mystat.st_nlink);
			printf("%s\t\t",pw->pw_name);
			printf("%s\t",gr->gr_name);
			printf("%ld\t", (long) mystat.st_blksize);
			printf("%zu\t",mystat.st_size);
			//printf("%s\t", ctime(&mystat.st_mtime));
			printf("%s\t", formatdate(date, mystat.st_mtime));
			printf("%s\n", myfile->d_name);
			
			if (((int) mystat.st_mode == 16888) && (strcmp(myfile->d_name, ".") != 0) && (strcmp(myfile->d_name, "..") != 0)){
			//if ((int) mystat.st_mode == 16888){ // decimal for 40770 octal
			//if (strcmp((str)mystat.st_mode, "40770") == 0){
				//printf("Found a folder inside.\n");
				//argnew[0] = argv[0];
				//argnew[1] = myfile->d_name;
				//printf("%s",argv[1]);
				//sprintf(argv[1], "%s%s", argv[1], "");
				//argv[1] = myfile->d_name;
				if(dir_count == 0) sprintf(saved_buf, "%s", buf);
				else if (dir_count == 1) sprintf(saved_buf1, "%s", buf);
				else if (dir_count == 2) sprintf(saved_buf2, "%s", buf);
				else if (dir_count == 3) sprintf(saved_buf3, "%s", buf);
				else if (dir_count == 4) sprintf(saved_buf4, "%s", buf);
				else if (dir_count == 5) sprintf(saved_buf5, "%s", buf);
				dir_count++;
				//saved_buf
				//argv[1] = buf;
				//main(argc, argv);
			}
		}
		closedir(mydir);
		//printf("%d", dir_count);
		for(i=0; i<dir_count; i++){
			//printf("Traversing directory.");
			//if(saved_buf[0] != 0) {
				printf("\n");
				//argv[1] = saved_buf;
				if(i == 0) argv[1] = saved_buf;
				else if (i == 1) argv[1] = saved_buf1;
				else if (i == 2) argv[1] = saved_buf2;
				else if (i == 3) argv[1] = saved_buf3;
				else if (i == 4) argv[1] = saved_buf4;
				else if (i == 5) argv[1] = saved_buf5;
				main(argc, argv);
			//}
		}
	}
	exit(EXIT_SUCCESS);
}