#if _WIN32_WINNT < 0x0500
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>
#include <inttypes.h>

// ! IMPORTANT: Remember to run the `compile.bat` script when `count.c` (this file) is
// ! altered -- to compile it -- and then add the .exe (`C-Count.exe`) file to the git
// ! commit!

void wait_for_newline(char *text)
{
    printf(text);
    char prev = 0;
    while (1)
    {
        char c = getchar();
        if (c == '\n' && prev == c)
        {
            break;
        }
        prev = c;
    }
}

int is_int(const char s[])
{
    if (strcmp(s, "-1") == 0) // If string equals "-1"
    {
        return 1;
    }

    // Loop through characters, return 0 if non-integer found
    for (size_t i; s[i] != '\0'; i++)
    {
        if (!isdigit(s[i]))
        {
            return 0;
        }
    }
    return 1;
}

void change_color(char color[])
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (strcmp(color, "red") == 0)
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    }
    else if (strcmp(color, "white") == 0)
    {
        // Reset to white
        SetConsoleTextAttribute(
            hConsole,
            FOREGROUND_RED |
                FOREGROUND_GREEN |
                FOREGROUND_BLUE |
                FOREGROUND_INTENSITY);
    }
    else
    {
        change_color("red");
        wait_for_newline("\nERROR: Invalid input! Only the colors red and white are "
                         "allowed.\n");
        change_color("white");
        exit(1);
    }
}

void count(unsigned int start, unsigned int end)
{
    // `start` is greater than `end` (`end` can however be -1, which means infinity)
    if (end < start && end != -1)
    {
        change_color("red");
        wait_for_newline("\nERROR: Invalid input! Start integer can't be greater than end"
                         " integer.\n");
        change_color("white");
        exit(2);
    }
    else if (start < 0) // `start` is negative
    {
        change_color("red");
        wait_for_newline("\nERROR: Invalid input! Start integer can't be a negative "
                         "number.\n");
        change_color("white");
        exit(2);
    }
    // `end` is negative (it can however be -1, which means infinity)
    else if (end < 0 && end != -1)
    {
        change_color("red");
        wait_for_newline("\nERROR: Invalid input! End integer can't be a negative number "
                         "(except for -1, which means infinity).\n");
        change_color("white");

        exit(2);
    }

    // Here, we have this if-else statement, with different code based on if `end` is -1
    // or not. Yes, we could just have one single for loop, which would continue forever
    // if `end` is -1. That, however, would be inefficient; checking if `end` is -1 every
    // recursion -- it's not going to change, so why check? Instead, we have a separate
    // never-ending while loop if `end` is -1, and it'll never stop and check `end`'s
    // value.
    if (end == -1)
    {
        unsigned int num = start;
        while (1)
        {
            printf("\n%d", num);
            num += 1;
        }
    }
    else
    {
        for (unsigned int num = start; num <= end; num++)
        {
            printf("\n%d", num);
        }
    }
}

// This function runs automatically every time the program is ran. If the program is ran
// as a standalone, it will run without a GUI, asking for a start and end number.
// * In Visual Studio Code, the following line gets marked as an error, but compiles and
// * runs correctly. This is a documented bug:
// * https://developercommunity.visualstudio.com/t/cc-intellisense-in-gccclang-mode-shows-attribute-c/796872
void __attribute__((constructor)) start_up(void)
{
    DWORD dwProcessId;
    GetWindowThreadProcessId(GetConsoleWindow(), &dwProcessId);
    if (GetCurrentProcessId() == dwProcessId) // If ran as a standalone
    {
        unsigned int start, end;

        printf("Start at: ");
        scanf("%d", &start);
        printf("End at: ");
        scanf("%d", &end);

        count(start, end);
        wait_for_newline("\n");
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    if (argc > 3) // More than 2 arguments
    {
        change_color("red");
        printf("ERROR: Too many arguments! The maximum is 2; start integer and end "
               "integer.\n");
        change_color("white");
        exit(2);
    }
    else if (argc < 3) // Fewer than 2 arguments
    {
        change_color("red");
        printf("ERROR: Missing argument(s)! Requires both start integer and end integer."
               "\n");
        change_color("white");
        exit(2);
    }
    else if (is_int(argv[1]) == 0 || is_int(argv[2]) == 0)
    {
        change_color("red");
        printf("ERROR: Invalid input! Start integer and end integer must be entirely "
               "numerical.\n");
        change_color("white");
        exit(2);
    }

    const char *start_param = argv[1];
    const char *end_param = argv[2];

    char *endptr;
    // Convert chars to 64 bit ints
    long start = strtoll(start_param, &endptr, 10);
    long end = strtoll(end_param, &endptr, 10);

    count(start, end);
    exit(0);
}
