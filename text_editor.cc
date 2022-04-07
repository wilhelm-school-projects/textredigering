#include <algorithm>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <queue>

using str_pair = std::pair<std::string, std::string>;

void handle_arguments(str_pair & args_p, std::vector<std::string> & text);

void do_remove(std::vector<std::string> & text);
void do_substitute(std::vector<std::string> & text);

void do_print(std::vector<std::string> const& text);
void do_table(std::vector<std::string> const& text);
void do_freqtable(std::vector<std::string> const& text);

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Please write: ./a.out <filename> <args>[=]<paremeter>+<value>" << std::endl;
        return 1;
    }
    std::fstream in_file{argv[1], std::ios::in};
    
    std::vector<std::string> arguments{argv + 2, argv + argc};
    std::vector<std::string> text{std::istream_iterator<std::string>(in_file), std::istream_iterator<std::string>()};
                               
    std::vector<str_pair> args_pair_list(arguments.size());
    std::transform(begin(arguments), end(arguments), begin(args_pair_list), [](std::string & line)
                                                                {
                                                                    auto eq_it{find(begin(line), end(line), '=')};
                                                                    std::string flag{begin(line), eq_it};
                                                                    
                                                                    std::string parameter{};
                                                                    if (eq_it != end(line))
                                                                        copy(eq_it + 1, end(line), std::back_inserter(parameter));
                                                                      
                                                                    return str_pair{flag, parameter};
                                                                });

    std::for_each(begin(args_pair_list), end(args_pair_list), [&text](str_pair& args_p)
                                                    {
                                                       handle_arguments(args_p, text);
                                                    });

    return 0;
}

void handle_arguments(str_pair & args_p, std::vector<std::string> & text)
{
    if (args_p.first == "--remove")
    {
        // TODO
    }
    else if (args_p.first == "--print")
    {
        do_print(text);
    }
    else if (args_p.first == "--frequency")
    {
        do_freqtable(text);
    }
    else if (args_p.first == "--table")
    {
        do_table(text);
    }
    else if (args_p.first == "--substitute")
    {
        // TODO
    }
}

void do_print(std::vector<std::string> const& text)
{
    std::copy(begin(text), end(text), std::ostream_iterator<std::string>(std::cout, " "));
    std::cout << std::endl;
}

void do_table(std::vector<std::string> const& text)
{

}

void do_freqtable(std::vector<std::string> const& text)
{

}