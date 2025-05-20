#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <algorithm> // Для std::find

class Group;

class User {
private:
    int userId;
    std::string username;
    std::string additionalInfo;
    Group* group;

public:
    User(int id, const std::string& name, const std::string& info = "") : userId(id), username(name), additionalInfo(info), group(nullptr) {}

    // Getters
    int getUserId() const { return userId; }
    std::string getUsername() const { return username; }
    std::string getAdditionalInfo() const { return additionalInfo; }
    Group* getGroup() const { return group; }

    // Setters
    void setGroup(Group* g) { group = g; }
    void removeGroup() { group = nullptr; }

    // Вывод информации о пользователе
    void printInfo() const;
};

class Group {
private:
    int groupId;
    std::vector<User*> users; // Список указателей на пользователей

public:
    // Конструктор
    Group(int id) : groupId(id) {}

    // Getters
    int getGroupId() const { return groupId; }
    const std::vector<User*>& getUsers() const { return users; } // Возвращаем константную ссылку для безопасности

    // Методы для управления пользователями в группе
    void addUser(User* user) {
        if (user == nullptr) {
            throw std::invalid_argument("User pointer cannot be null.");
        }

        // Проверка, чтобы пользователь не был добавлен дважды
        if (std::find(users.begin(), users.end(), user) == users.end()) {
            users.push_back(user);
            user->setGroup(this);
        }
    }

    void removeUser(User* user) {
        if (user == nullptr) {
            throw std::invalid_argument("User pointer cannot be null.");
        }

        auto it = std::find(users.begin(), users.end(), user);
        if (it != users.end()) {
            users.erase(it);
            user->removeGroup();
        }
    }

    // Вывод информации о группе
    void printInfo() const {
        std::cout << "Group ID: " << groupId << '\n';
        std::cout << "Users:\n";

        for (User* user : users) {
            if (user) { // Проверка на случай, если указатель стал недействительным
                user->printInfo();
                std::cout << "---\n";
            }
        }
    }
};

void User::printInfo() const {
    std::cout << "User ID: " << userId << '\n';
    std::cout << "Username: " << username << '\n';
    std::cout << "Additional Info: " << additionalInfo << '\n';

    if (group) {
        std::cout << "Group ID: " << group->getGroupId() << '\n';
    } else {
        std::cout << "Not in a group\n";
    }
}

// Класс для управления пользователями и группами
class UserManager {
private:
    std::unordered_map<int, User*> users;
    std::unordered_map<int, Group*> groups;

public:
    ~UserManager() {
        // Освобождаем память, выделенную под пользователей и группы
        for (auto it = users.begin(); it != users.end(); ++it) {
            delete it->second;
        }

        for (auto it = groups.begin(); it != groups.end(); ++it) {
            delete it->second;
        }
    }

    // Обработка команды создания пользователя
    void createUser(int userId, const std::string& username, const std::string& additionalInfo) {
        if (users.count(userId)) {
            throw std::runtime_error("User with this ID already exists.");
        }

        User* newUser = new User(userId, username, additionalInfo);
        users[userId] = newUser;
        std::cout << "User created successfully.\n";
    }

    // Обработка команды удаления пользователя
    void deleteUser(int userId) {
        if (!users.count(userId)) {
            throw std::runtime_error("User not found.");
        }

        User* userToDelete = users[userId];

        // Удаляем пользователя из всех групп, в которых он состоит
        for (auto it = groups.begin(); it != groups.end(); ++it) {
            it->second->removeUser(userToDelete);
        }

        delete userToDelete;
        users.erase(userId);
        std::cout << "User deleted successfully.\n";
    }

    // Обработка команды вывода информации по всем пользователям
    void allUsers() const {
        if (users.empty()) {
            std::cout << "No users found.\n";
            return;
        }

        for (auto it = users.begin(); it != users.end(); ++it) {
            it->second->printInfo();
            std::cout << "---\n";
        }
    }

    // Обработка команды вывода информации по одному пользователю
    void getUser(int userId) const {
        if (!users.count(userId)) {
            throw std::runtime_error("User not found.");
        }

        users.at(userId)->printInfo();
    }

    // Обработка команды создания группы
    void createGroup(int groupId) {
        if (groups.count(groupId)) {
            throw std::runtime_error("Group with this ID already exists.");
        }

        Group* newGroup = new Group(groupId);
        groups[groupId] = newGroup;
        std::cout << "Group created successfully.\n";
    }

    // Обработка команды удаления группы
    void deleteGroup(int groupId) {
        if (!groups.count(groupId)) {
            throw std::runtime_error("Group not found.");
        }

        Group* groupToDelete = groups[groupId];

        // Удаляем всех пользователей из группы
        for (User* user : groupToDelete->getUsers()) {
            if (user) {
                user->removeGroup();
            }
        }

        delete groupToDelete;
        groups.erase(groupId);
        std::cout << "Group deleted successfully.\n";
    }

    // Обработка команды вывода информации по всем группам
    void allGroups() const {
        if (groups.empty()) {
            std::cout << "No groups found.\n";
            return;
        }

        for (auto it = groups.begin(); it != groups.end(); ++it) {
            it->second->printInfo();
            std::cout << "===\n";
        }
    }

    // Обработка команды вывода информации по одной группе
    void getGroup(int groupId) const {
        if (!groups.count(groupId)) {
            throw std::runtime_error("Group not found.");
        }

        groups.at(groupId)->printInfo();
    }

    // Вспомогательный метод для добавления пользователя в группу
    void addUserToGroup(int userId, int groupId) {
        if (!users.count(userId)) {
            throw std::runtime_error("User not found.");
        }

        if (!groups.count(groupId)) {
            throw std::runtime_error("Group not found.");
        }

        User* user = users[userId];
        Group* group = groups[groupId];
        group->addUser(user);
        std::cout << "User added to group successfully.\n";
    }

    // Вспомогательный метод для удаления пользователя из группы
    void removeUserFromGroup(int userId, int groupId) {
        if (!users.count(userId)) {
            throw std::runtime_error("User not found.");
        }

        if (!groups.count(groupId)) {
            throw std::runtime_error("Group not found.");
        }

        User* user = users[userId];
        Group* group = groups[groupId];
        group->removeUser(user);
        std::cout << "User removed from group successfully.\n";
    }
};

// Функция для разделения строки на токены
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

int main() {
    UserManager userManager;
    std::string command;

    std::cout << "Start program\n";
    std::cout << "Possible commands:\n";
    std::cout << " 1. createUser {userId} {username} {…additional info…}\n";
    std::cout << " 2. deleteUser {userId}\n";
    std::cout << " 3. allUsers\n";
    std::cout << " 4. getUser {userId}\n";
    std::cout << " 5. createGroup {groupId}\n";
    std::cout << " 6. deleteGroup {groupId}\n";
    std::cout << " 7. allGroups\n";
    std::cout << " 8. getGroup {groupId}\n";
    std::cout << " 9. addUserToGroup {userId} {groupId}\n";
    std::cout << "10. removeUserFromGroup {userId} {groupId}\n";
    std::cout << "11. exit (or quit)\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);

        try {
            std::vector<std::string> parts = split(command, ' ');
            std::string cmd = parts[0];

            if (cmd == "createUser") {
                if (parts.size() < 3) {
                    std::cerr << "Usage: createUser {userId} {username} {…additional info…}\n";
                    continue;
                }

                int userId = std::stoi(parts[1]);
                std::string username = parts[2];
                std::string additionalInfo;
                if (parts.size() > 3) {
                    for (size_t i = 3; i < parts.size(); ++i) {
                        additionalInfo += parts[i] + " ";
                    }

                    additionalInfo.pop_back();
                }

                userManager.createUser(userId, username, additionalInfo);
            } else if (cmd == "deleteUser") {
                if (parts.size() != 2) {
                    std::cerr << "Usage: deleteUser {userId}\n";
                    continue;
                }

                int userId = std::stoi(parts[1]);
                userManager.deleteUser(userId);
            } else if (cmd == "allUsers") {
                userManager.allUsers();
            } else if (cmd == "getUser") {
                if (parts.size() != 2) {
                    std::cerr << "Usage: getUser {userId}\n";
                    continue;
                }

                int userId = std::stoi(parts[1]);
                userManager.getUser(userId);
            } else if (cmd == "createGroup") {
                if (parts.size() != 2) {
                    std::cerr << "Usage: createGroup {groupId}\n";
                    continue;
                }

                int groupId = std::stoi(parts[1]);
                userManager.createGroup(groupId);
            } else if (cmd == "deleteGroup") {
                if (parts.size() != 2) {
                    std::cerr << "Usage: deleteGroup {groupId}\n";
                    continue;
                }

                int groupId = std::stoi(parts[1]);
                userManager.deleteGroup(groupId);
            } else if (cmd == "allGroups") {
                userManager.allGroups();
            } else if (cmd == "getGroup") {
                if (parts.size() != 2) {
                    std::cerr << "Usage: getGroup {groupId}\n";
                    continue;
                }

                int groupId = std::stoi(parts[1]);
                userManager.getGroup(groupId);
            } else if (cmd == "addUserToGroup") {
                if (parts.size() != 3) {
                    std::cerr << "Usage: addUserToGroup {userId} {groupId}\n";
                    continue;
                }

                int userId = std::stoi(parts[1]);
                int groupId = std::stoi(parts[2]);
                userManager.addUserToGroup(userId, groupId);
            } else if (cmd == "removeUserFromGroup") {
                if (parts.size() != 3) {
                    std::cerr << "Usage: removeUserFromGroup {userId} {groupId}\n";
                    continue;
                }

                int userId = std::stoi(parts[1]);
                int groupId = std::stoi(parts[2]);
                userManager.removeUserFromGroup(userId, groupId);
            } else if (cmd == "exit" || cmd == "quit") {
                break;
            } else {
                std::cout << "Unknown command." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << '\n';
        }
    }

    return 0;
}