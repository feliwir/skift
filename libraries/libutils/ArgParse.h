#pragma once

#include <libsystem/process/Process.h>

#include <libutils/Callback.h>
#include <libutils/Optional.h>
#include <libutils/Scanner.h>
#include <libutils/String.h>
#include <libutils/Traits.h>
#include <libutils/Vector.h>

#include <stdio.h>

class ArgParseContext
{
private:
    Vector<String> _arguments;

public:
    ArgParseContext(int argc, char const *argv[])
    {
        for (int i = 0; i < argc; i++)
        {
            _arguments.push_back(argv[i]);
        }
    }

    bool any()
    {
        return _arguments.any();
    }

    const String &current()
    {
        return _arguments.peek();
    }

    Vector<String> pop_operands()
    {
        Vector<String> operands;

        while (any() && current()[0] != '-')
        {
            operands.push_back(pop());
        }

        return operands;
    }

    Optional<String> pop_operand()
    {
        if (current()[0] != '-')
        {
            return pop();
        }
        else
        {
            return {};
        }
    }

    String pop()
    {
        return _arguments.pop();
    }
};

using ArgParseOptionCallback = Callback<void(ArgParseContext &)>;

struct ArgParseOption
{
    char shortname;
    String longname{};
    String description{};
    ArgParseOptionCallback callback;
};

class ArgParse
{
private:
    String _name;
    Vector<String> _usages;
    Vector<ArgParseOption> _option;
    String _prologue;
    String _epiloge;
    Vector<String> _argv;

    bool _should_abort_on_failure = false;
    bool _show_help_if_no_operand_given = false;
    bool _show_help_if_no_option_given = false;

public:
    static constexpr char NO_SHORT_NAME = '\0';

    void prologue(String prologue) { _prologue = prologue; }

    void usage(String usage) { _usages.push_back(usage); }

    void epiloge(String epiloge) { _epiloge = epiloge; }

    auto argc() { return _argv.count(); }

    auto &argv() { return _argv; }

    void should_abort_on_failure() { _should_abort_on_failure = true; }

    void show_help_if_no_operand_given() { _show_help_if_no_operand_given = true; }

    void show_help_if_no_option_given() { _show_help_if_no_option_given = true; }

    ArgParse()
    {
        _option.push_back({'h', "help", "Show this help message and exit.", [&](auto &) {
                               help();
                               process_exit(PROCESS_SUCCESS);
                           }});
    }

    void option(char shortname, String longname, String description, ArgParseOptionCallback callback)
    {
        _option.push_back({shortname, longname, description, callback});
    }

    void option_string(char shortname, String longname, String description, Callback<void(String &)> callback)
    {
        _option.push_back({
            shortname,
            longname,
            description,

            [this, callback](ArgParseContext &ctx) {
                auto maybe_strings = ctx.pop_operand();

                if (maybe_strings)
                {
                    callback(*maybe_strings);
                }
                else
                {
                    usage();
                }
            },
        });
    }

    void option(bool &value, char shortname, String longname, String description)
    {
        option(shortname, longname, description, [&](auto &) {
            value = true;
        });
    }

    void option(String &value, char shortname, String longname, String description)
    {
        option(shortname, longname, description, [&](auto &ctx) {
            if (ctx.any())
            {
                value = ctx.pop();
            }
            else
            {
                usage();
            }
        });
    }

    void help()
    {
        if (!_prologue.null_or_empty())
        {
            printf("%s\n", _prologue.cstring());
            printf("\n");
        }

        if (_usages.any())
        {
            printf("\e[1mUsages:\e[0m\n");

            for (size_t i = 0; i < _usages.count(); i++)
            {
                printf("    %s %s\n", _name.cstring(), _usages[i].cstring());
            }

            printf("\n");
        }

        if (_option.any())
        {
            auto compute_padding = [&](auto &options) {
                size_t max = 0;

                for (size_t i = 0; i < options.count(); i++)
                {
                    if (!options[i].longname.null_or_empty())
                    {
                        max = MAX(max, options[i].longname.length());
                    }
                }

                return max;
            };

            auto padding = compute_padding(_option);

            printf("\e[1mOptions:\e[0m\n");

            for (size_t i = 0; i < _option.count(); i++)
            {
                auto &opt = _option[i];

                printf("    ");

                if (opt.shortname != '\0')
                {
                    printf("-%c", opt.shortname);
                }
                else
                {
                    printf("  ");
                }

                if (opt.shortname && !opt.longname.null_or_empty())
                {
                    printf(", ");
                }
                else
                {
                    printf("  ");
                }

                if (!opt.longname.null_or_empty())
                {
                    printf("--%s", opt.longname.cstring());

                    if (padding > opt.longname.length())
                    {
                        size_t computed_padding = padding - opt.longname.length();

                        for (size_t i = 0; i < computed_padding; i++)
                        {
                            printf(" ");
                        }
                    }
                }
                else
                {
                    for (size_t i = 0; i < padding; i++)
                    {
                        printf(" ");
                    }
                }

                if (!opt.description.null_or_empty())
                {
                    printf(" ");
                    printf("%s", opt.description.cstring());
                }

                printf("\n");
            }

            printf("\n");
        }

        if (!_epiloge.null_or_empty())
        {
            printf("%s\n", _epiloge.cstring());
        }
    }

    void fail()
    {
        stream_format(err_stream, "Try '%s --help' for more information.\n", _name.cstring());

        if (_should_abort_on_failure)
        {
            process_exit(PROCESS_FAILURE);
        }
    }

    void usage()
    {
        stream_format(err_stream, "%s: missing operand.\n", _name.cstring());
        fail();
    }

    void invalid_option(String option)
    {
        stream_format(err_stream, "%s: invalide option '%s'!\n", _name.cstring(), option.cstring());
        fail();
    }

    int eval(int argc, char const *argv[])
    {
        ArgParseContext context{argc, argv};

        _name = context.pop();

        auto has_run_option = false;

        while (context.any())
        {
            auto current = context.pop();
            StringScanner scan{current.cstring(), current.length()};

            if (scan.skip_word("--"))
            {
                String argument{current.cstring() + 2, current.length() - 2};

                bool found_valid_option = false;

                for (size_t i = 0; i < _option.count(); i++)
                {
                    if (_option[i].longname == argument)
                    {
                        _option[i].callback(context);
                        has_run_option = true;
                        found_valid_option = true;
                    }
                }

                if (!found_valid_option)
                {
                    invalid_option(current);
                    return PROCESS_FAILURE;
                }
            }
            else if (scan.skip('-'))
            {
                while (scan.do_continue())
                {
                    bool found_valid_option = false;

                    for (size_t i = 0; i < _option.count(); i++)
                    {
                        if (_option[i].shortname == scan.current())
                        {
                            _option[i].callback(context);

                            has_run_option = true;
                            found_valid_option = true;
                        }
                    }

                    if (!found_valid_option)
                    {
                        invalid_option(scan.current());
                        return PROCESS_FAILURE;
                    }

                    scan.foreward();
                }
            }
            else
            {
                _argv.push_back(current);
            }
        }

        if (!this->argv().any() && _show_help_if_no_operand_given)
        {
            usage();
            return PROCESS_FAILURE;
        }

        if (!has_run_option && _show_help_if_no_option_given)
        {
            usage();
            return PROCESS_FAILURE;
        }

        return PROCESS_SUCCESS;
    }
};
