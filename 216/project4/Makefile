# Sanjana Vellanki
# UID: 117990881
# Directory ID: svellank
# Last Updated: December 12, 2022


CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Werror -Wshadow -Wwrite-strings \
-pedantic-errors -fstack-protector-all

public01: calendar.o public01.o my_memory_checker_216.o
	$(CC) calendar.o public01.o my_memory_checker_216.o -o public01

public01.o: public01.c event.h calendar.h
	$(CC) $(CFLAGS) -c public01.c 

public02.o: public02.c event.h calendar.h
	$(CC) $(CFLAGS) -c public02.c

public03.o: public03.c event.h calendar.h
	$(CC) $(CFLAGS) -c public03.c

public04.o: public04.c event.h calendar.h
	$(CC) $(CFLAGS) -c public04.c

public05.o: public05.c event.h calendar.h
	$(CC) $(CFLAGS) -c public05.c

student_tests.o: student_tests.c event.h calendar.h	
	$(CC) $(CFLAGS) -c student_tests.c -o student_tests.o

calendar.o: calendar.c calendar.h event.h
	$(CC) $(CFLAGS) -c calendar.c -o calendar.o

my_memory_checker_216.o: my_memory_checker_216.c my_memory_checker_216.h
	$(CC) $(CFLAGS) -c my_memory_checker_216.c -o my_memory_checker_216.o

clean:
	rm shell_jr ./*.o

