/**
 * @file   Writer.c
 * @brief  This file aims to solve the practical work 1.
 *
 * @author Iriarte Nicolas <NicolasIriarte95@gmail.com>
 * @date   2023-05-21
 */

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_NAME "fifo_fd"

#define DATA_TOKEN "DATA:"
#define SIGN1_TOKEN "SIGN:1"
#define SIGN2_TOKEN "SIGN:2"

#define BUFFER_SIZE 300

static int32_t fd;

/* Write into the named fifo
 * This function expects a null terminated string.
 */
static int HandleFifoWrite(const char *message) {
  return write(fd, message, strlen(message));
}

/* Handle SIGUSR1 to write into the named fifo */
static void sigusr1_handler(int signum) {
  printf("Writting %s\n", SIGN1_TOKEN);
  HandleFifoWrite(SIGN1_TOKEN);
}

/* Handle SIGUSR2 to write into the named fifo */
static void sigusr2_handler(int signum) {
  printf("Writting %s\n", SIGN2_TOKEN);
  HandleFifoWrite(SIGN2_TOKEN);
}

static void InitializeNamedFifo(void) {
  int32_t returnCode;

  /* Create named fifo. No action if already exists */
  if ((returnCode = mknod(FIFO_NAME, S_IFIFO | 0666, 0)) != -1) {
    printf("Error creating named fifo: %d\n", returnCode);
    exit(1);
  }

  /* Open named fifo. Blocks until other process opens it */
  printf("waiting for readers...\n");
  if ((fd = open(FIFO_NAME, O_WRONLY)) < 0) {
    printf("Error opening named fifo file: %d\n", fd);
    exit(1);
  }
}

int main(void) {

  char outputBuffer[BUFFER_SIZE] = DATA_TOKEN;
  uint32_t bytesWrote;

  InitializeNamedFifo();

  /* Handle SIGUSR1 signal */
  signal(SIGUSR1, sigusr1_handler);
  signal(SIGUSR2, sigusr2_handler);

  /* open syscalls returned without error -> other process attached to named
   * fifo */
  printf("got a reader--type some stuff\n");

  /* Loop forever */
  while (1) {
    /* Get some text from console */
    fgets(outputBuffer + strlen(DATA_TOKEN), BUFFER_SIZE - strlen(DATA_TOKEN),
          stdin);

    /* Prepend the string "DATA:" to the output buffer */

    /* Write buffer to named fifo. Strlen - 1 to avoid sending \n char */
    if ((bytesWrote = HandleFifoWrite(outputBuffer)) == -1) {
      perror("write");
    } else {
      printf("writer: wrote %d bytes\n", bytesWrote);
    }
  }
  return 0;
}

// clang-format off
// Some emacs magic for auto-compilation of isolated files.
/*
  Local Variables:
  quickrun-option-cmd-alist: ((:command . "gcc")
                              (:args    . "apple melon")
                              (:exec    . ("%c -Wall -Werror -o writer %s"
                                           "echo Compiled writer"))
                              )
  End:
*/
