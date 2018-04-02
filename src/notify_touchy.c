/*
FILENAME :     notify_touchy.c
DESCRIPTION :  Service which touches a file to vrffile if the return code of program is status_ok.
AUTHOR :       Bandie
DATE :         2018-03-17T20:31:48+01:00
LICENSE :      GNU-GPLv3
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <libnotify/notify.h>

#define PROGARGVSIZE 256

char *prog = NULL;

void help(){
  printf("notify_touchy ( arguments )\n\n"
         "It checks if a program has a specific return code within some time. If it has not a notification will appear.\n\n"
         "Arguments:\n\n"
         "-h, --help                         Shows this here.\n"
         "-p, --program [program or script]  Executes and checks this program for the return code. (Required)\n"
         "-r, --return [return code]         Listen to this return code. (Default: 0)\n"
         "-s, --sleep [seconds]              Interval of when the next check should start. (Default: 20)\n"
         "-v, --verbose                      Show some unnecessary stuff.\n\n");
}

void exec_failed(){
  fprintf(stderr, "ERROR: It is not possible to execute %s: ", prog);
  if(access(prog, F_OK)) fprintf(stderr, "It does not exist (in your PATH variable).\n");
  else fprintf(stderr, "Access denied. (Are you root?)\n");
  exit(EXIT_FAILURE);
}

void sig_handler(int signo){
  if(signo == SIGTERM || signo == SIGINT){
    exit(0);
  }
}

int main(int argc, char *argv[]){
  int returncode = 0;
  int sleeptm = 20;
  int verbose = 0;
  int statval;
  char *progargv[PROGARGVSIZE];
 


  // Argument handling
  for(int i=0; i<argc; i++){
    if(!strncmp(argv[i], "--help", 256) || !strncmp(argv[i], "-h", 256)){
      help();
      return 0;
    }
    if(!strncmp(argv[i], "--verbose", 256) || !strncmp(argv[i], "-v", 256))
      verbose = 1;
    if((!strncmp(argv[i], "--program", 256) || !strncmp(argv[i], "-p", 256)) && (i+1) < argc){
      
      char *prgargptr;
      prgargptr = strtok(argv[i+1], " ");
      prog = prgargptr;
      progargv[0] = prgargptr;
      int l=1;
      while(prgargptr != NULL){
        prgargptr = strtok(NULL, " ");
        progargv[l] = prgargptr;
        l++;
      }
      
    }
    if((!strncmp(argv[i], "--return", 256) || !strncmp(argv[i], "-r", 256)) && (i+1) < argc){
      int j = 0;
      while(argv[i+1][j]){
        if(j==0 && argv[i+1][j] == '-') j++;
        if(!isdigit(argv[i+1][j])){
          fprintf(stderr, "ERROR: Invalid argument. The return code must be a digit.\n");
          return 1;
        }
        j++;
      }
      sscanf(argv[i+1], "%d", &returncode);
    }
    if((!strncmp(argv[i], "--sleep", 256) || !strncmp(argv[i], "-s", 256)) && (i+1) < argc){
      int j=0;
      while(argv[i+1][j]){
        if(!isdigit(argv[i+1][j]) || argv[i+1][0] == '0'){
          fprintf(stderr, "ERROR: Invalid argument. The sleep value must be a positive digit.\n");
          if(argv[i+1][0] == '0')
            fprintf(stderr, "No, you REALLY don't want to put a 0 in here.\n");
          return 1;
        }
        j++;
      }
      sscanf(argv[i+1], "%d", &sleeptm);
    }

  }

  if(!prog){
    fprintf(stderr, "ERROR: Missing argument.\nExcuse me but where did you specifiy which program you use? (-p)\n");
    exit(1);
  }

  // Signal handling
  if(signal(SIGTERM, sig_handler) == SIG_ERR || signal(SIGINT, sig_handler) == SIG_ERR)
    fprintf(stderr, "ERROR: Can't catch signal!");
  
  while(1){  

    if(fork() == 0){

      // Exec program silently by default
      if(!verbose){
        int fd = open("/dev/null", O_WRONLY | O_CREAT, 0666);    

        dup2(fd, 1);
        dup2(fd, 2);
        if(execvp(prog, progargv) < 0)
          exec_failed();
        
        close(fd);
      }
      else{
        if(execvp(prog, progargv) < 0)
          exec_failed();
      }


    } 

    else {

      if(verbose) 
        printf("Waiting for %d...\n", getpid());

      wait(&statval);
      if(WIFEXITED(statval)){
        if(verbose) 
          printf("Exit: %d\n", WEXITSTATUS(statval));

        if(WEXITSTATUS(statval) != returncode){
          char s[100];
          sprintf(s, "Warning: %s returned something bad.", prog);
          printf("%s\n", s);
          notify_init(prog);
          NotifyNotification *Not = notify_notification_new("Notify_touchy", s, "dialog-warning");
          notify_notification_show(Not, NULL);
          g_object_unref(G_OBJECT(Not));
          notify_uninit();
        }

      } 
    }

    sleep(sleeptm);
  }


  sig_handler(SIGTERM);
  return 0;
}
