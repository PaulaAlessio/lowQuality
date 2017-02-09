/** Uncompress input files using pipes.
 * Hook the standard file opening functions, open, fopen and fopen64.
 * If the extension of the file being opened indicates the file is
 * compressed (.gz, .bz2, .xz), open a pipe to a program that
 * decompresses that file (gunzip, bunzip2 or xzdec) and return a
 * handle to the open pipe.
 * @author Shaun Jackman <sjackman@bcgsc.ca>
 */

#include "fopen_gen.h"

static const char* zcatExec(const char* path)
{
        int strl = strlen(path);
        return (!strcmp(path + strl - 3, ".ar")) ? "ar -p" :
                       (! strcmp(path + strl - 4, ".tar")) ? "tar -xOf" :
                       (! strcmp(path + strl - 6, ".tar.Z")) ? "tar -zxOf" :
                       (! strcmp(path + strl - 7, ".tar.gz"))? "tar -zxOf" :
                       (! strcmp(path + strl - 8, ".tar.bz2")) ? "tar -jxOf" :
                       (! strcmp(path + strl - 7, ".tar.xz")) ? "tar --use-compress-program=xzdec -xOf" :
                       (! strcmp(path + strl - 2, ".Z")) ? "gunzip -c" :
                       (! strcmp(path + strl - 3, ".gz")) ? "gunzip -c" :
                       (! strcmp(path + strl - 4, ".bz2")) ? "bunzip2 -c" :
                       (! strcmp(path + strl - 3, ".xz")) ? "xzdec -c" :
                       (! strcmp(path + strl - 4, ".zip")) ? "unzip -p" :
                       (! strcmp(path + strl - 4, ".bam")) ? "samtools view -h" :
                       (! strcmp(path + strl - 3, ".jf")) ? "jellyfish dump" :
                       (! strcmp(path + strl - 4, ".jfq")) ? "jellyfish qdump" :
                       (! strcmp(path + strl - 4, ".sra")) ? "fastq-dump -Z --split-spot" :
                       (! strcmp(path + strl - 4, ".url")) ? "wget -O- -i" : NULL;
}

/** Open a pipe to uncompress the specified file.
 * Not thread safe.
 * @return a file descriptor
 */
static int uncompress(const char *path)
{
	const char *zcat = zcatExec(path);
	assert(zcat != NULL);

	int fd[2];
	if (pipe(fd) == -1)
		return -1;
	int err = setCloexec(fd[0]);
	assert(err == 0);
	(void) err;

	char arg0[16], arg1[16], arg2[16];
	int n = sscanf(zcat, "%s %s %s", arg0, arg1, arg2);
	assert(n == 2 || n == 3);

	/* It would be more portable to use fork than vfork, but fork can
	 * fail with ENOMEM when the process calling fork is using a lot
	 * of memory. A workaround for this problem is to set
	 * sysctl vm.overcommit_memory=1
	 */
#if HAVE_WORKING_VFORK
	pid_t pid = vfork();
#else
	pid_t pid = fork();
#endif
	if (pid == -1)
		return -1;

	if (pid == 0) {
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		if (n == 2)
			execlp(arg0, arg0, arg1, path, NULL);
		else
			execlp(arg0, arg0, arg1, arg2, path, NULL);
		// Calling perror after vfork is not allowed, but we're about
		// to exit and an error message would be really helpful.
		perror(arg0);
		_exit(EXIT_FAILURE);
	} else {
		close(fd[1]);
		return fd[0];
	}
}

/* Set the FD_CLOEXEC flag of the specified file descriptor. */
int setCloexec(int fd)
{
    int flags = fcntl(fd, F_GETFD, 0);
    if (flags == -1)
          return -1;
    flags |= FD_CLOEXEC;
    return fcntl(fd, F_SETFD, flags);
}

/** Open a pipe to uncompress the specified file.
 * @return a FILE pointer
 */
static FILE* funcompress(const char* path)
{
	int fd = uncompress(path);
	if (fd == -1) {
		perror(path);
		exit(EXIT_FAILURE);
	}
	return fdopen(fd, "r");
}

FILE* fopen_gen(const char *path, const  char * mode){

   // Check if the file exists 
   FILE* f = fopen(path, mode);
   if (f == NULL) {
       fprintf(stderr, "Error opening file: %s\n", path);
       _exit(EXIT_FAILURE);
   } 
   if (f && zcatExec(path) != NULL && (!strcmp(mode,"r"))){
      fclose(f);
      return funcompress(path);
   } else{
      return f;  
   }

}
