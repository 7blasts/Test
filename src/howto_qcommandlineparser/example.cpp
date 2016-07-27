/* QCommandLineParser demo.

Parse command line options for a hypothetical program with these options:

Usage: ./disasm8080 [options] filename
Disassembler for the Intel 8080 microprocessor.

Options:
  -h, --help               Displays this help.
  -v, --version            Displays version information.
  -n, --no-list            Don't list instruction bytes (make output suitable f
                           or assembler).
  -u, --uppercase          Use uppercase for mnemonics.
  -a, --address <address>  Specify decimal starting address (default is 0).
  -f, --format <format>    Use number format: 1=$1234 2=1234h 3=1234 4=177777 (
                           default is 1).

Arguments:
  filename                 Binary file to disassemble.

*/

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QString>
#include <QStringList>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("disasm8080");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main", "Disassembler for the Intel 8080 microprocessor."));

    // Standard -h / --help options.
    parser.addHelpOption();

    // Standard -v / --version options.
    parser.addVersionOption();

    // Accepts a single filename argument.
    parser.addPositionalArgument("filename", QCoreApplication::translate("main", "Binary file to disassemble."));

    // -n / --no-list option.
    QCommandLineOption noListOption(QStringList() << "n" << "no-list", QCoreApplication::translate("main", "Don't list instruction bytes (make output suitable for assembler)."));
    parser.addOption(noListOption);

    // -u / --uppercase option.
    QCommandLineOption uppercaseOption(QStringList() << "u" << "uppercase", QCoreApplication::translate("main", "Use uppercase for mnemonics."));
    parser.addOption(uppercaseOption);

    // -a ADDRESS / --address ADDRESS option.
    QCommandLineOption addressOption(QStringList() << "a" << "address", QCoreApplication::translate("main", "Specify decimal starting address (default is 0)."), QCoreApplication::translate("main", "address"), "0");
    parser.addOption(addressOption);

    // -f FORMAT / --format FORMAT option.
    QCommandLineOption formatOption(QStringList() << "f" << "format", QCoreApplication::translate("main", "Use number format: 1=$1234 2=1234h 3=1234 4=177777 (default is 1)."), QCoreApplication::translate("main", "format"), "1");
    parser.addOption(formatOption);

    parser.process(app);

    const QStringList args = parser.positionalArguments();

    // Check that one and only one filename option was specified.
    if (args.size() != 1) {
        fprintf(stderr, "%s\n", qPrintable(QCoreApplication::translate("main", "Error: Must specify one filename argument.")));
        parser.showHelp(1);
    }

    // Check that format is a number between 1 and 4.
    int format = parser.value(formatOption).toInt();
    if (format < 1 || format > 4) {
        fprintf(stderr, "%s\n", qPrintable(QCoreApplication::translate("main", "Error: Invalid format argument. Must be 1, 2, 3, or 4.")));
        parser.showHelp(1);
    }

    // Get address option. TODO: Could add checking here that address is a valid number in the correct range.
    int address = parser.value(addressOption).toInt();

    // Display values of arguments and options.
    qDebug() << "filename:  " << args.at(0);
    qDebug() << "no list:   " << parser.isSet(noListOption);
    qDebug() << "uppercase: " << parser.isSet(uppercaseOption);
    qDebug() << "address:   " << address;
    qDebug() << "format:    " << format;

    // Application main logic can continue here...

    return 0;
}
