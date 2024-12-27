#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h> // Для chmod
#include <unistd.h>   // Для getuid

using namespace std;

class User {
public:
    string _name;
    string _login;
    string _pass;

    User() = default;
    User(const string& name, const string& login, const string& pass)
        : _name(name), _login(login), _pass(pass) {}

    void serialize(ofstream& out) const {
        out << _name << '\n' << _login << '\n' << _pass << '\n';
    }

    static User deserialize(ifstream& in) {
        User user;
        getline(in, user._name);
        getline(in, user._login);
        getline(in, user._pass);
        return user;
    }
};

class Message {
public:
    string _text;
    string _sender;
    string _receiver;

    Message() = default;
    Message(const string& text, const string& sender, const string& receiver)
        : _text(text), _sender(sender), _receiver(receiver) {}

    void serialize(ofstream& out) const {
        out << _text << '\n' << _sender << '\n' << _receiver << '\n';
    }

    static Message deserialize(ifstream& in) {
        Message message;
        getline(in, message._text);
        getline(in, message._sender);
        getline(in, message._receiver);
        return message;
    }
};

void setFilePermissions(const string& filename) {
    if (chmod(filename.c_str(), S_IRUSR | S_IWUSR) == -1) {
        perror("chmod");
        exit(EXIT_FAILURE);
    }
}

void saveUsers(const vector<User>& users, const string& filename) {
    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "Failed to open file for writing: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    for (const auto& user : users) {
        user.serialize(out);
    }

    out.close();
    setFilePermissions(filename);
}

vector<User> loadUsers(const string& filename) {
    vector<User> users;
    ifstream in(filename);
    if (!in.is_open()) {
        cerr << "Failed to open file for reading: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    while (in.peek() != EOF) {
        users.push_back(User::deserialize(in));
    }

    in.close();
    return users;
}

void saveMessages(const vector<Message>& messages, const string& filename) {
    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "Failed to open file for writing: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    for (const auto& message : messages) {
        message.serialize(out);
    }

    out.close();
    setFilePermissions(filename);
}

vector<Message> loadMessages(const string& filename) {
    vector<Message> messages;
    ifstream in(filename);
    if (!in.is_open()) {
        cerr << "Failed to open file for reading: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    while (in.peek() != EOF) {
        messages.push_back(Message::deserialize(in));
    }

    in.close();
    return messages;
}

int main() {
    const string usersFile = "users.dat";
    const string messagesFile = "messages.dat";

    vector<User> users = {
        User("Alice", "alice123", "password1"),
        User("Bob", "bob456", "password2")
    };

    vector<Message> messages = {
        Message("Hello, Bob!", "Alice", "Bob"),
        Message("Hi, Alice!", "Bob", "Alice")
    };

    // Сохранение пользователей и сообщений
    saveUsers(users, usersFile);
    saveMessages(messages, messagesFile);

    // Загрузка пользователей и сообщений
    vector<User> loadedUsers = loadUsers(usersFile);
    vector<Message> loadedMessages = loadMessages(messagesFile);

    // Вывод загруженных данных
    cout << "Loaded Users:" << endl;
    for (const auto& user : loadedUsers) {
        cout << "Name: " << user._name << ", Login: " << user._login << ", Password: " << user._pass << endl;
    }

    cout << "\nLoaded Messages:" << endl;
    for (const auto& message : loadedMessages) {
        cout << "Text: " << message._text << ", Sender: " << message._sender << ", Receiver: " << message._receiver << endl;
    }

    return 0;
}
