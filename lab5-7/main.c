#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <zmq.h>
#include <assert.h>
#include <pthread.h>

#include "balancedBinaryTree.h"
#include "timer.h"

/*
Создание нового вычислительного узла
Формат команды: create id [parent]
id – целочисленный идентификатор нового вычислительного узла
parent – целочисленный идентификатор родительского узла. Если топологией не предусмотрено
введение данного параметра, то его необходимо игнорировать (если его ввели)
Формат вывода:
«Ok: pid», где pid – идентификатор процесса для созданного вычислительного узла
«Error: Already exists» - вычислительный узел с таким идентификатором уже существует
«Error: Parent not found» - нет такого родительского узла с таким идентификатором
«Error: Parent is unavailable» - родительский узел существует, но по каким-то причинам с ним не
удается связаться
«Error: [Custom error]» - любая другая обрабатываемая ошибка
Пример:
> create 10 5
Ok: 3128
Примечания: создание нового управляющего узла осуществляется пользователем программы
при помощи запуска исполняемого файла. Id и pid — это разные идентификаторы.

Исполнение команды на вычислительном узле
Формат команды: exec id [params]
id – целочисленный идентификатор вычислительного узла, на который отправляется команда
Формат вывода:
«Ok:id: [result]», где result – результат выполненной команды
«Error:id: Not found» - вычислительный узел с таким идентификатором не найден
«Error:id: Node is unavailable» - по каким-то причинам не удается связаться с вычислительным
узлом
«Error:id: [Custom error]» - любая другая обрабатываемая ошибка
Пример:
Можно найти в описании конкретной команды, определенной вариантом задания.
Примечание: выполнение команд должно быть асинхронным. Т.е. пока выполняется команда на
одном из вычислительных узлов, то можно отправить следующую команду на другой
вычислительный узел.
Набора команд 3 (локальный таймер)
Формат команды сохранения значения: exec id subcmd
subcmd – одна из трех команд: start, stop, time.
start – запустить таймер
stop – остановить таймер
time – показать время локального таймера в миллисекундах
Пример:
> exec 10 time
Ok:10: 0
>exec 10 start
Ok:10
>exec 10 start
Ok:10
*прошло 10 секунд*
> exec 10 time
Ok:10: 10000
*прошло 2 секунды*
>exec 10 stop
Ok:10
*прошло 2 секунды*
>exec 10 time
Ok:10: 12000

Команда проверки 2
Формат команды: ping id
Команда проверяет доступность конкретного узла. Если узла нет, то необходимо выводить 
ошибку: «Error: Not found» 
Пример:
> ping 10
Ok: 1 // узел 10 доступен
> ping 17
Ok: 0 // узел 17 недоступен
*/

char** split(char *str, char symbol) {
    size_t cnt = 0;
    char *tmp = str;
    char *last = NULL;
    char **result = NULL;
    char delimiter[2];
    delimiter[0] = symbol;
    delimiter[1] = 0; 
    
    while (*tmp) {
        if (symbol == *tmp) {
            ++cnt;
            last = tmp;
        }
        ++tmp;
    }

    cnt += last < (str + strlen(str) - 1);
    ++cnt;

    result = (char**)malloc(sizeof(char*) * cnt);
    if (result) {
        size_t index = 0;
        char *token = strtok(str, delimiter);
        while (token != NULL) {
            char *token_tmp = token;
            if (token_tmp[strlen(token_tmp) - 1] == '\n') {
                token_tmp[strlen(token_tmp) - 1] = '\0';
            }
            result[index++] = token_tmp;
            token = strtok(NULL, delimiter);
        }
    }

    return result;
}

char* intToStr(int number) {
    int number_lenght = snprintf(NULL, 0, "%d", number);
    char *result = (char*)malloc(number_lenght + 1);
    snprintf(result, number_lenght + 1, "%d", number);
    return result;
}

void create_msg(zmq_msg_t *zmq_msg, char *msg) {
    zmq_msg_init_size(zmq_msg, sizeof(msg));
    memcpy(zmq_msg_data(zmq_msg), msg, sizeof(msg));
}

void send_msg(void *socket, char *msg){
	zmq_msg_t zmq_msg;
	create_msg(&zmq_msg, msg);
	if(!zmq_msg_send(&zmq_msg, socket, 0)) {
        printf("Can not send message");
    }
	zmq_msg_close(&zmq_msg);
}

int main() {
    void *context = zmq_ctx_new();
    void *root_socket = zmq_socket(context, ZMQ_REQ);
    zmq_connect(root_socket, "ipc:///tmp/lab5-7_0");

    Node *tree = NULL;

    char *cmd = NULL;
    size_t cmd_capacity = 0;
    size_t cmd_len = 0; 
    size_t error = -1;

    while (1) {
        printf("> ");

        cmd_len = getline(&cmd, &cmd_capacity, stdin);
        if (cmd_len == error) {
            perror("getline");
            exit(1);
        }

        char **cmd_splited = split(cmd, ' ');
        if (cmd_splited == NULL) {
            perror("split");
            exit(1);
        }

        if (strcmp(cmd_splited[0], "exit") == 0) {
            break;
        }
        else if (strcmp(cmd_splited[0], "print") == 0) {
            printTree(tree);
        }
        else if (strcmp(cmd_splited[0], "create") == 0) {
            char *eptr;
            int id = strtol(cmd_splited[1], &eptr, 10);
            Node *res = search(tree, id);
            if (res != NULL) { 
                printf("Error: Already exists\n");
            }

            char *template = "ipc:///tmp/lab5-7_";
            char *id_str = intToStr(id);
            char *socet_path = (char*)malloc(strlen(template) + strlen(id_str) + 1);
            strcpy(socet_path, template);
            strcat(socet_path, id_str);
            
            pid_t calculationNodePid = fork();
            if (calculationNodePid == -1) {
                printf("Error: Fork\n");
            }
            else if (calculationNodePid == 0) {
                if (execl("./calculationNode.out", "./calculationNode.out", socet_path, NULL) == -1) {
                    printf("Error: Execl");
                }
            }
            else {
                printf("Ok: %d\n", calculationNodePid);
                tree = insert(tree, id);
            }
        } 
        else if (strcmp(cmd_splited[0], "exec") == 0) {
            char *eptr;
            int id = strtol(cmd_splited[1], &eptr, 10);
            Node *res = search(tree, id);
            if (res == NULL) { 
                printf("Error: Not found\n");
            }

            int *path = (int*)malloc(sizeof(int) * getHeight(tree));
            int path_len = pathToNode(tree, id, path, 0);

            char *message = NULL;
            int message_len = 0;

            for (int i = 0; i < path_len; ++i) {
                char *template = " ";

                char *number_to_str = intToStr(path[i]);

                char *str_responce = malloc(strlen(number_to_str) + strlen(template) + 1);
                strcpy(str_responce, number_to_str);
                strcat(str_responce, template);

                message_len += strlen(str_responce);
                message = realloc(message, sizeof(char) * message_len);

                for (int i = message_len - strlen(str_responce), j = 0; i < message_len; ++i, ++j) {
                    message[i] = str_responce[j];
                }
            }

            message_len += 1;
            message = realloc(message, sizeof(char) * message_len);
            message[message_len - 2] = '\n';
            message[message_len - 1] = '\0';

            //send_msg(root_socket, message);
            printf("Sending from control: %s\n", message);
            zmq_send(root_socket, message, message_len, 0);

            char buffer[10];
            zmq_recv(root_socket, buffer, 10, 0);
            printf("Recieve in control_node: %s\n", buffer);

            /*zmq_send(responder, message, message_len, 0);
            sleep(1);
            
            char buffer[10];
            zmq_recv(responder, buffer, 10, 0);
            printf("Recieve in control_node: %s\n", buffer);*/

            free(path);
            free(message);
        } 
        else if (strcmp(cmd_splited[0], "ping") == 0) {
            printf("Ping\n");
        }
        else {
            printf("Unknown command\n");
        }

        cmd_capacity = 0;
    }

    deleteTree(tree);
    zmq_close (root_socket);
    zmq_ctx_destroy (context);
    return 0;
}