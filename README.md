# SimpleContainer

## Что это?
Программа на си, которая с помощью clone создает новые пространства имен (в частности пространство файловой системы и UTS) для процесса.
Далее в этом процессе-песочнице изменяются эти пространства и выполняется программа, переданная в качестве аргумента (для примера простой сервер, написанный на Go).

собрать -- gcc main.c -o main; go build (внутри simpleGoServer)
запустить -- ./main ./simpleGoServer/http-server