#include "pipe_networking.h"


int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

  while (1){
    printf("Input Data: ");
    char input[BUFFER_SIZE];
    char output[BUFFER_SIZE];
    fgets(input, BUFFER_SIZE, stdin);

    int writing = write(to_server, input, BUFFER_SIZE);
    int reading = read(from_server, output, BUFFER_SIZE);

    printf("server msg:[%s]\n", output);

  }
}
