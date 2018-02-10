#include "cli.hpp"
#include "game/context/nwcontext.hpp"

void ServerCommandLine::initBuiltinCommands() noexcept
{
    mCommands.registerCommand("help", { "internal","Help" },
        [this](Command cmd)->CommandExecuteStat
    {
        std::string helpString = "\nAvailable commands:\n";
        for (const auto& command : mCommands.getCommandMap())
        {
            helpString += command.first + " - " + command.second.first.author
            + " : " + command.second.first.help + "\n";
        }
        return{ true, helpString };
    });

    mCommands.registerCommand("server.stop", { "internal","Stop the server." },
        [this](Command cmd)->CommandExecuteStat
    {
        context.rpc.getServer().stop();
        return{ true, "" };
    });

    mCommands.registerCommand("conf.get",
        { "internal","Get one configuration item. Usage: conf.get <confname>" },
        [this](Command cmd)->CommandExecuteStat
    {
        if (cmd.args.size() == 1)
        {
            auto keys = split(cmd.args[0], '.');
            Json now = getSettings();
            bool exist = true;
            for (auto key : keys)
            {
                auto iter = now.find(key);
                if (iter == now.end())
                {
                    exist = false;
                    break;
                }
                now = iter.value();
            }

            if (exist)
                return{ true, cmd.args[0] + " = " + now.dump() };
            else
                return{ false, "The configuration item does not exist." };
        }
        else
        {
            return{ false, "Usage: conf.get <confname>" };
        }
    });

    mCommands.registerCommand("conf.show", { "internal","Show the configuration." },
        [this](Command cmd)->CommandExecuteStat
    {
        return{ true, getSettings().dump() };
    });

    mCommands.registerCommand("conf.save", { "internal","Save the configuration." },
        [this](Command cmd)->CommandExecuteStat
    {
        writeJsonToFile(SettingsFilename, getSettings());
        return{ true,"Done!" };
    });

    mCommands.registerCommand("server.ups", { "internal","Show the ups." },
        [this](Command cmd)->CommandExecuteStat
    {
        // TODO: Add UPS counter for server
        return{ true,"[Server UPS counter not finished yet!]" };
    });

    mCommands.registerCommand("chunks.count",
        { "internal","Show how many chunks are loaded" },
        [this](Command cmd)->CommandExecuteStat
    {
        size_t sum = 0;
        for (auto&& world : mWorlds)
        {
            sum += world->getChunkCount();
        }
        return{ true,"Chunks loaded: " + std::to_string(sum) };
    });
}
