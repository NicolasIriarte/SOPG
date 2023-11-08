# Introduction
This project solves the problem described on [this file](./SOPG_TP1.pdf).

# How to run

## Compile
```bash
make all # NOTE the "all" to compile reader and writer
```
## Run
Run both binaries (Reader and Writer) on different terminals.

From the writter one you can write any text and send it to the reader one with
the return key.

Also you can send SIGUSR1 and SIGUSR2 to the writer process to send a custom
message to the reader.

```bash
kill -SIGUSR1 <writer_pid> # Send SIGUSR1 message to the writer

kill -SIGUSR2 <writer_pid> # Send SIGUSR2 message to the writer
```

## Notes
This program will generate the following files were is executed:
  - `fifo_fd`: FIFO file used for IPC.
  - `signals.txt`: When a signal is sended to the writer, the reader will write
    this file.
  - `log.txt`: When a text is written to the writer, the reader will write
    this file.
