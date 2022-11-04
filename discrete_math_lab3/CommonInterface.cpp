#include "CommonInterface.h"
#include <Windows.h>
#include <cassert>
#include <unordered_map>


namespace KHAS {
    void CommonInterface::printHeader()
    {
        push(delimiter('='));
        push(stringGeneration(' ', "����������:", "", "", "", "", "���������� ����������"));
        push(stringGeneration(' ', "����������� ������:", "", "", "", "", "", "� 1"));
        push(delimiter('-'));
        push(stringGeneration(' ', "����:", "", "", "", "", "��������� � �� ��������"));
        push(stringGeneration(' ', "�������� �������:", "", "", "", "", "������� �.�."));
        push(delimiter('='));

        flush();
    }

    void CommonInterface::printMenu()
    {
        push(delimiter('='));
        push(stringGeneration(' ', "�������", "", "", "", "��������"));
        push(delimiter('-'));
        push(stringGeneration(' ', "[ 1 ]", "    ", "", "", "", "", "�������� ����"));
        push(stringGeneration(' ', "[ 2 ]", "    ", "", "", "", "", "������� ����"));
        push(stringGeneration(' ', "[ 5 ] ", "", "", "", "", "", "������ ����� ���������"));
        push(stringGeneration(' ', "[ 0 ]", "            ", "", "", "", "", "�����"));
        push(delimiter('='));

        flush();
    }

    Commands CommonInterface::selectCommand()
    {
        int tmp_cmd{};
        while (!(std::cin >> tmp_cmd)) {
            push(delimiter('-'));
            push(stringGeneration(' ', "������ �����! ��������� ����!"));
            push(delimiter('-'));
            flush();
            std::cin.clear();
            auto mx = std::numeric_limits<std::streamsize>::max;
            std::cin.ignore(mx(), '\n');
        }
        static std::unordered_map<int, Commands> base{
            { 1, Commands::AddingAPair }
            , { 2, Commands::DeletingAPair }
            , { 5, Commands::SetNewSET }
            , { 0, Commands::Exit }
        };
        if (base.find(tmp_cmd) == base.end()) return Commands::Unknown;

        return base[tmp_cmd];
    }

    void CommonInterface::runCommands(Commands cmd)
    {
        switch (cmd) {
        case Commands::AddingAPair:

            break;
        case Commands::DeletingAPair:

            break;
        case Commands::SetNewSET:

            break;
        default:
            break;
        }
    }

    void CommonInterface::printUnknown()
    {
        push(delimiter('-'));
        push(stringGeneration(' ', "������ �����! ��������� ����!"));
        push(delimiter('-'));
        flush();
    }

    void CommonInterface::flush()
    {
        cout_ << out_buffer_.str();
        clearOutBuffer();
    }

    void CommonInterface::clearOutBuffer()
    {
        out_buffer_.str("");
    }

    std::string CommonInterface::delimiter(char del) const
    {
        assert(table_width_ >= min_table_width_);
        std::string tmp(table_width_, del);
        tmp.front() = tmp.back() = '#';
        return tmp;
    }

    CommonInterface::CommonInterface(std::size_t table_width, std::ostream& out)
        : cout_(out)
        , out_buffer_()
        , table_width_(table_width)
    {
        assert(table_width >= min_table_width_);
    }
}