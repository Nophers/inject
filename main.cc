#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <regex>

#include "utils.hpp"

std::string readLine()
{
    std::string line;
    std::getline(std::cin, line);
    return line;
}

void clearTerminal()
{
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void setTextColor(int color)
{
    if (color < 0 || color > 255)
    {
        std::cerr << "Invalid text color: " << color << std::endl;
        return;
    }

#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hconsole, color);
#else
    std::cout << "\033[38;5;" << color << "m";
#endif
}

template <typename T>
int navigateList(const std::vector<T> &items, bool canCancel = true)
{
    int selected = 0;
    while (true)
    {
        clearTerminal();
        std::cout << "Please select an option:" << std::endl;
        for (int i = 0; i < items.size(); i++)
        {
            std::cout << (i == selected ? "> " : "  ") << items[i] << std::endl;
        }
        if (canCancel)
        {
            std::cout << "  (Press 'q' to cancel)" << std::endl;
        }

        int c = std::getchar();
        switch (c)
        {
        case 'q':
            return -1;
        case '\033':
            c = std::getchar();
            if (c == '[')
            {
                c = std::getchar();
                if (c == 'A')
                {
                    selected = std::max(0, selected - 1);
                }
                else if (c == 'B')
                {
                    selected = std::min((int)items.size() - 1, selected + 1);
                }
            }
            break;
        case '\n':
        case '\r':
            return selected;
        default:
            break;
        }
    }
}

void install_deps()
{
    std::string command = "npm install";
    system(command.c_str());
}

int main()
{
    std::cout << "Enter project name: ";
    std::string projectName = readLine();
    std::regex projectNameRegex("^[a-zA-Z]{3,16}$");

    if (!std::regex_match(projectName, projectNameRegex))
    {
        setTextColor(1); // red
        std::cout << "Project name invalid, make sure to enter at least 4 characters and less than 16, only Alphabets allowed" << std::endl;
        return 1;
    }

    std::vector<std::string> templates;
    std::string templateDirectory = "template";
    for (const auto &entry : std::filesystem::directory_iterator(templateDirectory))
    {
        templates.push_back(entry.path().filename().string());
    }

    std::cout << "Select a project template:" << std::endl;
    for (int i = 0; i < templates.size(); i++)
    {
        std::cout << i + 1 << ") " << templates[i] << std::endl;
    }
    std::cout << "Enter a number: ";

    int templateIndex = -1;
    while (templateIndex == -1)
    {
        int c = getchar();
        switch (c)
        {
        case '\n':
        case '\r':
            continue;
        case 27: // escape character
            c = getchar();
            if (c == 91)
            {
                c = getchar();
                templateIndex = (c == 65) ? navigateList(templates, -1) : ((c == 66) ? navigateList(templates, 1) : -1);
            }
            break;
        default:
            if (isdigit(c))
            {
                std::cin.putback(c);
                std::cin >> templateIndex;
                templateIndex--;
                if (templateIndex < 0 || templateIndex >= templates.size())
                {
                    std::cout << "Invalid selection, please enter a valid number" << std::endl;
                    templateIndex = -1;
                }
                else
                {
                    std::cout << "Invalid format, please enter a valid number";
                }
            }
            break;
        }
    }

    std::string templatePath = templateDirectory + "/" + templates[templateIndex];

    std::vector<std::string> dependencies = {"npm"};
    int dependencyIndex = navigateList(dependencies);
    if (dependencyIndex == -1)
    {
        return 0;
    }
    std::string dependencyManager = dependencies[dependencyIndex];

    std::filesystem::create_directory(projectName);
    for (const auto &entry : std::filesystem::directory_iterator(templatePath))
    {
        std::filesystem::copy(entry, projectName / entry.path().filename());
    }

    clearTerminal();
    setTextColor(2); // green
    std::cout << "Generated project: " + projectName
              << std::endl;
    for (const auto &entry : std::filesystem::directory_iterator(projectName))
    {
        std::cout << "  " << entry.path().filename() << std::endl;
    }

    setTextColor(7);
    std::cout << "\nDo you want to install dependencies? (y/n) ";
    std::string installDependencies;
    std::cin >> installDependencies;

    if (installDependencies == "y")
    {
        std::filesystem::current_path(projectName);
        install_deps();

        std::cout
            << "Installed dependencies:\n"
            << std::endl;
    }
    else
    {
        std::cout << "Happy hacking!" << std::endl;
    }

    return 0;
}
