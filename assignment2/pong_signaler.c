#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct msg_buffer {
	long msg_type;
	char msg_text[100];
} message_t;

int main()
{
	key_t key = ftok("progfile", 65);

	int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        perror("Unable to get message queue\n");
        return 1;
    }
	
    message_t PING;
    PING.msg_type = 1;
    PING.msg_type = "PONG";
    
    
	if (msgsnd(msgid, &PONG, sizeof(PING), 0) == -1)
    {
        perror("Unable to send a message\n");
        return 1;
    }

    message_t PING;
	int bytes_count = msgrcv(msgid, &PING, sizeof(message_t), 1, 0);
    if (bytes_count == -1)
    {
        perror("Unable to receive a message.\n");
        return 1;
    }

    while(bytes_count != -1)
    {
        msgsnd(msgid, &PONG, sizeof(PING), 0);
        
	    printf("%s \n", PONG.msg_text);
    }
    
	return 0;
}
