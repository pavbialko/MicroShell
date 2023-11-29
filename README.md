# MicroShell

## Author
Pavel Bielko

## Description
MicroShell is a simple Unix-like shell implemented in C. It supports basic shell commands such as `exit`, `help`, `cd`, `pwd`, and `echo`. Additionally, it can execute external commands using the `execvp` system call.

## Features
- **Built-in Commands:**
  - `exit`: Terminate the shell.
  - `help`: Display information about the MicroShell project or specific commands.
  - `cd`: Change the working directory.
  - `pwd`: Print the name of the current directory.
  - `echo`: Write arguments to the standard output.

- **Command History:**
  - The shell keeps track of the current and previous working directories, providing a simple command history.

- **Customizable Prompt:**
  - The shell prompt displays the username, current path, and supports custom colors.

## Usage
1. Open a new terminal window or cmd if you are on windows.
2. Change the directory to the directory in which you have your source.cpp file. For example, if it is in User/Documents/Downloads, enter your command line:
   ```bash
   cd ~/Documents/Downloads
4. Compile the MicroShell program using a gcc compiler. For example:
   ```bash
   g++ -o <name-you-want-to-give> microshell.c
5. Run it! Now you can run the program using:
   ```bash
   ./<name-you-want-to-give>

## Examples
```bash
./microshell
```
***MicroShell***
**Author: Pavel Bielko**
*Implemented commands: exit, help, cd, pwd, echo*

```bash
 help
```
Display information about the MicroShell project or specific commands.

```bash
cd /path/to/directory
```
Change the working directory.

```bash
echo Hello, MicroShell!
```
Hello, MicroShell!

```bash
exit
```
Ending shell
