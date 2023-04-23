#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/neutrino.h>

int main() {
    char msg[256], rpl[256];
    int pid, chid, coid;

    printf("Enter process ID's\n-> ");
    scanf("%i %i", &pid, &chid);

    coid = ConnectAttach(0, pid, chid, 0, 0);
    if (coid == -1) {
        perror("ConnectAttach");
        exit(EXIT_FAILURE);
    }

    strcpy(msg, "All checked. In progress");
    if (MsgSend(coid, msg, strlen(msg) + 1, rpl, sizeof(rpl)) == -1) {
        perror("MsgSend");
        exit(EXIT_FAILURE);
    }

    printf("Process %i returned \"%s\"\n", pid, rpl);

    ConnectDetach(coid);

    return (EXIT_SUCCESS);
}
