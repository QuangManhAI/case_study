// Thêm một liên hệ mới vào sổ địa chỉ.
// Tìm kiếm liên hệ theo tên.
// Tìm kiếm liên hệ theo số điện thoại.
// Cập nhật thông tin của một liên hệ đã có.
// Xóa một liên hệ khỏi sổ địa chỉ.
// In ra toàn bộ danh bạ theo thứ tự tên đã sắp xếp.

#include <bits/stdc++.h>
using namespace std;

struct Contact
{
    /* data */
    string ten;
    string soDienThoai;
    string diaChi;
    Contact* next;
};

struct BSTNode
{
    /* data */
    Contact* data;
    BSTNode* left;
    BSTNode* right;
    BSTNode(Contact* key): data(key), left(nullptr), right(nullptr){};
};

string toLowerString(string str){
    for (char &i : str) i = tolower(i);
    return str;
}

// them mot lien he
BSTNode* insertBSTContact(BSTNode* root, Contact* c){
    if (!root) return new BSTNode(c);
    string key = toLowerString(c->ten);
    string cur = toLowerString(root->data->ten);
    if (key < cur) root->left = insertBSTContact(root->left, c);
    else if (key > cur) root->right = insertBSTContact(root->right, c);
    return root;
}

// tim kiem lien he theo ten
BSTNode* findContact(BSTNode* root, const string& name){
    if (!root) return nullptr;
    string key = toLowerString(name);
    string cur = toLowerString(root->data->ten);
    if (key == cur) return root;
    else if (key < cur) return findContact(root->left, name);
    else if (key > cur) return findContact(root->right, name);
    else {
        return nullptr;
    }
}

// tim min ben trai de the mang
BSTNode* findMin(BSTNode* root){
    while (root && root->left)
    {
        /* code */
        root = root->left;
    }
    return root;
}
// xoa mot lien he
BSTNode* removeContact(BSTNode* root, const string& name, Contact*& removed){
    if (!root) return nullptr;
    string key = toLowerString(name);
    string cur = toLowerString(root->data->ten);
    if (key < cur) root->left = removeContact(root->left, name, removed);
    else if (key > cur) root->right = removeContact(root->right, name, removed);
    else {
        removed = root->data;
        if (!root->left){
            BSTNode* r = root->right;
            delete root;
            return r;
        } 
        else if (!root->right){
            BSTNode* l = root->left;
            delete root;
            return l;
        }
        else {
            BSTNode* temp = findMin(root->right);
            root->data = temp->data;
            Contact* changeNode = nullptr;
            root->right = removeContact(root->right, temp->data->ten, changeNode);
        }
    }
    return root;
}

// LNR de duyet tang dan
void printLNR(BSTNode* root){
    if (!root) return;
    printLNR(root->left);
    cout << left << root->data->ten << "|"<<setw(10)
         << left << root->data->soDienThoai << "|"<<setw(10)
         << left << root->data->diaChi << "|"<< endl;
    printLNR(root->right); 
}

// hashing
const int hashSize = 101;
Contact* hashTable[hashSize];

int hashPhone(const string& phone){
    long h = 0;
    for (char c: phone){
        if (isdigit(c)) h = h * 10 + (c - '0');
        else h = h * 131 + c; 
    }
    return (int)(h % hashSize);
}

void initTable(){
    for (int i = 0; i < hashSize; i++){
        hashTable[i] = nullptr;
    }
}

void insertHashContact(Contact* c){
    int idx = hashPhone(c->soDienThoai);
    Contact* cur = hashTable[idx];
    while (cur)
    {
        /* code */
        if (cur->soDienThoai == c->soDienThoai){
            return;
        }
        cur = cur->next;
    }
    c->next = hashTable[idx];
    hashTable[idx] = c;
}

Contact* searchByPhone(const string& phone){
    int idx = hashPhone(phone);
    Contact* cur =  hashTable[idx];
    while (cur)
    {
        /* code */
        if (cur->soDienThoai == phone) return cur;
        cur = cur->next;
    }
    return nullptr;
}

bool removeByPhone(const string& phone){
    int idx = hashPhone(phone);
    Contact* cur = hashTable[idx];
    Contact* prev = nullptr;
    
    while (cur)
    {
        /* code */
        if (cur->soDienThoai == phone){
            if (prev) prev->next = cur->next;
            else hashTable[idx] = cur->next;
            delete cur;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

