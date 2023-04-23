#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/neutrino.h>

//структура сообщения для сервера
typedef struct {
    char text[256];
} message_t;

void server(void) {
    int chid;
    int rcvid;
    message_t msg;
    message_t reply;

    //создание канала
    chid = ChannelCreate(0);
    if (chid == -1) {
        perror("ChannelCreate");
        exit(EXIT_FAILURE);
    }

    printf("Server started, channel ID: %d\n", chid);

    //ожидание сообщений и ответ на них в бесконечном цикле
    for (;;) {
        //получение сообщения
        rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL);
        if (rcvid == -1) {
            perror("MsgReceive");
            continue;
        }

        printf("Received message: %s\n", msg.text);

        //обработка сообщения и создание ответа        
        strncpy(reply.text, msg.text, sizeof(reply.text) - 1);
        reply.text[sizeof(reply.text) - 1] = '\0';

        //Ответ на сообщение
        if (MsgReply(rcvid, EOK, &reply, sizeof(reply)) == -1) {
            perror("MsgReply");
        } else {
            printf("Replied with message: %s\n", reply.text);
        }
    }
}
//запуск
int main(void) {
    server();
    return 0;
}
