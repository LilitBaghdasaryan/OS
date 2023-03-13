#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
    int fd[argc - 1][2];
    pid_t p[argc - 1];
    p[0] = fork();
    if(p[0] < 0) 
    {
        perror("Failed to fork child 1.");
        return 1;
    }
    if(p[0] == 0)
    {
        close(fd[1][0]);
        close(fd[1][1]);
        close(fd[0][0]);
        dup2(fd[0][1], STDOUT_FILENO);
        close(fd[0][1]);
        if (execlp(argv[1], argv[1], NULL) < 0) 
        {
            perror("Failed to execute process 1.");
            return 1;
        }
    }

    for(int i = 1; i < argc - 2; ++i)
    {
        p[i] = fork();
        if(p[i] < 0) 
        {
            perror("Failed to fork child.");
            return 1;
        }
        if(p[i] == 0) 
        {
            close(fd[i][1]);
            dup2(fd[i][0], STDIN_FILENO);
            close(fd[i][0]);
            close(fd[i + 1][0]);
            dup2(fd[i + 1][1], STDOUT_FILENO);
            close(fd[i + 1][1]);
            if(execlp(argv[i + 2], argv[i + 2], NULL) < 0) 
            {
                perror("Failed to execute process.");
                return 1;
            }
        }
    }

    if(argc > 2)
    {
    	p[argc - 1] = fork();
    	if(p[argc - 1] < 0) 
    	{
        	perror("Failed to fork last child.");
        	return 1;
    	}
    	if(p[argc - 1] == 0) 
    	{
        	close(fd[argc - 2][0]);
        	close(fd[argc - 2][1]);
        	close(fd[argc - 1][1]);
        	dup2(fd[argc - 1][0], STDIN_FILENO);
        	close(fd[argc - 1][0]);
        	if(execlp(argv[argc - 1], argv[argc - 1], NULL) < 0) 
        	{
            	perror("Failed to execute last process.");
            	return 1;
        	}
    	}
    }
    waitpid(p[0], NULL, 0);
    for(int i = 0; i < argc - 1; ++i)
    {
        close(fd[i][0]);
        close(fd[i][1]);
        close(fd[i + 1][0]);
        close(fd[i + 1][1]);
        waitpid(p[i],NULL, 0);
        return 0;
    }
    waitpid(p[argc - 1], NULL, 0);
}
