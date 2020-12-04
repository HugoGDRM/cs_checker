#define _GNU_SOURCE

#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 25
#define MAX_FUNCTION 10
#define MAX_DEFAULT 5
#define MAX_STATIC 5

static int fail = 0;
static int nb_line = 0;
static int nb_function = 0;
static int nb_default_function = 0;
static int nb_static_function = 0;

static FILE *open_file(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
        errx(1, "fopen fail");
    return file;
}

static int is_static(char *line)
{
    char *find = strstr(line, "static");
    if (find == NULL)
        return 0;
    return 1;
}

static char *is_function(char *line)
{
    size_t i = 0;
    size_t len = strlen(line);
    while (line[i] != '\0' && line[i] != '(')
        i++;

    if (i <= len && line[i] == '(' && line[len - 2] == ')')
    {
        i = 0;
        while (line[i] != ' ')
            i++;

        while (line[i] == ' ')
            i++;

        size_t len =  strcspn(line + i, "(");
        char *name = calloc(len + 1, sizeof(char));
        size_t y = 0;
        while (y < len)
        {
            name[y] = line[i + y];
            y++;
        }
        name[y] = '\0';
        return name;
    }
    return NULL;
}

static int contain_hook(char *line)
{
    int in_quote = 0;
    size_t i = 0;
    while (line[i] != '\0')
    {
        if (line[i] == '\'' || line[i] == '\"')
            in_quote = !in_quote;

        if (in_quote == 0 && (line[i] == '{' || line[i] == '}'))
            return 1;

        i++;
    }
    return 0;
}

static int is_close_hook(char *line)
{
    return line[0] == '}' && line[1] == '\n';
}

static int is_blank(char *line)
{
    return line[0] == '\n';
}

static int is_legit_line(char *line)
{
    return is_blank(line) == 0 && contain_hook(line) == 0;
}

static void is_fail(int n, int max)
{
    if (n > max)
    {
        fail = 1;
        printf("                              <--- FAIL\n");
    }
    else
        printf("\n");
}

static void print_result()
{
    printf("\n");
    printf("Nombre de fonctions exportée: %d", nb_default_function);
    is_fail(nb_default_function, 5);
    printf("Nombre de fonctions statique: %d", nb_static_function);
    is_fail(nb_static_function, 5);
    printf("Nombre de fonctions total: %d", nb_function);
    is_fail(nb_function, 10);
}

static int is_valid_filename(char *filename)
{
    size_t len = strlen(filename);
    return filename[len - 2] == '.' && filename[len - 1] == 'c';
}

int main(int argc, char **argv)
{
    printf("\n");

    if (argc != 2 && is_valid_filename(argv[1]) == 0)
        errx(1, "invalid input");

    FILE *file = open_file(argv[1]);

    char *line;
    size_t n = 0;
    size_t read;

    char *name;
    int in_func = 0;
    while ((read = getline(&line, &n, file) != -1))
    {
        if (in_func == 0 && (name = is_function(line)) != NULL)
        {
            nb_line = 0;
            nb_function++;
            if (is_static(line) == 1)
                nb_static_function++;
            else
                nb_default_function++;
            in_func = 1;
        }
        else if(in_func == 1 && is_legit_line(line))
            nb_line++;

        if (is_close_hook(line) == 1)
        {
            in_func = 0;
            printf("%s contient %d lignes", name, nb_line);
            is_fail(nb_line, 25);
            free(name);
        }
    }

    free(line);
    print_result();
    printf("\n");

    if (fail == 1)
        return 1;
    return 0;
}
