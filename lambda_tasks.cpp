#include <memory>
#include <iostream>
#include <deque>
#include <functional>

template <typename Func>
using AmazingQueue = std::deque<Func>;


struct do_nothing
{ 
    template<typename...Args> 
    void operator()(Args&&...)const{}
};

template<typename...cmd>
struct maybe_run_t;

template<> 
struct maybe_run_t<> : do_nothing{};

template<typename cmd> struct maybe_run_t<cmd>
{
  cmd&& value;
  template<typename...Args>
  void operator()(Args&&...args) const
  {
    value(std::forward<Args>(args)...);
  }
};

template<typename...Args>
maybe_run_t<Args...> maybe_run(Args&&...args)
{
  return {std::forward<Args>(args)...};
}

template<typename...Args>
decltype(auto) labda_fabric(int x, double d, 
        std::function<void(std::string, std::string)> fBody)
{
    return [x, d, fBody](std::string s, std::string s2, auto&&..._) 
                mutable->decltype(auto) { 
                    maybe_run(_...)(x, d);
                    std::cout << s << s2 << x << " " << d << std::endl;
                    fBody(s, s2); 
                };
}

int main() {
    int x = 0;
    double d = 0.0;

    AmazingQueue<decltype(labda_fabric(x, d, nullptr))> tasks;

    for (int i = 0; i < 9; ++i)
    {
        auto FuncBody = [](std::string s1, std::string s2)
        {
            std::cout << "firstBoduType" << s1 << " " << s2 << std::endl;
        };
        tasks.push_back(labda_fabric(x, d, FuncBody));
    }

    auto FuncBody = [](std::string s1, std::string s2)
    {
        std::cout << "secondBoduType" << std::endl;
    };
    tasks.push_back(labda_fabric(x, d, FuncBody));
    
    int counter = 0;
    for (auto && q: tasks)
    {
        q("Первый параметр ", "Eщё что-то передали. ", 
            [counter](auto&x, auto&d){ x += counter; d += counter * 2;});
        q("Первый параметр. ", "Eщё что-то передали. ");

        ++counter;
    }
}
