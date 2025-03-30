#include "lua.hpp"
#include "sol.hpp"
#include "glm.hpp"

#define SCRIPTS_FILEPATH std::string("Assets/Scripts/")

namespace Alas {

    struct vec2
    {
        float x;
        float y;
    };

    class ScriptingEngine 
    {
    public:
        static sol::state lua;
    public:
        static void Init()
        {
            lua.open_libraries(sol::lib::base, sol::lib::io, sol::lib::math, sol::lib::table);
        }

        static void LoadAndExecuteScript(std::string filename)
        {
            try
            {
                lua.safe_script_file(SCRIPTS_FILEPATH + filename);
                ALAS_CORE_INFO("script was read");
            }
            catch (const sol::error& e)
            {
                ALAS_CORE_ERROR(e.what());
                return;
            }

            // Step 7) Map C++ Types to Lua "Types"
            lua.new_usertype<vec2>("vec2",
                sol::constructors<vec2()>(),
                "x", sol::property(&vec2::x),
                "y", sol::property(&vec2::y));
        }

        static glm::vec2 LuaUpdate()
        {
            vec2 velocity = lua["CPP_Update"]();

            return glm::vec2(velocity.x, velocity.y);
        }

        static void LoadAndExecuteScript()
        {
            try
            {
                lua.safe_script_file(SCRIPTS_FILEPATH + "test.lua");
                ALAS_CORE_INFO("script was read");
            }
            catch (const sol::error& e)
            {
                ALAS_CORE_ERROR(e.what());
                return;
            }

            // Step 2) Read Simple Variables
            std::cout << "[CPP S2] a = " << lua["a"].get_or(0) << "\n";
            std::cout << "[CPP S2] b = " << lua["b"].get_or(0) << "\n";
            std::cout << "[CPP S2] c = " << lua["c"].get_or(0) << "\n";
            std::cout << "[CPP S2] d = " << lua["d"].get_or<std::string>("0") << "\n";

            // Explicit alternative
            auto a_double = lua["a"].get<double>();
            std::cout << "[CPP S2] a_double = " << a_double << "\n";

            // Existence Checking
            sol::optional<double> an_optional_double = lua["a"];
            if (an_optional_double)
                std::cout << "[CPP S2] an_optional_double = " << an_optional_double.value() << "\n";

            // ^ Workaround for
            auto an_optional_double_fix = lua["a"].get<std::optional<double>>();
            if (an_optional_double_fix)
                std::cout << "[CPP S2] an_optional_double_fix = " << an_optional_double_fix.value() << "\n";

            // Step 3) Read Table
            struct Player
            {
                std::string title;
                std::string name;
                std::string family;
                int level;
            } player;

            std::optional<sol::table> a_table = lua["player"];
            if (a_table)
            {
                for (const auto& entry : a_table.value())
                {
                    sol::object key = entry.first;
                    sol::object value = entry.second;

                    std::string sKey = key.as<std::string>();

                    if (sKey == "Name") player.name = value.as<std::string>();
                    if (sKey == "Family") player.family = value.as<std::string>();
                    if (sKey == "Title") player.title = value.as<std::string>();
                    if (sKey == "Level") player.level = value.as<int>();

                }

                std::cout << "[CPP S3] a_table = " << player.title << " " << player.name << " of "
                    << player.family << " [Lvl: " << player.level << "]\n";
            }

            // Or even simpler
            sol::table another_table = lua["player"];
            player.name = another_table["Name"].get<std::string>();
            player.family = another_table["Family"].get<std::string>();
            player.title = another_table["Title"].get<std::string>();
            player.level = another_table["Level"].get<int>();

            std::cout << "[CPP S3] another_table = " << player.title << " " << player.name << " of "
                << player.family << " [Lvl: " << player.level << "]\n";

            // Step 4) Calling a Lua Function
            lua["CalledFromCPP1"](5.0f, 6.0f, "Bwa Ha Ha!");

            // Function might return something
            float x1 = lua["CalledFromCPP1"](5.0f, 6.0f, "Bwa Ha Ha!");
            std::cout << "[CPP S4] 'CalledFromCPP1' returned " << x1 << "\n";


            // Safely call function
            sol::protected_function function_object = lua["CalledFromCPP1"];
            if (function_object)
            {
                auto x2 = function_object(5.0f, 6.0f, "Bwa Ha Ha!");
                if (x2.valid())
                {
                    std::cout << "[CPP S4] 'CalledFromCPP1' returned " << x2.get<float>() << "\n";
                }
                else
                {
                    std::cout << "[CPP S4] 'CalledFromCPP1' FAILED!!!\n";	
                    sol::error e = x2;
                    std::cout << e.what() << "\n";
                }
            }

            // Step 5) Call Lua Function, which calls C++ Function!
            lua["HostFunction"] = [](float a, float b)
                {
                    std::cout << "[CPP S5] HostFunction(" << a << ", " << b << ") called from Lua\n";
                    return a * b;
                };

            float x3 = lua["CalledFromCPP2"](5.0f, 6.0f);

            // Step 6) Inject variables into Lua state machine
            lua["entirely_new_variable"] = 42;

            lua.script("function entirely_new_function(a) "
                        "    return entirely_new_variable * a "
                        "end");

            int x4 = lua["entirely_new_function"](3);
            std::cout << "[CPP S6] x4 = " << x4 << "\n";

            // Step 7) Map C++ Types to Lua "Types"
            lua.new_usertype<Player>("Player",
                sol::constructors<Player()>(),
                "Name", sol::property(&Player::name),
                "Family", sol::property(&Player::family),
                "Title", sol::property(&Player::title),
                "Level", sol::property(&Player::level));


            Player player1 = lua["CreatePlayer1"]();

            std::cout << "[CPP S7] player1 = " << player1.title << " " << player1.name << " of "
                << player1.family << " [Lvl: " << player1.level << "]\n";



            // Step 8) Use C++ Containers in Lua
            std::vector<Player> vecPlayers;
            lua["allPlayers"] = &vecPlayers;

            std::cout << "[CPP S9] vecPlayers.size() = " << vecPlayers.size() << "\n";
            lua["CreatePlayer2"](13);
            std::cout << "[CPP S9] vecPlayers.size() = " << vecPlayers.size() << "\n";

            for (const auto& p : vecPlayers)
                std::cout << "[CPP S9] player1 = " << p.title << " " << p.name << " of "
                << p.family << " [Lvl: " << p.level << "]\n";
        }
    };
}

