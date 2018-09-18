#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <iostream>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>
#include <stdlib.h>
#include <cstdlib>
#include <curses.h>
#include <math.h> 

#include <fstream>
#include <sstream>
using namespace std;

#include <termio.h>
#include <unistd.h>

//the following are UBUNTU/LINUX ONLY terminal color codes.
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

//listar processos ordenados pelo resident set size, the non-swapped physical memory that
string listPCommand = "ps -aeo user,comm,pid,psr,pcpu,pri,ni,cputime,time --sort -rss | head -30";

//mudar cpu processo
void task_set(int pid,int cpu) {
        int result;
        cpu_set_t mask;
        CPU_ZERO(&mask);
        CPU_SET(cpu, &mask);
        if((sched_setaffinity(pid, sizeof(mask), &mask)) == 0){
            cout<<"Done\n";
            system("clear");
        }else{
            cout<<"Error\n";
        }
}


int main(){
    int escolha,newCpu,newPrio;
    pid_t pidShot;
    string palavraChave;
    bool error=0;
    do{
        system("clear");
        system(listPCommand.c_str());
        if(error){
            cout<<"Operação inválida!\n";
            error!=error;
        }
        cout << BOLDRED << "---------\n1-Pausar | 2-Continuar | 3-Parar | 4-Mudar Psr | 5-Mudar Pri | 6-Tree | 7 - Filtro | 8- %CPU \n 0-Exit\n";
        cout<<"Digite sua escolha: " << RESET;
        cin>>escolha;
        while(escolha>8 || escolha<0){
            cout<<"Operação inválida, digite novamente.\n";
            cin>>escolha;
        }
        
        if(escolha < 7 && escolha != 0){
            cout<<endl<<"Digite o PID do processo: ";
            cin>>pidShot;
        }
        
        switch(escolha){
            case 0:
                cout << "Saindo..." << endl;
                exit(1);
                break;
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
                break;
            
            case 6:
                char treeCommand[40];
                sprintf(treeCommand, "pstree %d -u", pidShot);
                system((treeCommand));
                system("read -p \"Pressione enter para sair\" saindo");
                break;
                
            case 7:
                cout<< "Digite uma palavra-chave para a busca: ";
                cin >> palavraChave;
                cout << endl << "Resultado: " << endl;
                system((listPCommand + " | grep " + palavraChave).c_str());
                //system("pause");
                system("read -p \"Pressione enter para sair\" saindo");
                break;
            
            case 8: 
            { //precisa dessas chaves para especificação de escopo das variáveis desse caso
                ifstream infile("/proc/stat");
                const int nCores = 4; //Deixar nCores estático enquanto não descobrir um jeito de indentificar quantos cores existem
                string line;
                string lines[30];
                string tokenToSearch[nCores];
                
                string splitChar = " ";
                string token;
                string item[10];

                int total[nCores] = {0,0,0,0};
                int idle[nCores] = {0,0,0,0};
                
                int nLine = 0;
                float percent;
                //prepara os tokens para serem identificados
                for (int i = 0; i < nCores; i++){
                    tokenToSearch[i] = "cpu" + to_string(i);
                }
                
                //le linha a linha o arquivo
                while(getline(infile, line)){
                    istringstream iss(line);
                    //passa o stream para um vetor de strings
                    if(!(iss >> lines[nLine])){iss.clear();break;}
                    
                    //stream para pegar as informações da linha
                    stringstream rightLine(line);
                    
                    //checa o cabeçalho da linha
                    token = lines[nLine].substr(0, lines[nLine].find(splitChar));
                    
                    for(int i = 0; i < nCores; i++){
                        if(token == tokenToSearch[i]){ 
                            //se o cabeçalho for o procurado pega o stream da linha toda 
                            for(int j = 0; j < 10  && rightLine.good(); j++){
                            //11 itens onde o primeiro é o cabeçalho
                                rightLine >> item[j];
                                if(j == 0) cout << item[j] << endl;
                                //soma os tempos totais
                                if(j != 0){
                                    total[i] += stoi(item[j]);
                                }
                                //soma o tempo que o core ficou de boas
                                if(j == 4){
                                    idle[i] = stoi(item[j]);
                                }
                            }
                        }
                    }      
                }
                
                cout << endl << "Gráfico de uso de CPU" << endl;
                for(int i = 0; i < nCores; i++){
                    percent = 100 - 100*(idle[i])/total[i];
                    if(percent < 20){
                        cout << GREEN;
                    } else if (percent < 70) {
                        cout << YELLOW;
                    } else {
                        cout << RED;
                    }
                    
                    cout << "CPU" << i << " " <<  percent << "% [";
                    //cout << (int) percent/10;
                    for(int j = 0; j < (int) percent/10; j++){
                        cout << "|";
                    }
                    for(int j = 0; j < 10 - (int) percent/10; j++){
                        cout << " ";
                    }
                    cout << "]" << endl << RESET;
                }   
                cout << endl;                             
                system("read -p \"Pressione enter para voltar\" saindo");
                infile.clear();
                
                break;
                
            }
            default:
                cout<<"Escolha invalida\n";
                error!=error;
                break;
        }
    }while(1);
}
        
