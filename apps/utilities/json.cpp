#include <libutils/json/Json.h>
#include <libutils/ArgParse.h>

constexpr auto PROLOGUE = "Reformats JSON to make it easier to read.";

constexpr auto OPTION_COLOR_DESCRIPTION = "Color json levels using VT100 sequences.";
constexpr auto OPTION_IDENT_DESCRIPTION = "Ident json levels.";

constexpr auto EPILOGUE = "Options can be combined";

int main(int argc, char const *argv[])
{
    ArgParse args{};
    args.should_abort_on_failure();

    args.prologue(PROLOGUE);

    args.usage("");
    args.usage("OPTION...");
    args.usage("OPTION... FILES...");

    auto options = Prettifier::NONE;

    args.option('c', "color", OPTION_COLOR_DESCRIPTION, [&](auto &) {
        options |= Prettifier::COLORS;
    });

    args.option('i', "indent", OPTION_IDENT_DESCRIPTION, [&](auto &) {
        options |= Prettifier::INDENTS;
    });

    args.epiloge(EPILOGUE);

    args.eval(argc, argv);

    if (args.argc() == 0)
    {
        StreamScanner scanner{in_stream};
        auto root = json::parse(scanner);

        Prettifier pretty{options};
        json::prettify(pretty, root);
        printf("%s", pretty.finalize().cstring());
    }
    else
    {
        args.argv().foreach ([&](auto &path) {
            auto root = json::parse_file(path.cstring());

            Prettifier pretty{options};
            json::prettify(pretty, root);
            printf("%s", pretty.finalize().cstring());

            return Iteration::CONTINUE;
        });
    }

    return PROCESS_SUCCESS;
}
