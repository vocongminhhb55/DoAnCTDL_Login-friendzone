#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <random>
#include <string>
#include <sstream>
#include "sha256.h"

using namespace std;

#if defined(_WIN32)
#define clearscreen "cls"
#elif defined(__MACH__)
#define clearscreen "clear"
#endif

#define tableSize 255

struct user
{
    string name;
    string salt;
    string hashedPass;
    string *recoveryQuestion = new string[3];
    string *hashedrecoveryAns = new string[3];
    
    
    user(string n = "", string s = "", string hp = "",string *recovQ = NULL, string *recovA = NULL) : name(n), salt(s), hashedPass(hp)
    {
        if (recovQ != NULL)
            recoveryQuestion = recovQ;
        if (recovA != NULL)
            hashedrecoveryAns = recovA;
    }
    
    void newUser()
    {
        string pass;
        string ans;
        cout << "Ghi ten ban vao ne: ";
        getline(cin, name);
        salt = random_string();
        cout << "pass nua UwU ";
        getline(cin, pass);
        hashedPass = sha256(pass + salt);
        for (int i = 0; i < 3; i++) {
            cout << "Recovery question " << i + 1 << ": ";
            getline(cin, recoveryQuestion[i]);
            cout << "Recovery answer " << i + 1 << ": ";
            getline(cin, ans);
            hashedrecoveryAns[i] = sha256(ans + salt);
        }
    }
    
    string random_string() {
        string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

        random_device seed;
        default_random_engine generator(seed());

        shuffle(str.begin(), str.end(), generator);

        return str.substr(0, 7);    // assumes 7 < number of characters in str
    }
};

class Hash
{
    int BUCKET;
    
    list<user> *table;
public:
    Hash(int V); // Constructor

    // inserts a key into hash table
    bool insertItem(user key);

    // deletes a key from hash table
    bool deleteItem(user key);
    
    // authentication a key
    bool authItem(string username);
    
    // hash function to map values to key
    unsigned short hashFunction(string str)
    {
        //djb2 hash
        unsigned short hash = 5381;
        int c;

        short strLength = str.length();
        for (short i = 0; i < strLength; i++)
        {
            c = str[i];
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        }
        return hash % tableSize;
    }

    void displayHash();
    
    void saveFile(string name);

    bool loadFile(string fileName);
};

Hash::Hash(int b)
{
    this->BUCKET = b;
    table = new list<user>[BUCKET];
}

bool Hash::insertItem(user key)
{
    int index = hashFunction(key.name);
    for (auto x : table[index])
        if (x.name == key.name)
            return false;
    table[index].push_back(key);
    return true;
}

bool Hash::deleteItem(user key)
{
    // get the hash index of key
    int index = hashFunction(key.name);

    // find the key in (index)th list
    list <user> :: iterator i;
    for (i = table[index].begin(); i != table[index].end(); i++) {
        if (i->name == key.name)
            break;
    }

    // if key is found in hash table, remove it
    if (i != table[index].end())
        {
            table[index].erase(i);
            return true;
        }
    return false;
}

bool Hash::authItem(string username)
{
    string hashedPass, pass;
    int index = hashFunction(username);
    for (auto x : table[index])
        if (x.name == username)
        {
            cout << "Nhap password: ";
            getline(cin, pass);
            if (x.hashedPass == sha256(pass + x.salt)) {
                return true;
            }
        }
    return false;
}

    // function to display hash table
void Hash::displayHash()
{
    for (int i = 0; i < BUCKET; i++)
    {
        cout << i;
        for (auto x : table[i])
            cout << " --> " << x.name;
        cout << endl;
    }
}


void Hash::saveFile(string name)
{
    ofstream ofs(name);
    
    for (int i = 0; i < tableSize; i++) {
        ofs << i;
        for (auto x : table[i])
            ofs << " --> " << x.name << ',' << x.salt << ',' << x.hashedPass << ',' << x.recoveryQuestion[0] << ',' << x.hashedrecoveryAns[0] << ',' << x.recoveryQuestion[1] << ',' << x.hashedrecoveryAns[1] << ',' << x.recoveryQuestion[2] << ',' << x.hashedrecoveryAns[2];
        ofs << endl;
    }
    
    ofs.close();
}

bool Hash::loadFile(string fileName)
{
    ifstream ifs(fileName);
    string name;
    string salt;
    string hashedPass;
    string *recoveryQuestion = new string[3];
    string *hashedrecoveryAns = new string[3];
    
    if(ifs.is_open())
    {
        string s;
        int idx;
        while (!ifs.eof()) {
            getline(ifs, s);
            stringstream ss(s);
            ss >> idx;
            while (!ss.eof()) {
                
                ss.ignore(5);
                getline(ss, name, ',');
                getline(ss, salt, ',');
                getline(ss, hashedPass, ',');
                for (int i = 0; i < 2; i++) {
                    getline(ss, recoveryQuestion[i], ',');
                    getline(ss, hashedrecoveryAns[i], ',');
                }
                getline(ss, recoveryQuestion[2], ',');
                getline(ss, hashedrecoveryAns[2]);
                user *temp = new user(name, salt, hashedPass, recoveryQuestion, hashedrecoveryAns);
                this->insertItem(*temp);
                delete temp;
            }
        }

        ifs.close();
        return true;
    }
    else
        return false;
}

void printMenu() {
    cout << "Register and login" << endl;
    cout << "1. Register" << endl;
    cout << "2. Login" << endl;
    cout << "3. Delete user" << endl;
    cout << "4. Display hash table" << endl;
    cout << "5. Clear screen" << endl;
    cout << "6. Save file" << endl;
    cout << "7. Load file" << endl;
    cout << "0. Exit" << endl;
    cout << "-----------------------------" << endl;
    cout << "Your choice:";
}

void menu()
{
    // insert the keys into the hash table
    Hash h(tableSize); // 65535 is count of buckets in hash table (max of unsigned short)
    string username, pass;
    user temp;
    while (true)
    {
        int opt;
        printMenu();
        cin >> opt;
        cin.ignore();

        switch (opt)
        {
            case 1:
                temp.newUser();
                h.insertItem(temp);
                break;
            case 2:
                cout << "Enter username: ";
                getline(cin, username);
                if(h.authItem(username))
                    cout << "login sucsessfully!";
                else
                    cout << "error";
                break;
            case 3:
                cout << "Enter username: ";
                getline(cin, username);
                if(h.authItem(username))
                {
                    struct user key(username);
                    h.deleteItem(key);
                    cout << "delete sucsessfully!";
                }
                else
                    cout << "error";
                break;
            case 4:
                h.displayHash();
                break;
            case 5:
                system(clearscreen);
                break;
            case 6:
                h.saveFile("output");
                break;
            case 7:
                h.loadFile("input");
                break;
            case 0:
                exit(0);
                break;
            default:
                break;
        }

        cout << "Press enter to continue!";
        while ((getchar()) != '\n');
    }
}

int main(int argc, const char * argv[]) {
    menu();
    return 0;
}
