#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int is_prime(int n){
  if (n <= 1) return 0;
  for(int i = 2; i < n;i++){
    if(!(n % i)){
      return 0;
    }
  }
  return 1;
}

void generator(int cd){
  int i = 2;
  while(1){
    if(channel_put(cd, i++) < 0){
      exit(0);
    }
  }
}

void checker(int cd_in, int cd_out){
  int data;
  while(1){
    if(channel_take(cd_in, &data) < 0){
      exit(0);
    }
    if(is_prime(data)){
      if(channel_put(cd_out, data) < 0){
        exit(0);
      }
    }
  }
}

void printer(int cd){
  int n = 0;
  int data;
  while(n < 100){
    channel_take(cd, &data);
    n++;
    printf("%d\n", data);
  }
}

int
main(int argc, char *argv[])
{
  int checkers_number = 3;
  if (argc > 1) {
    checkers_number = atoi(argv[1]);
  }

  int chan1 = channel_create();
  int chan2 = channel_create();

  if (chan1 < 0 || chan2 < 0) {
    printf("Failed to create channels\n");
    exit(1);
  }

  if(fork() == 0){
    generator(chan1);
    exit(0);
  }

  for(int i = 0; i < checkers_number; i++){
    if(fork() == 0){
      checker(chan1, chan2);
      exit(0);
    }
  }
    
  printer(chan2);
  channel_destroy(chan2);
  channel_destroy(chan1);
  
  char buf[10];
  printf("Restart? (y/n): ");
  gets(buf, sizeof(buf));
  if (buf[0] == 'y' || buf[0] == 'Y') {
    main(argc, argv);
  }
  
  exit(0);
}