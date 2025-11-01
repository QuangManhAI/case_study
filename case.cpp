#include <bits/stdc++.h>
using namespace std;

string toLowerStr(string s) {
    for (auto &ch : s) ch = tolower(ch);
    return s;
}

struct Contact {
    string name;
    string phone;
    string email;
    string address;
};

struct BSTNode {
    Contact* data;
    BSTNode* left;
    BSTNode* right;
    BSTNode(Contact* c) : data(c), left(nullptr), right(nullptr) {}
};

BSTNode* findMin(BSTNode* root) {
    while (root && root->left) root = root->left;
    return root;
}

BSTNode* bstInsert(BSTNode* root, Contact* c) {
    if (!root) return new BSTNode(c);
    string key = toLowerStr(c->name);
    string cur = toLowerStr(root->data->name);
    if (key < cur) root->left = bstInsert(root->left, c);
    else if (key > cur) root->right = bstInsert(root->right, c);
    else cout << "Lien he da ton tai theo ten.\n";
    return root;
}

BSTNode* bstSearch(BSTNode* root, const string& name) {
    if (!root) return nullptr;
    string key = toLowerStr(name);
    string cur = toLowerStr(root->data->name);
    if (key == cur) return root;
    if (key < cur) return bstSearch(root->left, name);
    return bstSearch(root->right, name);
}

BSTNode* bstRemove(BSTNode* root, const string& name, Contact*& removed) {
    if (!root) return nullptr;
    string key = toLowerStr(name);
    string cur = toLowerStr(root->data->name);
    if (key < cur) {
        root->left = bstRemove(root->left, name, removed);
    } else if (key > cur) {
        root->right = bstRemove(root->right, name, removed);
    } else {
        removed = root->data;
        if (!root->left) {
            BSTNode* r = root->right;
            delete root;
            return r;
        } else if (!root->right) {
            BSTNode* l = root->left;
            delete root;
            return l;
        } else {
            BSTNode* tmp = findMin(root->right);
            root->data = tmp->data;
            Contact* dummy = nullptr;
            root->right = bstRemove(root->right, tmp->data->name, dummy);
        }
    }
    return root;
}

void bstInorder(BSTNode* root) {
    if (!root) return;
    bstInorder(root->left);
    cout << root->data->name << " | "
         << root->data->phone << " | "
         << root->data->email << " | "
         << root->data->address << "\n";
    bstInorder(root->right);
}

class HashTable {
public:
    static const int TABLE_SIZE = 101;
    vector<list<Contact*>> table;

    HashTable() {
        table.resize(TABLE_SIZE);
    }

    int hashPhone(const string& phone) {
        long long h = 0;
        for (char c : phone) {
            if (isdigit(c)) h = h * 10 + (c - '0');
            else h = h * 131 + c;
        }
        return (int)(h % TABLE_SIZE);
    }

    void insert(Contact* c) {
        int idx = hashPhone(c->phone);
        for (auto *p : table[idx]) {
            if (p->phone == c->phone) {
                cout << "So dien thoai da ton tai.\n";
                return;
            }
        }
        table[idx].push_back(c);
    }

    Contact* search(const string& phone) {
        int idx = hashPhone(phone);
        for (auto *p : table[idx]) {
            if (p->phone == phone) return p;
        }
        return nullptr;
    }

    bool remove(const string& phone) {
        int idx = hashPhone(phone);
        for (auto it = table[idx].begin(); it != table[idx].end(); ++it) {
            if ((*it)->phone == phone) {
                table[idx].erase(it);
                return true;
            }
        }
        return false;
    }
};

struct AddressBook {
    BSTNode* root;
    HashTable ht;

    AddressBook() : root(nullptr) {}

    void addContact(const string& name,
                    const string& phone,
                    const string& email,
                    const string& address) {
        if (bstSearch(root, name)) {
            cout << "Ten nay da ton tai, khong them.\n";
            return;
        }
        Contact* c = new Contact{name, phone, email, address};
        root = bstInsert(root, c);
        ht.insert(c);
        cout << "Da them lien he.\n";
    }

    Contact* searchByName(const string& name) {
        BSTNode* node = bstSearch(root, name);
        return node ? node->data : nullptr;
    }

    Contact* searchByPhone(const string& phone) {
        return ht.search(phone);
    }

    void deleteByName(const string& name) {
        Contact* removed = nullptr;
        root = bstRemove(root, name, removed);
        if (removed) {
            ht.remove(removed->phone);
            delete removed;
            cout << "Da xoa lien he theo ten.\n";
        } else {
            cout << "Khong tim thay ten.\n";
        }
    }

    void deleteByPhone(const string& phone) {
        Contact* c = ht.search(phone);
        if (!c) {
            cout << "Khong tim thay so dien thoai.\n";
            return;
        }
        string name = c->name;
        Contact* removed = nullptr;
        root = bstRemove(root, name, removed);
        ht.remove(phone);
        if (removed) {
            delete removed;
            cout << "Da xoa lien he theo sdt.\n";
        }
    }

    void updateByName(const string& name) {
        BSTNode* node = bstSearch(root, name);
        if (!node) {
            cout << "Khong tim thay lien he.\n";
            return;
        }
        Contact* c = node->data;

        cout << "Nhap sdt moi (de trong neu giu): ";
        string p; getline(cin, p);
        cout << "Nhap email moi (de trong neu giu): ";
        string e; getline(cin, e);
        cout << "Nhap dia chi moi (de trong neu giu): ";
        string a; getline(cin, a);

        if (!p.empty() && p != c->phone) {
            ht.remove(c->phone);
            c->phone = p;
            ht.insert(c);
        }
        if (!e.empty()) c->email = e;
        if (!a.empty()) c->address = a;

        cout << "Cap nhat thanh cong.\n";
    }

    void printAll() {
        cout << "===== DANH BA =====\n";
        bstInorder(root);
    }
};

void showMenu() {
    cout << "\n===== MENU =====\n";
    cout << "1. Them lien he\n";
    cout << "2. Tim theo ten\n";
    cout << "3. Tim theo sdt\n";
    cout << "4. Xoa theo ten\n";
    cout << "5. Xoa theo sdt\n";
    cout << "6. Cap nhat theo ten\n";
    cout << "7. In danh ba\n";
    cout << "0. Thoat\n";
    cout << "Chon: ";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    AddressBook ab;

    while (true) {
        showMenu();
        int ch;
        if (!(cin >> ch)) break;
        cin.ignore();

        if (ch == 0) break;

        if (ch == 1) {
            string name, phone, email, address;
            cout << "Nhap ten: "; getline(cin, name);
            cout << "Nhap sdt: "; getline(cin, phone);
            cout << "Nhap email: "; getline(cin, email);
            cout << "Nhap dia chi: "; getline(cin, address);
            ab.addContact(name, phone, email, address);
        } else if (ch == 2) {
            string name;
            cout << "Nhap ten can tim: "; getline(cin, name);
            Contact* c = ab.searchByName(name);
            if (c) {
                cout << c->name << " | " << c->phone << " | "
                     << c->email << " | " << c->address << "\n";
            } else {
                cout << "Khong tim thay.\n";
            }
        } else if (ch == 3) {
            string phone;
            cout << "Nhap sdt can tim: "; getline(cin, phone);
            Contact* c = ab.searchByPhone(phone);
            if (c) {
                cout << c->name << " | " << c->phone << " | "
                     << c->email << " | " << c->address << "\n";
            } else {
                cout << "Khong tim thay.\n";
            }
        } else if (ch == 4) {
            string name;
            cout << "Nhap ten can xoa: "; getline(cin, name);
            ab.deleteByName(name);
        } else if (ch == 5) {
            string phone;
            cout << "Nhap sdt can xoa: "; getline(cin, phone);
            ab.deleteByPhone(phone);
        } else if (ch == 6) {
            string name;
            cout << "Nhap ten can cap nhat: "; getline(cin, name);
            ab.updateByName(name);
        } else if (ch == 7) {
            ab.printAll();
        } else {
            cout << "Khong hop le.\n";
        }
    }

    return 0;
}
