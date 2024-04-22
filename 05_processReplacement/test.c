#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t id = fork();
    assert(id != -1);
        
    if (id == 0) {
        //execl("./myproc", "myproc", NULL);
        //execlp("ls", "ls", "-a", "-l", "--color=auto", NULL);
        
        //char* const argv_[] = {
        //    (char*)"ls",
        //    (char*)"-a",
        //    (char*)"-l",
        //    (char*)"--color=auto",
        //    NULL
        //}; 

        //execv("/usr/bin/ls", argv_); 
        
        //char* const envp_[] = {
        //    (char*)"MYENV=123456",
        //    NULL
        //};
        //execle("./myproc", "myproc", NULL, envp_);
        
        extern char** environ;
        //execle("./myproc", "myproc", NULL, environ);
        
        putenv((char*)"MYENV=123");
        execle("./myproc", "myproc", NULL, environ);

        exit(1);
    }

   int status = 0;
   pid_t ret = waitpid(id, &status, 0);
   if (ret > 0) {
       printf("exit_code: %d, signal_code: %d\n", (status >> 8) & 0xFF, status & 0x7F);
   }


    return 0;
}
