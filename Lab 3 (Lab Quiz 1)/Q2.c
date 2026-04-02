#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    // Check for the correct number of command-line arguments
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <input_file> <pattern1> <pattern2>\n", argv[0]);
        return 1;
    }

    char *file = argv[1];
    char *pattern1 = argv[2];
    char *pattern2 = argv[3];

    int pipe1_fd[2]; // Pipe for the first grep to the second grep
    int pipe2_fd[2]; // Pipe for the second grep to wc

    // Create the two pipes
    if (pipe(pipe1_fd) == -1) {
        perror("pipe1 failed");
        return 1;
    }
    if (pipe(pipe2_fd) == -1) {
        perror("pipe2 failed");
        return 1;
    }

    pid_t pid1, pid2, pid3;
    int status1, status2, status3;
    int max_exit_status = 0;

    // Fork for the first child (grep pattern1 file)
    if ((pid1 = fork()) == 0) {
        // Child 1
        close(pipe1_fd[0]); // Close read end of pipe1
        close(pipe2_fd[0]); // Close read end of pipe2
        close(pipe2_fd[1]); // Close write end of pipe2

        // Redirect stdout to the write end of pipe1
        dup2(pipe1_fd[1], STDOUT_FILENO);
        close(pipe1_fd[1]); // Close the original file descriptor

        execlp("/usr/bin/grep", "grep", pattern1, file, NULL);
        perror("execlp grep1 failed");
        exit(errno);
    }
    if (pid1 == -1) {
        perror("fork failed");
        return 1;
    }

    // Fork for the second child (grep pattern2)
    if ((pid2 = fork()) == 0) {
        // Child 2
        close(pipe1_fd[1]); // Close write end of pipe1
        close(pipe2_fd[0]); // Close read end of pipe2

        // Redirect stdin to the read end of pipe1
        dup2(pipe1_fd[0], STDIN_FILENO);
        close(pipe1_fd[0]); // Close the original file descriptor

        // Redirect stdout to the write end of pipe2
        dup2(pipe2_fd[1], STDOUT_FILENO);
        close(pipe2_fd[1]); // Close the original file descriptor

        execlp("/usr/bin/grep", "grep", pattern2, NULL);
        perror("execlp grep2 failed");
        exit(errno);
    }
    if (pid2 == -1) {
        perror("fork failed");
        return 1;
    }

    // Fork for the third child (wc -l)
    if ((pid3 = fork()) == 0) {
        // Child 3
        close(pipe1_fd[0]); // Close read end of pipe1
        close(pipe1_fd[1]); // Close write end of pipe1
        close(pipe2_fd[1]); // Close write end of pipe2

        // Redirect stdin to the read end of pipe2
        dup2(pipe2_fd[0], STDIN_FILENO);
        close(pipe2_fd[0]); // Close the original file descriptor

        execlp("/usr/bin/wc", "wc", "-l", NULL);
        perror("execlp wc failed");
        exit(errno);
    }
    if (pid3 == -1) {
        perror("fork failed");
        return 1;
    }

    // Parent process closes all pipe ends
    close(pipe1_fd[0]);
    close(pipe1_fd[1]);
    close(pipe2_fd[0]);
    close(pipe2_fd[1]);

    // Wait for all children and get their exit statuses
    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);
    waitpid(pid3, &status3, 0);

    // Calculate the maximum exit status
    if (WIFEXITED(status1)) {
        max_exit_status = WEXITSTATUS(status1);
    }
    if (WIFEXITED(status2) && WEXITSTATUS(status2) > max_exit_status) {
        max_exit_status = WEXITSTATUS(status2);
    }
    if (WIFEXITED(status3) && WEXITSTATUS(status3) > max_exit_status) {
        max_exit_status = WEXITSTATUS(status3);
    }

    // Handle negative exit status for processes terminated by a signal
    if (WIFSIGNALED(status1) || WIFSIGNALED(status2) || WIFSIGNALED(status3)) {
        return -1;
    }

    return max_exit_status;
}
