#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/neutrino.h>

int main() {
    int chid, rcvid;
    char buffer[256];

    chid = ChannelCreate(0);
    if (chid == -1) {
        perror("ChannelCreate()");
        exit(EXIT_FAILURE);
    }
    
    printf("Hello! Im %i, and I've created channel %i\n", getpid(), chid);

    while (1) {
        rcvid = MsgReceive(chid, buffer, sizeof(buffer), NULL);
        printf("Recived message from (rcvid %X): \"%s\"\n", rcvid, buffer);
        
        strcpy(buffer, "Reply");
        MsgReply(rcvid, EOK, buffer, sizeof(buffer));
    }

    return (EXIT_SUCCESS);
}
