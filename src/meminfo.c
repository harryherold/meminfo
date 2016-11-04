#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_PATH 2048
#define MAX_LINE 512

void print_meminfo(int childpid)
{
    char* line = NULL;
    size_t len;
    char buf[MAX_PATH];

    snprintf(buf, MAX_PATH, "/proc/%d/status", childpid);

    FILE* fin = fopen(buf, "r");
    if (fin == NULL)
    {
        fprintf(stderr, "[ERROR]: %s does not exist", buf);
        return;
    }

    line = calloc(MAX_LINE, sizeof(char));
    assert(line);

    char key[6];
    char unit[2];
    unsigned long long value;
    printf("\nMemory Statistics:\n");
    while (getline(&line, &len, fin) != -1)
    {
        if (strncmp(line, "VmHWM", 5) == 0)
        {
            sscanf(line, "%5s: %llu %2s", key, &value, unit);
            printf("Physical Memory: %llu %2s\n", value, unit);
        }
        if (strncmp(line, "VmPeak", 6) == 0)
        {
            sscanf(line, "%6s: %llu %2s", key, &value, unit);
            printf("Virtual Memory : %llu %2s\n", value, unit);
        }
        if (strncmp(line, "VmStk", 5) == 0)
        {
            sscanf(line, "%5s: %llu %2s", key, &value, unit);
            printf("Stack Segment  : %llu %2s\n", value, unit);
        }
        if (strncmp(line, "VmData", 6) == 0)
        {
            sscanf(line, "%6s: %llu %2s", key, &value, unit);
            printf("Data Segment   : %llu %2s\n", value, unit);
        }
        if (strncmp(line, "VmExe", 5) == 0)
        {
            sscanf(line, "%5s: %llu %2s", key, &value, unit);
            printf("Text Segment   : %llu %2s\n", value, unit);
        }
    }

    free(line);
    fclose(fin);
}

void print_io_info(int childpid)
{
    char* line = NULL;
    size_t len;
    char buf[MAX_PATH];

    snprintf(buf, MAX_PATH, "/proc/%d/io", childpid);

    FILE* fin = fopen(buf, "r");
    if (fin == NULL)
    {
        fprintf(stderr, "[ERROR]: %s does not exist", buf);
        return;
    }

    line = calloc(MAX_LINE, sizeof(char));
    assert(line);

    char key[6];
    unsigned long long value;
    printf("\nFile I/O Statistics:\n");
    while (getline(&line, &len, fin) != -1)
    {
        if (strncmp(line, "read_bytes", 10) == 0)
        {
            sscanf(line, "%10s: %llu", key, &value);
            printf("Bytes read:    %llu\n", value);
        }
        if (strncmp(line, "write_bytes", 11) == 0)
        {
            sscanf(line, "%11s: %llu", key, &value);
            printf("Bytes written: %llu\n", value);
        }
    }

    free(line);
}

int main(int argc, char* argv[])
{
    long data = PTRACE_O_TRACEEXIT;
    pid_t child;
    child = fork();
    if (child == 0)
    {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execvp(argv[1], &argv[1]);
    }
    else
    {
        int status;
        waitpid(child, &status, 0);

        if (waitpid(child, &status, WNOHANG))
        {
            fprintf(stderr, "Error: unable to run %s\n", argv[1]);
            exit(EXIT_FAILURE);
        }

        int ret = ptrace(PTRACE_SETOPTIONS,
                         child, NULL,
                         (void*)data);
        if (ret == -1)
        {
            perror("Can not trace process");
            return EXIT_FAILURE;
        }

        while (1)
        {
            ptrace(PTRACE_CONT, child, NULL, NULL);
            waitpid(child, &status, 0);
            if (status >> 8 == (SIGTRAP | (PTRACE_EVENT_EXIT << 8)))
            {
                print_meminfo(child);
                print_io_info(child);
                break;
            }
        }
    }
    return EXIT_SUCCESS;
}
