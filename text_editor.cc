#include <algorithm>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <iomanip>
#include <map>
#include <sstream>

using str_pair = std::pair<std::string, std::string>;

void handle_arguments(str_pair & args_p, std::vector<std::string> & text);

void do_remove(std::vector<std::string> & text, std::string const& parameter);
void do_substitute(std::vector<std::string> & text, std::string const& parameter);

void do_print(std::vector<std::string> const& text);
void do_table(std::vector<std::string> const& text);
void do_freqtable(std::vector<std::string> const& text);

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "Please write: ./a.out <filename> --<args>[=]<paremeter>+<value>" << std::endl;
        return 1;
    }
    std::fstream in_file{argv[1], std::ios::in};
    
    std::vector<std::string> arguments{argv + 2, argv + argc};
    std::vector<std::string> text{std::istream_iterator<std::string>(in_file), std::istream_iterator<std::string>()};
                               
    std::vector<str_pair> args_pair_list(arguments.size());
    std::transform(begin(arguments), end(arguments), begin(args_pair_list), 
                                                    [](std::string & line)
                                                    {
                                                        auto eq_it{std::find(begin(line), end(line), '=')};
                                                        std::string flag{begin(line), eq_it};
                                                        
                                                        std::string parameter{};
                                                        if (eq_it != end(line))
                                                            std::copy(eq_it + 1, end(line), std::back_inserter(parameter));
                                                            
                                                        return str_pair{flag, parameter};
                                                    });

    try
    {
        std::for_each(begin(args_pair_list), end(args_pair_list), 
                                            [&text](str_pair& args_p)
                                            {
                                                handle_arguments(args_p, text);
                                            });        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}


void handle_arguments(str_pair & args_p, std::vector<std::string> & text)
{
    if (args_p.first == "--remove")
    {
        do_remove(text, args_p.second);
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
        do_substitute(text, args_p.second);
    }
    else
    {
        throw std::logic_error{"------------------------------\nWrong argument. Arguments are:\n------------------------------\nremove\nprint\nfrequency\ntable\nsubstitute\n------------------------------"};
    }
}
void do_substitute(std::vector<std::string> & text, std::string const& parameter)   
{
    long unsigned int pos{parameter.find('+')};
    if (pos == std::string::npos)
    {
        throw std::logic_error{"Wrong format, please write: ./a.out --substitute=<word>+<new_word>"};
    }
    std::string old_word{parameter.substr(0, pos)};
    std::string new_word{parameter.substr(pos + 1, parameter.size())};
    std::replace(begin(text), end(text), old_word, new_word);
}

void do_remove(std::vector<std::string> & text, std::string const& parameter)
{
    if (parameter.empty())
    {
        throw std::logic_error{"./a.out --remove=<word>"};
    }
    text.erase(std::remove(begin(text), end(text), parameter));
}

void do_print(std::vector<std::string> const& text)
{
    std::copy(begin(text), end(text), std::ostream_iterator<std::string>(std::cout, " "));
    std::cout << std::endl;
}

void do_table(std::vector<std::string> const& text)
{
    std::map<std::string, int> word_freq{};
    std::for_each(begin(text), end(text), [&word_freq](std::string const& word)
                                            {
                                                ++word_freq[word];
                                            });

    std::size_t max_width {std::max_element(begin(text), end(text), 
                                            [](std::string const& lhs, std::string const& rhs)
                                            {
                                                return lhs.size() < rhs.size();
                                            })->size()};
    
    std::transform(begin(word_freq), end(word_freq), std::ostream_iterator<std::string>(std::cout, "\n"),
                                                    [max_width](std::pair<std::string, int> const& p)
                                                    {
                                                        std::stringstream line{};
                                                        line << std::left << std::setw(max_width) << p.first << " " << p.second;
                                                        return line.str();
                                                    });                                            
}

void do_freqtable(std::vector<std::string> const& text)
{
    std::map<std::string, int> word_freq{};
    std::for_each(begin(text), end(text), 
                                [&word_freq](std::string const& word)
                                {
                                    ++word_freq[word];
                                });

    std::vector<std::string> unique_words{};
    std::transform(begin(word_freq), end(word_freq), std::back_inserter(unique_words),
                                                    [](std::pair<std::string, int> const& p)
                                                    {
                                                        return p.first;
                                                    });

    std::sort(begin(unique_words), end(unique_words), 
                                    [&word_freq](std::string const& first, std::string const& second)
                                    {
                                        return word_freq[first] > word_freq[second];
                                    });

    std::size_t max_width {std::max_element(begin(text), end(text), 
                                            [](std::string const& lhs, std::string const& rhs)
                                            {
                                                return lhs.size() < rhs.size();
                                            })->size()};

    std::transform(begin(unique_words), end(unique_words), std::ostream_iterator<std::string>(std::cout, "\n"), 
                                                        [&word_freq, max_width](std::string const& s)
                                                        {
                                                            std::stringstream line{};
                                                            line << std::setw(max_width) << s << " " << std::to_string(word_freq[s]);
                                                            return line.str();
                                                        });
}
