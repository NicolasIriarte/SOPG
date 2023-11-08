/**
 * @file   Reader.c
 * @brief  This file aims to solve the practical work 1.
 *
 * @author Iriarte Nicolas <NicolasIriarte95@gmail.com>
 * @date   2023-05-21
 */

#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_NAME "fifo_fd"
#define SIGNAL_NAME "signals.txt"
#define LOG_NAME "log.txt"

#define DATA_TOKEN "DATA:"
#define SIGN1_TOKEN "SIGN:1"
#define SIGN2_TOKEN "SIGN:2"

#define BUFFER_SIZE 300

int main(void) {
  char inputBuffer[BUFFER_SIZE];
  uint32_t bytesRead;
  int32_t returnCode, fd;

  /* Create named fifo. No action if already exists */
  if ((returnCode = mknod(FIFO_NAME, S_IFIFO | 0666, 0)) != -1) {
    printf("Error creating named fifo: %d\n", returnCode);
    exit(1);
  }

  /* Open named fifo. Blocks until other process opens it */
  printf("waiting for writers...\n");
  if ((fd = open(FIFO_NAME, O_RDONLY)) < 0) {
    printf("Error opening named fifo file: %d\n", fd);
    exit(1);
  }

  /* Open the file signals.txt and log.txt */
  FILE *signals = fopen(SIGNAL_NAME, "w+");
  FILE *log = fopen(LOG_NAME, "w+");

  /* Check if files where opened correctly */
  if (signals == NULL) {
    perror("Error opening signals file\n");
    exit(1);
  }

  if (log == NULL) {
    perror("Error opening log file\n");
    exit(1);
  }

  /* open syscalls returned without error -> other process attached to named
   * fifo */
  printf("Got a writer\n");

  /* Loop until read syscall returns a value <= 0 */
  do {
    /* read data into local buffer */
    if ((bytesRead = read(fd, inputBuffer, BUFFER_SIZE)) == -1) {
      perror("Error reading file descriptor\n");
      break;
    }

    inputBuffer[bytesRead] = '\0';
    printf("reader: read %d bytes: \"%s\"\n", bytesRead, inputBuffer);

    /* Check if the readed data starts with "DATA" or "SIGN" */
    if (strncmp(inputBuffer, DATA_TOKEN, strlen(DATA_TOKEN)) == 0) {
      /* Write the data to the log file */
      printf("Appending data to log file\n");
      fprintf(log, "%s\n", inputBuffer + strlen(DATA_TOKEN));
      fflush(log); // Force the write to the file
    } else if (strncmp(inputBuffer, SIGN1_TOKEN, strlen(SIGN1_TOKEN) - 1) == 0) {
      /* Write the data to the log file */
      printf("Appending data to signal file\n");
      fprintf(signals, "%s\n", inputBuffer);
      fflush(signals); // Force the write to the file
    } else if (strncmp(inputBuffer, SIGN2_TOKEN, strlen(SIGN2_TOKEN)) == 0) {
      /* Write the data to the log file */
      printf("Appending data to signal file\n");
      fprintf(signals, "%s\n", inputBuffer);
      fflush(signals); // Force the write to the file
    } else {
      /* Write the data to the log file */
      perror("Invalid message\n");
    }

  } while (bytesRead > 0);

  /* Close files */
  fclose(signals);
  fclose(log);

  /* Close named fifo */
  close(fd);

  return 0;
}

// clang-format off
// Some emacs magic for auto-compilation of isolated files.
/*
  Local Variables:
  quickrun-option-cmd-alist: ((:command . "gcc")
                              (:args    . "apple melon")
                              (:exec    . ("%c -o reader %s"
                                           "echo Compiled reader"))
                              )
  End:
*/
