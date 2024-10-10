/** @name MAMAN 14 The summuray project
* @authors Shira Kedmi & Tamar Biton
* @date 11.08.24
* @brief This program implements an assembler in C language.
* The assembler receives one or more files in assembly language 
* defined in the instructions, and starts to operate on each file individually.
* At first, the program works on the pre-processor stage, which includes 
* retires a macro and checking its integrity. If an error is detected at this stage, 
* the program moves on to the next file.
* In the second step, the program starts working on encoding the file in 2 passes.
* In the first pass, the program analyzes the type of command, analyzes the syntax 
* of the command, and checks the correctness of the command. If the command is incorrect, 
* the program moves to the next file.
* In the second pass, a translation of the label addresses is performed and another 
* correctness check is made, also here if an error is detected, the program moves to the 
* next file.
* If no errors were detected during the transition to the program, the program encodes 
* the commands and enters them into memory.
* @assemption 
* @note Our assemption:
*       - In the preprocessor phase, if in the same line there are 2 calls to the macro 
*         that has already been defined, the macro is opened only once 
*       - In .string if the string given has white spaces - the program translate them as well
*       - If given an invalid name of file (not existing file) exit from the program
*/
#include "assembler.h"

int main(int argc, char *argv[]) {
    int i,flag;
    Macro * macro;
    FILE *am_file;
    char file_name[LENGTH_FILE_NAME];
    struct assembler_table table = {0};
    macro = NULL;

    printf("\n\t\tThis program implements an assembler in C language on the given files.\n\n");
    if (argc < MIN_NUM_ARG) {
        fprintf(stderr,"Error: No input files provided\n\n---------------------------------------------\n\n");
        return EXIT_FAILURE;
    }


    for (i = 1; i < argc; i++) {
        flag = 0;
        printf("Start preproccessor for %s file.\n", argv[i]);
        flag = preprocessor(argv[i],&macro);
        if(flag == ERROR){
            fprintf(stderr,"preproccesor for file %s faild.\n\n---------------------------------------------\n\n", argv[i]);
        } else {
            create_file(file_name, argv[i], SUFFIX_AM);

            am_file = fopen(file_name, "r+");
            if (am_file == NULL) {
                fprintf(stderr,"Error opening .am file\n");
                flag = ERROR;
            }
            if (flag != ERROR) {
                printf("Start the first pass for %s file.\n", argv[i]);
                if (first_pass(&table, am_file, macro) == ERROR) {
                    fprintf(stderr,"First pass failed.\n\n---------------------------------------------\n\n");
                    flag = ERROR;
                }
                if (flag != ERROR) {
                    flag = 0;
                    rewind(am_file);

                    printf("Start the second pass for %s file.\n", argv[i]);
                    if (second_pass(&table, am_file, macro) == ERROR) {
                        fprintf(stderr,"Second pass failed.\n\n---------------------------------------------\n\n");
                        flag = ERROR;
                    }
                    if (flag != ERROR) {
                        ob_file(&table, argv[i]);
                        entries_file(&table, argv[i]);
                        externals_file(&table, argv[i]);
                        printf("Succeed the %s file.\n\n---------------------------------------------\n\n", argv[i]);
                    }
                }
                fclose(am_file);
                ll_destroy_symbols(&table.symbol_table);
            }
            free_macro_table(&macro);  
        }
    }
    return EXIT_SUCCESS;
}
