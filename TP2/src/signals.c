#include "signals.h"

pid_t foreground_pid = -1; // Almacena el PID del proceso en primer plano

void sigint_handler(int sig)
{
    if (foreground_pid > 0)
    {
        kill(foreground_pid, SIGINT);
    }
}

void sigtstp_handler(int sig)
{
    if (foreground_pid > 0)
    {
        kill(foreground_pid, SIGTSTP);
    }
}

void sigquit_handler(int sig)
{
    if (foreground_pid > 0)
    {
        kill(foreground_pid, SIGQUIT);
    }
}

void setup_signal_handlers()
{
    signal(SIGINT, sigint_handler);
    signal(SIGTSTP, sigtstp_handler);
    signal(SIGQUIT, sigquit_handler);
}
