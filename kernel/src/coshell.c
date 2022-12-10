#include "coshell.h"
#include "terminal.h"
#include "keyboard.h"
#include "string.h"

struct command_entry
{
    const char* command;
    void (*handler)(const char* arg);
    const char* description;
    const char* detail;
};

static void on_line_input(const char* line);
static void command_echo(const char* arg);
static void command_reset(const char* arg);
static void command_help(const char* arg);

static struct command_entry entries[] = {
    { "echo",   command_echo,   "echo your inputs",     NULL },
    { "reset",  command_reset,  "reset computer",       NULL },
    { "help",   command_help,   "show help",            "Usage) help (specific command)" },
};

void coshell_init(void)
{
}

void coshell_on_input(uint8_t cascii)
{
    enum term_mode mode = term_get_mode();
    char input[81];

    if (mode != TERM_MODE_NORMAL && cascii == CASCII_ESC)
    {
        term_mode_normal();
    }
    else if (mode == TERM_MODE_NORMAL && cascii == ':')
    {
        term_mode_input(":");
    }
    else if (mode == TERM_MODE_INPUT)
    {
        term_on_input(cascii);
        if (term_get_input(input, sizeof(input)))
        {
            on_line_input(input);
        }
    }
}

static void on_line_input(const char* line)
{
    char buf[strlen(line) + 1];
    strcpy(buf, line);

    term_write_format(TERM_COLOR_DEFAULT, "> %s\n", line);

    char* cmd = strchr_not(buf, ' ');
    if (!cmd)
        return;

    char* arg = strchr(cmd, ' ');
    if (arg)
    {
        *arg++ = 0;
        arg = strchr_not(arg, ' ');
    }

    const size_t len = sizeof(entries) / sizeof(entries[0]);
    for (unsigned idx = 0; idx < len; ++idx)
    {
        if (strcmp(entries[idx].command, cmd) == 0)
        {
            entries[idx].handler(arg);
            return;
        }
    }

    term_write_format(TERM_LIGHT_RED, "'%s' is not a valid command\n", line);
}

static void command_echo(const char* arg)
{
    term_write_format(TERM_COLOR_DEFAULT, "%s\n", arg ? arg : "");
}

static void command_reset(const char* arg)
{
    keyboard_resetPC();
}

static void show_help(const struct command_entry* entry)
{
    term_write_format(TERM_COLOR_DEFAULT, "%s : %s\n", entry->command, entry->description);
}
static void command_help(const char* arg)
{
    if (arg == NULL)
    {
        struct command_entry *end = entries + sizeof(entries) / sizeof(entries[0]);
        unsigned line = 0;

        for (struct command_entry *entry = entries; entry < end; ++entry)
        {
            show_help(entry);
        }
        term_write_string(TERM_COLOR_DEFAULT, "To see more detail help of specific command, enter \"help [command]\"\n");
    }
    else
    {
        struct command_entry *end = entries + sizeof(entries) / sizeof(entries[0]);
        struct command_entry *entry;
        for (entry = entries; entry < end; ++entry)
        {
            if (strcmp(entry->command, arg) == 0)
                break;
        }

        if (entry != end)
        {
            show_help(entry);
            if (entry->detail != NULL)
            {
                term_write_string(TERM_COLOR_DEFAULT, entry->detail);
                term_write_string(TERM_COLOR_DEFAULT, "\n");
            }
        }
        else
        {
            term_write_format(TERM_COLOR_DEFAULT, "'%s' is not a command. type <help> to see help of whole commands.\n", arg);
        }
    }
}
