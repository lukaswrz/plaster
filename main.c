#include "scanner.h"
#include "w.h"

#define READARG_IMPLEMENTATION
#include "readarg.h"

enum opts {
    OPT_HELP,
    // OPT_DATA,
    OPT_FORMAT,
    // OPT_FILTER
};

enum opers {
    OPER_FILE
};

void usage(char *argv0);

int main(int argc, char **argv) {
    struct readarg_opt opts[] = {
        [OPT_HELP] = {
            .names = {
                [READARG_FORM_SHORT] = READARG_STRINGS("h"),
                [READARG_FORM_LONG] = READARG_STRINGS("help"),
            },
            .arg.bounds.val = {
                1,
            },
        },
        [OPT_FORMAT] = {
            .names = {
                [READARG_FORM_SHORT] = READARG_STRINGS("f"),
                [READARG_FORM_LONG] = READARG_STRINGS("format"),
            },
            .req = 1,
            .arg.bounds.val = {
                1,
            },
        },
        // [OPT_FILTER] = {
        //     .names = {
        //         [READARG_FORM_SHORT] = READARG_STRINGS("F"),
        //         [READARG_FORM_LONG] = READARG_STRINGS("filter"),
        //     },
        //     .arg.bounds.inf = 1,
        // },
        {
            0,
        },
    };

    struct readarg_arg opers[] = {
        // If no file has been specified, stdin will be used instead.
        [OPER_FILE] = {
            .name = "file",
            .bounds.inf = 1,
        },
        {
            0,
        },
    };

    struct readarg_parser rp;
    readarg_parser_init(
        &rp,
        opts,
        opers,
        (struct readarg_view_strings){
            .strings = (const char **)argv + 1,
            .len = argc - 1});

    while (readarg_parse(&rp))
        ;

    if (rp.error != READARG_ESUCCESS)
    {
        // TODO: Switch on error
        w_fprintf(stderr, "Error: %d\n", rp.error);
        usage(argv[0]);
        return 1;
    }

    if (rp.opts[OPT_HELP].arg.val.len >= 1) {
        usage(argv[0]);
        return 0;
    }

    struct scanner_format format = {
        .start = '$',
        .sep = ':',
        .open = '{',
        .close = '}'
    };

    if (rp.opts[OPT_FORMAT].arg.val.len == 1) {
        const char *formatstr = rp.opts[OPT_FORMAT].arg.val.strings[0];

        char *out[] = {
            &format.start,
            &format.sep
        };
        for (size_t i = 0; i < sizeof out / sizeof *out; i++) {
            switch (*formatstr) {
            case '!':
            case '"':
            case '$':
            case '%':
            case '&':
            case '/':
            case '=':
            case '?':
            case '*':
            case '+':
            case '~':
            case '\'':
            case '#':
            case '-':
            case '_':
            case ',':
            case ';':
            case '.':
            case ':':
                *out[i] = *formatstr++;
                break;
            default:
                w_fprintf(stderr, "Invalid format\n");
                return 1;
            }
        }

        const char *brackets[] = { "()", "{}", "[]", "<>" };
        const char *match = NULL;
        for (size_t i = 0; i < sizeof brackets / sizeof *brackets; i++) {
            if (strncmp(formatstr, brackets[i], strlen(brackets[i])) == 0) {
                match = brackets[i];
                break;
            }
        }
        if (match == NULL) {
            w_fprintf(stderr, "Invalid format\n");
        }

        format.open = *(formatstr++);
        format.close = *(formatstr++);
    }

    struct scanner sc;
    scanner_init(&sc, format);

    return 0;
}

void usage(char *argv0) {
    w_fprintf(stderr,
        "Usage: %s\n",
        argv0
    );
}
