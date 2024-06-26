#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int cd = channel_create();
  if (cd < 0) {
    printf("Failed to create channel\n");
    exit(1);
  }
  if (fork() == 0) {
    if (channel_put(cd, 42) < 0) {
      printf("Failed to put data in channel\n");
      exit(1);
    }
    channel_put(cd, 43); // Sleeps until cleared
    // Handle error
    channel_destroy(cd);
    // Handle error
  } else {
    int data;
    if (channel_take(cd, &data) < 0) { // 42
      printf("Failed to take data from channel\n");
      exit(1);
    }
    printf("Received: %d\n", data);
    
    // Handle error
    
    if (channel_take(cd, &data) < 0) {// Sleep until child destroys channel
      printf("Failed to take data from channel\n");
      exit(1);
    }
    // Handle error
    printf("Received: %d\n", data);
  }
  exit(0);
}