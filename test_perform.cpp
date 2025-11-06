#include <bits/stdc++.h>
#include <chrono>
using namespace std;
#include "_.cpp"

string randomName(int len = 6) {
    static const string letters = "abcdefghijklmnopqrstuvwxyz";
    string s;
    for (int i = 0; i < len; i++) s += letters[rand() % letters.size()];
    return s;
}
string randomPhone() {
    string s = "0";
    for (int i = 1; i < 10; i++) s += to_string(rand() % 10);
    return s;
}
string randomAddress() {
    static const string city[] = {"Hanoi", "Saigon", "Danang", "Hue", "CanTho"};
    return city[rand() % 5];
}

int main() {
    srand(time(0));
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> testSizes = {20000, 40000, 60000, 80000, 100000};
    ofstream fout("result.csv");
    fout << "N,BST_Insert,BST_Search,BST_Remove,Hash_Insert,Hash_Search,Hash_Remove\n";
    fout.close();

    cout << left << setw(10) << "N"
         << setw(15) << "BST_Insert"
         << setw(15) << "BST_Search"
         << setw(15) << "BST_Remove"
         << setw(15) << "Hash_Insert"
         << setw(15) << "Hash_Search"
         << setw(15) << "Hash_Remove" << "\n";
    cout << string(90, '=') << "\n";

    for (int N : testSizes) {
        BSTNode* root = nullptr;
        initTable();
        vector<Contact*> contacts;
        contacts.reserve(N);

        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < N; i++) {
            Contact* c = new Contact{randomName(), randomPhone(), randomAddress(), nullptr};
            root = insertBSTContact(root, c);
            contacts.push_back(c);
        }
        auto end = chrono::high_resolution_clock::now();
        auto bst_insert_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < N; i++) insertHashContact(contacts[i]);
        end = chrono::high_resolution_clock::now();
        auto hash_insert_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; i++) {
            string name = contacts[rand() % contacts.size()]->ten;
            findContact(root, name);
        }
        end = chrono::high_resolution_clock::now();
        auto bst_search_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; i++) {
            string phone = contacts[rand() % contacts.size()]->soDienThoai;
            searchByPhone(phone);
        }
        end = chrono::high_resolution_clock::now();
        auto hash_search_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; i++) {
            string name = contacts[rand() % contacts.size()]->ten;
            Contact* removed = nullptr;
            root = removeContact(root, name, removed);
        }
        end = chrono::high_resolution_clock::now();
        auto bst_remove_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; i++) {
            string phone = contacts[rand() % contacts.size()]->soDienThoai;
            removeByPhone(phone);
        }
        end = chrono::high_resolution_clock::now();
        auto hash_remove_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        cout << left << setw(10) << N
             << setw(15) << bst_insert_time
             << setw(15) << bst_search_time
             << setw(15) << bst_remove_time
             << setw(15) << hash_insert_time
             << setw(15) << hash_search_time
             << setw(15) << hash_remove_time << "\n";

        fout.open("result.csv", ios::app);
        fout << N << ","
             << bst_insert_time << ","
             << bst_search_time << ","
             << bst_remove_time << ","
             << hash_insert_time << ","
             << hash_search_time << ","
             << hash_remove_time << "\n";
        fout.close();
    }
    return 0;
}
