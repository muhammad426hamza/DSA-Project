#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cctype> // For character checking functions like isdigit, toupper, isalpha
using namespace std;

// Account class to represent user account data
class Account
{
public:
    int accountID;
    string FirstName, LastName, Gmail, Password;
    char Gender;
    long long MobileNo; // Changed to long long to support 11-digit numbers
    Account *left;
    Account *right;
    int height;
};

// AVL Tree class for managing accounts
class AVL
{
private:
    int nextAccountID;
    Account *root;

    // Creates a new account node
    Account *create_Account(int id, string FirstName, string LastName, string Gmail, string Password, char Gender, long long MobileNo)
    {
        Account *newAccount = new Account();
        newAccount->accountID = id;
        newAccount->FirstName = FirstName;
        newAccount->LastName = LastName;
        newAccount->Gmail = Gmail;
        newAccount->Password = Password;
        newAccount->Gender = Gender;
        newAccount->MobileNo = MobileNo;
        newAccount->left = nullptr;
        newAccount->right = nullptr;
        newAccount->height = 1;
        return newAccount;
    }

    // Gets height of a node
    int getheight(Account *node)
    {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    // Returns maximum of two integers
    int max(int a, int b)
    {
        return (a > b) ? a : b;
    }

    // Calculates balance factor of a node
    int getBalanceFactor(Account *node)
    {
        if (node == nullptr)
            return 0;
        return getheight(node->left) - getheight(node->right);
    }

    // Performs left-left rotation
    Account *LLRotation(Account *node)
    {
        Account *newRoot = node->left;
        Account *temp = newRoot->right;
        newRoot->right = node;
        node->left = temp;
        node->height = 1 + max(getheight(node->left), getheight(node->right));
        newRoot->height = 1 + max(getheight(newRoot->left), getheight(newRoot->right));
        return newRoot;
    }

    // Performs right-right rotation
    Account *RRRotation(Account *node)
    {
        Account *newRoot = node->right;
        Account *temp = newRoot->left;
        newRoot->left = node;
        node->right = temp;
        node->height = 1 + max(getheight(node->left), getheight(node->right));
        newRoot->height = 1 + max(getheight(newRoot->left), getheight(newRoot->right));
        return newRoot;
    }

    // Performs left-right rotation
    Account *LRRotation(Account *node)
    {
        node->left = RRRotation(node->left);
        return LLRotation(node);
    }

    // Performs right-left rotation
    Account *RLRotation(Account *node)
    {
        node->right = LLRotation(node->right);
        return RRRotation(node);
    }

    // Inserts a new account into the AVL tree
    Account *BuildTree(Account *node, int accountID, string FirstName, string LastName, string Gmail, string Password, char Gender, long long MobileNo)
    {
        if (node == nullptr)
        {
            return create_Account(accountID, FirstName, LastName, Gmail, Password, Gender, MobileNo);
        }

        if (accountID < node->accountID)
        {
            node->left = BuildTree(node->left, accountID, FirstName, LastName, Gmail, Password, Gender, MobileNo);
        }
        else if (accountID > node->accountID)
        {
            node->right = BuildTree(node->right, accountID, FirstName, LastName, Gmail, Password, Gender, MobileNo);
        }

        node->height = 1 + max(getheight(node->left), getheight(node->right));
        int balance = getBalanceFactor(node);

        if (balance > 1 && accountID < node->left->accountID)
        {
            return LLRotation(node);
        }
        if (balance < -1 && accountID > node->right->accountID)
        {
            return RRRotation(node);
        }
        if (balance > 1 && accountID > node->left->accountID)
        {
            return LRRotation(node);
        }
        if (balance < -1 && accountID < node->right->accountID)
        {
            return RLRotation(node);
        }
        return node;
    }

    // Finds the node with minimum value
    Account *minValueNode(Account *node)
    {
        Account *current = node;
        while (current->left != nullptr)
        {
            current = current->left;
        }
        return current;
    }

    // Deletes a node from the AVL tree
    Account *deleteNode(Account *node, int accountID)
    {
        if (node == nullptr)
        {
            return node;
        }

        if (accountID < node->accountID)
        {
            node->left = deleteNode(node->left, accountID);
        }
        else if (accountID > node->accountID)
        {
            node->right = deleteNode(node->right, accountID);
        }
        else
        {
            if (node->left == nullptr || node->right == nullptr)
            {
                Account *temp = node->left ? node->left : node->right;
                if (temp == nullptr)
                {
                    temp = node;
                    node = nullptr;
                }
                else
                {
                    *node = *temp;
                }
                delete temp;
            }
            else
            {
                Account *temp = minValueNode(node->right);
                node->accountID = temp->accountID;
                node->FirstName = temp->FirstName;
                node->LastName = temp->LastName;
                node->Gmail = temp->Gmail;
                node->Password = temp->Password;
                node->Gender = temp->Gender;
                node->MobileNo = temp->MobileNo;
                node->right = deleteNode(node->right, temp->accountID);
            }
        }

        if (node == nullptr)
        {
            return node;
        }

        node->height = 1 + max(getheight(node->left), getheight(node->right));
        int balance = getBalanceFactor(node);

        if (balance > 1)
        {
            return (getBalanceFactor(node->left) >= 0) ? LLRotation(node) : LRRotation(node);
        }
        if (balance < -1)
        {
            return (getBalanceFactor(node->right) <= 0) ? RRRotation(node) : RLRotation(node);
        }

        return node;
    }

    // Searches for an account by Gmail and Password
    Account *searchByGmailAndPassword(Account *node, string Gmail, string Password)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        if (node->Gmail == Gmail && node->Password == Password)
        {
            return node;
        }

        Account *leftResult = searchByGmailAndPassword(node->left, Gmail, Password);
        if (leftResult != nullptr)
        {
            return leftResult;
        }

        return searchByGmailAndPassword(node->right, Gmail, Password);
    }

    // Performs inorder traversal to display accounts
    void Inorder(Account *node)
    {
        if (node == nullptr)
        {
            return;
        }

        Inorder(node->left);
        cout << left << setw(25) << (node->FirstName + " " + node->LastName) << " | "
             << left << setw(30) << node->Gmail << " | "
             << node->Gender << " | "
             << node->MobileNo << endl;
        Inorder(node->right);
    }

    // Saves accounts to file in inorder
    void saveInorder(Account *node, ofstream &out)
    {
        if (node == nullptr)
        {
            return;
        }

        saveInorder(node->left, out);
        out << node->accountID << "|"
            << node->FirstName << "|"
            << node->LastName << "|"
            << node->Gmail << "|"
            << node->Password << "|"
            << node->Gender << "|"
            << node->MobileNo << "\n";
        saveInorder(node->right, out);
    }

public:
    // Constructor initializes AVL tree
    AVL()
    {
        root = nullptr;
        nextAccountID = 1;
    }

    // Inserts a new account with validations
    void insert()
    {
        string FirstName, LastName, Username, Password;
        char Gender;
        long long MobileNo; // Changed to long long

        system("cls");
        cout << "\n=== Insert New Account ===\n\n";

        cin.ignore();

        // First Name validation
        while (true)
        {
            cout << "Enter First Name (alphabets only): ";
            getline(cin, FirstName);
            bool valid = true;
            for (char c : FirstName)
            {
                if (!isalpha(c))
                {
                    valid = false;
                    break;
                }
            }
            if (valid && !FirstName.empty())
            {
                break;
            }
            cout << endl << "Invalid First Name! Must contain only alphabets and cannot be empty." << endl;
        }

        // Last Name validation
        while (true)
        {
            cout << "Enter Last Name (alphabets only): ";
            getline(cin, LastName);
            bool valid = true;
            for (char c : LastName)
            {
                if (!isalpha(c))
                {
                    valid = false;
                    break;
                }
            }
            if (valid && !LastName.empty())
            {
                break;
            }
            cout << endl << "Invalid Last Name! Must contain only alphabets and cannot be empty." << endl;
        }

        // Username validation (combination of characters and digits)
        while (true)
        {
            cout << "Enter Gmail Username (combination of characters and digits): ";
            cin >> Username;
            bool hasAlpha = false, hasDigit = false;
            for (char c : Username)
            {
                if (isalpha(c))
                {
                    hasAlpha = true;
                }
                if (isdigit(c))
                {
                    hasDigit = true;
                }
            }
            if (hasAlpha && hasDigit && !Username.empty())
            {
                break;
            }
            cout << endl << "Invalid Username! Must contain at least one letter and one digit, and cannot be empty." << endl;
        }

        // Password validation (combination of characters and digits)
        while (true)
        {
            cout << "Enter Password (combination of characters and digits): ";
            cin >> Password;
            bool hasAlpha = false, hasDigit = false;
            for (char c : Password)
            {
                if (isalpha(c))
                {
                    hasAlpha = true;
                }
                if (isdigit(c))
                {
                    hasDigit = true;
                }
            }
            if (hasAlpha && hasDigit && !Password.empty())
            {
                break;
            }
            cout << endl << "Invalid Password! Must contain at least one letter and one digit, and cannot be empty." << endl;
        }

        // Gender input and validation
        cout << "Enter Gender (M/F): ";
        cin >> Gender;

        Gender = toupper(Gender); // Convert to uppercase
        while (Gender != 'M' && Gender != 'F')
        {
            cout << endl << "Invalid gender! Enter M or F: ";
            cin >> Gender;
            Gender = toupper(Gender);
        }

        // Updated mobile number input and validation
        string mobileStr;
        bool validMobile = false;
        while (!validMobile)
        {
            cout << "Enter Mobile No (11 digits, e.g., 03XXXXXXXXX): ";
            cin >> mobileStr;

            if (mobileStr.length() != 11)
            {
                cout << endl << "Invalid mobile number! Must be exactly 11 digits." << endl;
                continue;
            }

            validMobile = true;
            for (char c : mobileStr)
            {
                if (!isdigit(c))
                {
                    validMobile = false;
                    cout << endl << "Invalid mobile number! Must contain only digits (0-9)." << endl;
                    break;
                }
            }
        }

        MobileNo = stoll(mobileStr); // Convert string to long long

        string Gmail = Username + "@gmail.com"; // Auto add @gmail.com only on insert

        int newID = nextAccountID++;
        root = BuildTree(root, newID, FirstName, LastName, Gmail, Password, Gender, MobileNo);

        cout << "\nAccount created successfully!\n";
        cout << "Full Gmail: " << Gmail << endl;
        system("pause");
    }

    // Searches for an account
    void search()
    {
        if (root == nullptr)
        {
            system("cls");
            cout << "\nNo accounts in the system!\n\n";
            system("pause");
            return;
        }

        string Gmail, Password;
        system("cls");
        cout << "\n=== Search Account by Gmail & Password ===\n\n";
        cout << "Enter Full Gmail (e.g., user@gmail.com): ";
        cin >> Gmail;
        cout << "Enter Password: ";
        cin >> Password;
        cin.ignore();

        Account *result = searchByGmailAndPassword(root, Gmail, Password);

        if (result == nullptr)
            cout << "\nAccount not found or incorrect password.\n";
        else
        {
            cout << "\n--- Account Found ---\n";
            cout << "Name       : " << result->FirstName << " " << result->LastName << endl;
            cout << "Gmail      : " << result->Gmail << endl;
            cout << "Gender     : " << result->Gender << endl;
            cout << "Mobile No  : " << result->MobileNo << endl;
        }
        system("pause");
    }

    // Removes an account
    void remove()
    {
        if (root == nullptr)
        {
            system("cls");
            cout << "\nTree is empty! Nothing to delete.\n\n";
            system("pause");
            return;
        }

        string Gmail, Password;
        system("cls");
        cout << "\n=== Delete Account by Gmail & Password ===\n\n";
        cout << "Enter Full Gmail (e.g., user@gmail.com): ";
        cin >> Gmail;
        cout << "Enter Password: ";
        cin >> Password;
        cin.ignore();

        Account *accountToDelete = searchByGmailAndPassword(root, Gmail, Password);

        if (accountToDelete == nullptr)
            cout << "\nAccount not found or incorrect password.\n";
        else
        {
            root = deleteNode(root, accountToDelete->accountID);
            cout << "\nAccount (Name: " << accountToDelete->FirstName << " " << accountToDelete->LastName << ") deleted successfully!\n";
        }
        system("pause");
    }

    // Displays all accounts in order
    void inorder()
    {
        system("cls");
        cout << "\n=== All Accounts (Sorted by Account ID) ===\n\n";
        if (root == nullptr)
        {
            cout << "No accounts in the system.\n\n";
        }
        else
        {
            cout << "Name                       | Gmail                          | G | Mobile     \n";
            cout << "---------------------------+--------------------------------+---+------------\n";
            Inorder(root);
            cout << "----------------------------------------------------------------------------------\n";
        }
        system("pause");
    }

    // Saves tree to file
    void saveToFile()
    {
        ofstream out("Gmailaccounts.txt");
        if (out.is_open())
        {
            saveInorder(root, out);
            out.close();
        }
    }

    // Loads tree from file
    void loadFromFile()
    {
        ifstream in("Gmailaccounts.txt");
        if (!in.is_open())
            return;

        string line;
        int maxID = 0;

        while (getline(in, line))
        {
            if (line.empty())
                continue;

            stringstream ss(line);
            string idStr, fn, ln, gmail, pass, genderStr, mobileStr;

            getline(ss, idStr, '|');
            getline(ss, fn, '|');
            getline(ss, ln, '|');
            getline(ss, gmail, '|');
            getline(ss, pass, '|');
            getline(ss, genderStr, '|');
            getline(ss, mobileStr);

            int id = 0;
            for (char c : idStr)
                if (c >= '0' && c <= '9')
                    id = id * 10 + (c - '0');

            long long mobile = stoll(mobileStr); // Changed to stoll for long long

            char gender = genderStr.empty() ? 'M' : toupper(genderStr[0]); // Ensure uppercase on load

            root = BuildTree(root, id, fn, ln, gmail, pass, gender, mobile);

            if (id > maxID)
                maxID = id;
        }

        nextAccountID = maxID + 1;
        in.close();
    }
};

// Main function with menu-driven interface
int main()
{
    AVL tree;
    tree.loadFromFile();

    int choice;
    while (true)
    {
        system("cls");
        cout << "=== AVL Tree Account Management System ===\n\n";
        cout << "1. Insert Account (Auto ID)\n";
        cout << "2. Search Account (Gmail + Password)\n";
        cout << "3. Delete Account (Gmail + Password)\n";
        cout << "4. Display All Accounts\n";
        cout << "5. Exit\n\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            tree.insert();
            break;
        case 2:
            tree.search();
            break;
        case 3:
            tree.remove();
            break;
        case 4:
            tree.inorder();
            break;
        case 5:
            tree.saveToFile();
            cout << "\nData saved successfully. Goodbye!\n";
            return 0;
        default:
            cout << "\nInvalid choice! Please try again.\n";
            system("pause");
        }
    }

    return 0;
}