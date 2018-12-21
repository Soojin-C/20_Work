#include "pipe_networking.h"
/*=========================
  server_handshake
  args: int * to_client
  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
    // create pipe
    int well_known_pipe = mkfifo("main", 0644);
    int receive_msg = open("main", O_RDONLY);

    // receive message
    char message[HANDSHAKE_BUFFER_SIZE];
    int reading_pipe = read(receive_msg, message, HANDSHAKE_BUFFER_SIZE);
    printf("message: %s\n", message);

    // remove pipe
    remove("main");

    // connects to private fifo
    *to_client = open(message, O_WRONLY);

    // send initial message
    int writing = write(*to_client, ACK, HANDSHAKE_BUFFER_SIZE);

    // get response from client
    reading_pipe = read(receive_msg, message, HANDSHAKE_BUFFER_SIZE);
    printf("message: %s\n", message);
    // return fd to wkp
    return receive_msg;
}


/*=========================
  client_handshake
  args: int * to_server
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.
  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
    // create pipes
    char pipe_name[HANDSHAKE_BUFFER_SIZE];
    sprintf(pipe_name, "%d", getpid());
    int private = mkfifo(pipe_name, 0644);


    // set to_server to wkp
    *to_server = open("main", O_WRONLY);

    // sends private fifo name to server
    int writing= write(*to_server, pipe_name, HANDSHAKE_BUFFER_SIZE);

    // wait for response
    int receive_msg = open(pipe_name, O_RDONLY);

    // receive server message
    char message[HANDSHAKE_BUFFER_SIZE];
    int reading = read(receive_msg, message, HANDSHAKE_BUFFER_SIZE);
    printf("message %s\n", message);

    // remove private fifo
    remove(pipe_name);

    writing= write(*to_server, ACK, HANDSHAKE_BUFFER_SIZE);
    return receive_msg;
}
