# unix-shell-c

A minimal Unix-like shell implemented in C to understand process management, system calls, and inter-process communication in Linux.
This project was built as a systems programming exercise to explore how real shells work internally.

Overview:
  This shell supports execution of external programs, built-in commands, pipes, I/O redirection, background processes, and signal handling.
  The goal of the project was to gain a deeper understanding of Unix process control and operating system concepts.

Features:
  Execute external commands (ls, cat, grep, etc.)
  Built-in cd command
  Single pipe support (|)
  Input redirection (<)
  Output redirection (>)
  Background execution (&)
  Signal handling (Ctrl+C)
  Zombie process cleanup using SIGCHLD
  Proper use of fork–exec model

Example Usage:
  myshell> ls
  myshell> cd ..
  myshell> ls | wc
  myshell> echo hello > file.txt
  myshell> cat < file.txt
  myshell> sleep 5 &

How It Works:
  The shell repeatedly reads user input, parses it, and determines whether the command is built-in or external.
  For external commands:
  The shell creates a child process using fork()
  The child process sets up pipes or redirection if needed
  The child calls execvp() to run the program
  The parent either waits or continues depending on background execution
  Pipes are implemented using pipe() and dup2() to connect stdout of one process to stdin of another.
  Redirection is handled by opening files and replacing file descriptors before calling exec().

Key Concepts Used:
  fork()
  execvp()
  wait() / waitpid()
  pipe()
  dup2()
  file descriptors
  signal handling (SIGINT, SIGCHLD)
  process creation and termination
  background process handling

Compilation:(use Linux)
  gcc shell.c -o myshell
  ./myshell

What I Learned:
  This project helped me understand how Unix shells manage processes and handle command execution.
  I gained hands-on experience with process creation, pipes, file descriptor manipulation, and signal handling.
  Building this shell improved my understanding of operating system concepts and low-level programming in C.

Limitations:
  Supports only single pipe
  Basic command parsing
  No advanced job control
  No command history

Future Improvements:
  Multiple pipes
  Command history
  Auto-completion
  Job control
  Better parsing

Author:
  Built as part of systems programming learning and preparation for research internships.
