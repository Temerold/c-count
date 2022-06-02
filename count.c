#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>
#include <inttypes.h>

// ! IMPORTANT: Remember to run the `compile_count_c.bat` script when `count.c` (this file)
// ! is altered, to compile it, and then add the .exe (c-count.exe) file to the git commit

int digits_only(const char s[])
{
    if (strcmp(s, "-1") == 0)
    {
        return 1;
    }
    else
        while (*s)
        {
            if (isdigit(*s++) == 0)
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
        printf("ERROR: Invalid input! Only the colors red and white are allowed.");
        change_color("white");
        exit(1);
    }
}

void count(int start, int end)
{
    // ´start´ is greater than ´end´ (´end´ can however be -1, which means infinity)
    if (end < start && end != -1)
    {
        change_color("red");
        printf("ERROR: Invalid input! Start integer can't be greater than end integer."
               "\n");
        change_color("white");
        exit(2);
    }
    else if (start < 0) // ´start´ is negative
    {
        change_color("red");
        printf("ERROR: Invalid input! Start integer can't be a negative number.\n");
        change_color("white");
        exit(2);
    }
    // ´end´ is negative (it can however be -1, which means infinity)
    else if (end < 0 && end != -1)
    {
        change_color("red");
        printf("ERROR: Invalid input! End integer can't be a negative number (except for"
               "-1, which means infinity).\n");
        change_color("white");
        exit(2);
    }

    // Start counting at ´start´, and end at ´end´. If ´end´ is -1, the program will
    // continue forever
    for (int num = start; num <= (end) || end == -1; ++num)
    {
        printf("\n%d", num);
    }
}

int main(int argc, char *argv[])
{
    if (argc > 3) // More than 3 arguments
    {
        change_color("red");
        printf("ERROR: Too many arguments! The maximum is 2; start integer and end "
               "integer.\n");
        change_color("white");
        exit(2);
    }
    else if (argc < 3) // Less than 3 arguments
    {
        change_color("red");
        printf("ERROR: Missing argument(s)! Requires both start integer and end integer."
               "\n");
        change_color("white");
        exit(2);
    }
    else if (digits_only(argv[1]) == 0 || digits_only(argv[2]) == 0)
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
