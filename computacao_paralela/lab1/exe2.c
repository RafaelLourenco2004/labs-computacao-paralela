# include <stdio.h>
# include <unistd.h>
# include <sys/wait.h>

int main(){
    pid_t pid;
    int n = 4;
    for (int i=0; i<n; i++){
        pid = fork();

        if (pid == 0){
            printf("Eu sou o filho, meu pid é %d e o do meu pai é %d\n", getpid(), getppid());
            return 0;
        }

        if (pid < 0){
            printf("Fork falhou.");
            return -1;
        }
    }
    
    for (int i=0; i<n; i++)
        wait(NULL);

    printf("Eu sou o pai e todos os meus filhos terminaram.");

    return 0;
}