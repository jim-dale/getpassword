#pragma once

class CommandLineManager
{
private:
    enum class ParserState
    {
        ExpectOptionOrGetName
        ,ExpectAddEntryName
        ,ExpectAddEntryPassword
        ,ExpectListEntriesFilter
        ,ExpectPrefix
        ,ExpectOutputFileName
    };

public:
    CommandLineManager()
    {
    }
    ~CommandLineManager()
    {
    }

    AppContext ParseCommandLine(int argc, PWSTR argv[])
    {
        AppContext result{};

        ParserState state = ParserState::ExpectOptionOrGetName;
        std::wstring value;

        result.m_showHelp = (argc == 1);

        for (int i = 1; i < argc; i++)
        {
            PCWSTR arg = argv[i];
            size_t length = wcslen(arg);

            switch (state)
            {
            case ParserState::ExpectOptionOrGetName:
                if (length > 1 && (arg[0] == '-' || arg[0] == '/'))
                {
                    state = ProcessOption(result, &arg[1]);
                }
                else
                {
                    result.m_commands.push_back(AppCommand::CreateGetPasswordCommand(arg));
                }
                break;
            case ParserState::ExpectAddEntryName:
                value = arg;
                state = ParserState::ExpectAddEntryPassword;
                break;
            case ParserState::ExpectAddEntryPassword:
                result.m_commands.push_back(AppCommand::CreateAddEntryCommand(value, arg));
                state = ParserState::ExpectOptionOrGetName;
                break;
            case ParserState::ExpectListEntriesFilter:
                result.m_commands.push_back(AppCommand::CreateListCommand(arg));
                state = ParserState::ExpectOptionOrGetName;
                break;
            case ParserState::ExpectPrefix:
                result.m_prefix = arg;
                state = ParserState::ExpectOptionOrGetName;
                break;
            case ParserState::ExpectOutputFileName:
                result.m_outputFileName = arg;
                state = ParserState::ExpectOptionOrGetName;
                break;
            default:
                break;
            }
        }
        return result;
    }

    void ShowHelp()
    {
        ShowVersion();

        wprintf(L"Displays the password for the given credential name stored in the Windows Credential Manager.\n\n");

        wprintf(L"Usage: %s [-?] [-v] [-c] [-l filter] [-p prefix] [-o output] [-s Name Password] [Name]\n\n", ProgramName);

        wprintf(L"  -?  Display this help information.\n");
        wprintf(L"  -v  Display version information.\n");
        wprintf(L"  -c  Display the runtime configuration.\n");
        wprintf(L"  -l  List entries with the specified filter. The filter is appended to the prefix to match entries.\n");
        wprintf(L"  -o  Set output filename.\n");
        wprintf(L"  -p  Sets the prefix for the password entry. The prefix is combined with the name to produce a unique name.\n");
        wprintf(L"      The prefix can also be specified in the 'CM_NAME_PREFIX' environment variable.\n");
        wprintf(L"  -s  Sets the password for the given credential name. Overwrites the existing password if the name already exists.\n\n");

        wprintf(L"Specify [Name] to display the password for the named generic credential.\n");
    }

    void ShowVersion()
    {
        wprintf(L"%s %s-%s\n\n", ProgramName, ProgramVersion, ProgramConfig);
    }

    void ShowIfError(const AppContext& ctx)
    {
        if (false == ctx.m_errorMessage.empty())
        {
            fwprintf(stderr, L"%s\n\n", ctx.m_errorMessage.c_str());
        }
    }

    void ShowConfiguration(const AppContext& ctx)
    {
        wprintf(L"ShowHelp: %s\n", Utility::to_wstr(ctx.m_showHelp));
        wprintf(L"ShowVersion: %s\n", Utility::to_wstr(ctx.m_showVersion));
        wprintf(L"ShowConfiguration: %s\n", Utility::to_wstr(ctx.m_showConfiguration));
        wprintf(L"Prefix: \"%s\"\n", ctx.m_prefix.c_str());
        wprintf(L"OutputFileName: \"%s\"\n", ctx.m_outputFileName.c_str());
        wprintf(L"Commands:\n");
        for each (AppCommand command in ctx.m_commands)
        {
            wprintf(L"    CommandType: \"%s\"\n", Utility::to_wstr(command.m_commandType));
            wprintf(L"    Name: \"%s\"\n", command.m_name.c_str());
            wprintf(L"    Password: \"%s\"\n", command.m_password.c_str());
        }
        wprintf(L"\n");
    }

private:
    ParserState ProcessOption(AppContext& appContext, PCWSTR arg)
    {
        ParserState result = ParserState::ExpectOptionOrGetName;

        if (_wcsicmp(L"?", arg) == 0)
        {
            appContext.m_showHelp = true;
        }
        else if (_wcsicmp(L"v", arg) == 0)
        {
            appContext.m_showVersion = true;
        }
        else if (_wcsicmp(L"c", arg) == 0)
        {
            appContext.m_showConfiguration = true;
        }
        else if (_wcsicmp(L"l", arg) == 0)
        {
            result = ParserState::ExpectListEntriesFilter;
        }
        else if (_wcsicmp(L"o", arg) == 0)
        {
            result = ParserState::ExpectOutputFileName;
        }
        else if (_wcsicmp(L"p", arg) == 0)
        {
            result = ParserState::ExpectPrefix;
        }
        else if (_wcsicmp(L"s", arg) == 0)
        {
            result = ParserState::ExpectAddEntryName;
        }
        else
        {
            appContext.m_showHelp = true;
            appContext.m_errorMessage = std::wstring(L"Parameter format not correct - '") + std::wstring(arg) + std::wstring(L"'.");
        }
        return result;
    }
};
