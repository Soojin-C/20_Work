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

  while(1){

    int well_known_pipe = mkfifo("main", 0644);
    int receive_msg = open("main", O_RDONLY);

    char message[HANDSHAKE_BUFFER_SIZE];
    int reading_pipe = read(receive_msg, message, HANDSHAKE_BUFFER_SIZE);
    printf("Initial message: %s\n", message);


    int f = fork();
    if (f){

      remove ( "main" );
      close ( receive_msg );

    }
    else{

      int to_client;
      int from_client;

      from_client = receive_msg;
      server_handshake2(&to_client, &from_client, message);

      char input[BUFFER_SIZE] = "message recieved";
      char output[BUFFER_SIZE];

      int reading = read(from_client, output, BUFFER_SIZE);
      while(reading){
        //printf("Msg : %s\n", output);
        int counter = 0;
        while (counter < strlen(output)){

          char curr = output[counter];
          if(!strcmp(&curr, "\n")){
            output[counter] = 0;
            //printf("%s\n", output);
          }

          output[counter] = curr + 3;
          counter ++;

        }
        printf("output: %s\n", output);

        int writing = write(to_client, output, BUFFER_SIZE);

        reading = read(from_client, output, BUFFER_SIZE);
      }

      close(to_client);
      close(from_client);

      return 0;

    }
  }
}
