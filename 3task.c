#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/neutrino.h>

typedef struct {
    char text[256];
} message_t;

void server(void) {
    int chid;
    int rcvid;
    message_t msg;
    message_t reply;

    // Create a channel
    chid = ChannelCreate(0);
    if (chid == -1) {
        perror("ChannelCreate");
        exit(EXIT_FAILURE);
    }

    printf("Server started, channel ID: %d\n", chid);

    for (;;) {
        rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL);
        if (rcvid == -1) {
            perror("MsgReceive");
            continue;
        }

        printf("Received message: %s\n", msg.text);

        strncpy(reply.text, msg.text, sizeof(reply.text) - 1);
        reply.text[sizeof(reply.text) - 1] = '\0';

        if (MsgReply(rcvid, EOK, &reply, sizeof(reply)) == -1) {
            perror("MsgReply");
        } else {
            printf("Replied with message: %s\n", reply.text);
        }
    }
}

int main(void) {
    server();
    return 0;
}
