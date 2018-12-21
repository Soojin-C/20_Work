#include "pipe_networking.h"

static void sighandler(int signo) {
    if (signo == SIGINT) {
        remove("main");
        printf("Removed main\n");
        exit(0);
    }
}

int main() {
  signal(SIGINT, sighandler);

  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );

  while(1){
    char input[BUFFER_SIZE] = "message recieved";
    char output[BUFFER_SIZE];

    int reading = read(from_client, output, BUFFER_SIZE);
    //printf("%c", output[0]);
/*
    int counter = 0;
    while (counter <= BUFFER_SIZE){
      output[counter] = output[counter] + 3;
      printf("temp: [%c]", output[counter]);
      counter++;
    }
*/
    //printf("msg : %s", output);

    int writing = write(to_client, output, BUFFER_SIZE);

    close(to_client);
    close(from_client);
  }
}
