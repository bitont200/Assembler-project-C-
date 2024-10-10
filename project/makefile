assembler: assembler.o macro_structure.o preprocessor.o line_structure.o command_parsing.o generic_linked_list.o tables.o first_pass.o first_pass_func.o second_pass.o second_pass_func.o octal_translation.o
	gcc -ansi -pedantic -Wall -g assembler.o macro_structure.o preprocessor.o line_structure.o command_parsing.o generic_linked_list.o tables.o first_pass.o first_pass_func.o second_pass.o second_pass_func.o octal_translation.o -o assembler
assembler.o: assembler.c
	gcc -c -ansi -pedantic -Wall -g assembler.c -o assembler.o
macro_structure.o: macro_structure.c macro_structure.h
	gcc -c -ansi -pedantic -Wall -g macro_structure.c -o macro_structure.o
preprocessor.o: preprocessor.c preprocessor.h
	gcc -c -ansi -pedantic -Wall -g preprocessor.c -o preprocessor.o
line_structure.o: line_structure.c line_structure.h
	gcc -c -ansi -pedantic -Wall -g line_structure.c -o line_structure.o
command_parsing.o: command_parsing.c command_parsing.h
	gcc -c -ansi -pedantic -Wall -g command_parsing.c -o command_parsing.o
generic_linked_list.o: generic_linked_list.c generic_linked_list.h
	gcc -c -ansi -pedantic -Wall -g generic_linked_list.c -o generic_linked_list.o
tables.o: tables.c tables.h
	gcc -c -ansi -pedantic -Wall -g tables.c -o tables.o
first_pass.o: first_pass.c first_pass.h
	gcc -c -ansi -pedantic -Wall -g first_pass.c -o first_pass.o
first_pass_func.o: first_pass_func.c first_pass_func.h
	gcc -c -ansi -pedantic -Wall -g first_pass_func.c -o first_pass_func.o
second_pass.o: second_pass.c second_pass.h
	gcc -c -ansi -pedantic -Wall -g second_pass.c -o second_pass.o
second_pass_func.o: second_pass_func.c second_pass_func.h
	gcc -c -ansi -pedantic -Wall -g second_pass_func.c -o second_pass_func.o
octal_translation.o: octal_translation.c octal_translation.h
	gcc -c -ansi -pedantic -Wall -g octal_translation.c -o octal_translation.o
