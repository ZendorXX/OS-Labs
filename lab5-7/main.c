#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

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
Формат команды сохранения значения: exec id subcommand
subcommand – одна из трех команд: start, stop, time.
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

int main() {
    Node *tree = NULL;
   
    char cmd[10];
    while (1) {
        printf("> ");
        scanf("%s", cmd);

        if (strcmp(cmd, "exit") == 0) {
            return 0;
        }
        else if (strcmp(cmd, "print") == 0) {
            printTree(tree);
        }
        else if (strcmp(cmd, "create") == 0) {
            scanf("%s", cmd);
            char *eptr;
            int id = strtol(cmd, &eptr, 10);
            Node *res = search(tree, id);
            if (res != NULL) { 
                printf("Calculation node with this id already exist\n");
            }
            tree = insert(tree, id);
        } 
        else if (strcmp(cmd, "exec") == 0) {
            scanf("%s", cmd);
            pid_t proc = fork();
            if (proc == 0) {
                execl("./calculationNode.out", "./calculationNode.out", cmd, NULL);
            }
        } 
        else if (strcmp(cmd, "ping") == 0) {
            printf("Ping\n");
        }
    }

    deleteTree(tree);

    return 0;
}