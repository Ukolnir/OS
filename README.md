# OS
lab2.sh - Задание: Написать сценарий, который выводит для каждого своего аргумента вывод команды ls -l, 
если аргумент является именем регулярного файла. Если сценарий запущен без аргументов, он должен вывести сообщение 
об использовании сценария.

lab3.sh - Задание: Написать сценарий, который будет выбирать 20 случайных лет в диапазоне от 1000 до 2100 и проверять 
каждый из них на високосность. Необходимо на отдельной строке вывести год и результат проверки.

lab4.sh - Задание: Написать сценарий, который будет удалять пустые строки из заданного файла. 
Если сценарий запущен без аргументов, он должен вывести сообщение об использовании сценария.

lab5.c - Задание: Вычислить "контрольную сумму" файла: байт, являющийся результатом выполнения 
побитовой операции "исключающее или" для всех байт файла.

lab_ad.c - Задание: Определить, является ли содержимое заданного файла палиндромом. Решить задачу с 
помощью отображения файлов в память.

POSIX8.c - Задание: Реализовать программу, которая дожидается запуска заданного количества экземпляров своего процесса, которым назначаются последовательные номера. Дальше первый процесс генерирует случайное число, которое передаёт второму процессу. Второй процесс увеличивает число на 1 и передаёт дальше. Последний процесс после увеличения числа передаёт его первому процессу, который после его увеличения выводит на печать. Передачу значений можно организовать при помощи массива размером, равным количеству процессов. Выполнить задание при помощи интерфейса POSIX.

 - Задание: Родительский процесс должен считать пять строк и передать их 
дочернему процессу. Дочерний процесс должен выполнить замену всех вхождений 
второй строки в первой на третью. Результат замены вместе с четвёртой и пятой 
строкой дочерний процесс должен передать своему дочернему процессу, который, 
аналогично, должен выполнить замену и вывести свой результат на печать. 
Задание реализуется в двух вариантах: при помощи интерфейсов Windows API и 
POSIX. Для взаимодействия процессов необходимо использовать неименованные 
каналы.Считывать входные данные можно из стандартного ввода, текстового файла 
или аргументов командной строки.


