#include "command.hpp"

class ServerCommandLine {
public:
    ServerCommandLine() {
        initBuiltinCommands();
    }
    void start();

private:
    void initBuiltinCommands() noexcept;
    CommandManager mCommands;
};
