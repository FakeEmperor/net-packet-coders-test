#include <iostream>
#include <cmd/cmd.h>
int main(int argc, char** argv)
{
    npct::cmd::CommandLine cmd;
    try
    {
        cmd.start(const_cast<const char**>(argv+1), (size_t) (argc - 1));
    } catch (std::runtime_error &e) {
        std::cerr << "[ERROR]! " << e.what();
    }


}