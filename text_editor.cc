#include <algorithm>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <queue>

template <typename T>
class Queue_Adapter             
{
public:
    using value_type = T;

    Queue_Adapter(std::queue<T> & q)
        : q{q}
    {}
    void push_back(T const& element)
    {
        q.push(element);
    }
private:
    std::queue<T>& q;
};

void insert_args_to_queue(Queue_Adapter<std::pair<std::string, std::string>> & Q, std::vector<std::string> & arguments, std::vector<std::string> & text);
void execute_args(std::queue<std::pair<std::string, std::string>> & args_queue, std::vector<std::string> & text);


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Please write: ./a.out <filename> <args>" << std::endl;
        return 1;
    }
    std::fstream in_file{argv[1], std::ios::in};
    
    std::vector<std::string> arguments{argv + 2, argv + argc}; // Varför inte begin(argv), end(argv)?
    std::vector<std::string> text{std::istream_iterator<std::string>(in_file), std::istream_iterator<std::string>()};
    
    std::queue<std::pair<std::string, std::string>> args_queue{}; 
    Queue_Adapter<std::pair<std::string, std::string>> Q{args_queue};       // Used to make push_back available for std::back_inserter

    // Effectively fills args_queue with argument-parameter pairs
    insert_args_to_queue(Q, arguments, text);                               
    
    execute_args(args_queue, text);

    return 0;
}

void insert_args_to_queue(Queue_Adapter<std::pair<std::string, std::string>> & Q, std::vector<std::string> & arguments, std::vector<std::string> & text)
{
    std::transform(begin(arguments), end(arguments), std::back_inserter(Q), [](std::string & line)
                                                                {
                                                                    auto eq_it{find(begin(line), end(line), '=')};
                                                                    std::string flag{begin(line), eq_it};
                                                                    
                                                                    std::string parameter{};
                                                                    if (eq_it != end(line))
                                                                        copy(eq_it + 1, end(line), std::back_inserter(parameter));
                                                                      
                                                                    return std::pair<std::string, std::string>{flag, parameter};
                                                                });
}

void execute_args(std::queue<std::pair<std::string, std::string>> & args_queue, std::vector<std::string> & text)
{
    while (!args_queue.empty())
    {
        std::cout << args_queue.front().first << " " << args_queue.front().second << std::endl;
        args_queue.pop();
    }
}


    // std::copy(begin(text), end(text), std::ostream_iterator<std::string>(std::cout, " "));
    // for(string const& e : arguments)
    // {
        // cout << e << endl;
    // }