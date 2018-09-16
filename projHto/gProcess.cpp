#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <iostream>
#include<sys/time.h>
#include<sys/resource.h>
#include<string.h>
#include <stdlib.h>

using namespace std;


//listar processos ordenados pelo resident set size, the non-swapped physical memory that
string listPCommand = "ps -eo user,comm,pid,psr,pcpu,pri,ni,cputime,time --sort -rss | head -20";

//mudar cpu processo
void task_set(int pid,int cpu) {
        int result;
        cpu_set_t  mask;
        CPU_ZERO(&mask);
        CPU_SET(cpu, &mask);
        if((sched_setaffinity(pid, sizeof(mask), &mask)) == 0){
            cout<<"Done\n";
            //dar cls
        }else{
            cout<<"Error\n";
        }
}


int main(){
    int escolha,newCpu,newPrio;
    pid_t pidShot;
    bool error=0;
    do{
        system("clear");
        system(listPCommand.c_str());
        if(error){
            cout<<"Operação inválida!\n";
            error!=error;
        }
        cout<<"---------\n1-Pausar | 2-Continuar | 3-Parar | 4-Mudar Psr | 5-Mudar Pri | 6-Mudar Visu(tree)\n7-Exit\n";
        cout<<"Digite sua escolha: ";
        cin>>escolha;
        while(escolha>7 || escolha<1){
            cout<<"Operação inválida, digite novamente.\n";
            cin>>escolha;
        }
        if(escolha == 7){
            exit(1);
        }
        cout<<endl<<"Digite o PID do processo: ";
        cin>>pidShot;
        switch(escolha){
            case 1:
                if(!(kill(pidShot,SIGSTOP))){
                    cout<<"Parei! \n";
                }else{
                    cout<<"Operacao invalida\n";
                }
                break;
            case 2:
                if(!(kill(pidShot,SIGCONT))){
                    cout<<"Continuei! \n";
                }else{
                    cout<<"Operacao invalida\n";
                }
                break;
            case 3:
                if(!(kill(pidShot,SIGKILL))){
                    cout<<"Matei! \n";
                }else{
                    cout<<"Operacao invalida\n";
                }
                break;
            case 4: 
                cout<<"Digite a nova CPU: ";
                cin>>newCpu;
                while(newCpu>3 || newCpu<0){ //meu pc
                    cout<<"CPU invalida, digite novamente\n";
                    cin>>newCpu;
                }
                //testar se é valida "nproc --all"
                task_set(pidShot,newCpu);
                break;
            case 5:
                cout<<"Digite o novo valor Nice (inv. prop. à prioridade): ";
                cin>>newPrio;
                if(setpriority(PRIO_PROCESS, pidShot, newPrio) == -1){
                    cout<<"Erro ao setar prioridade, super user?\n";
                }
            /*
            case 6:
                //fazer filtro
            case 7:
                //fazer tree
            case 8: 
                //fazer grafico %cpu
            */
            default:
                cout<<"Escolha invalida\n";
                error!=error;
        }
    }while(1);
}
        