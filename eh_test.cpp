#include "extendible-hashing.hpp"
void menu(){
    cout << "---------------------------" << endl;
    cout << "Usage: (extensible hashing support multiple types by using template," << endl;
    cout << "but we just provide the string type for testing.)" << endl;
    cout << "insert: <key> <value>" << endl;
    cout << "update: <key> <new value>" << endl;
    cout << "remove: <key>" << endl;
    cout << "search: <key>" << endl;
    cout << "display"  << endl;
    cout << "exit"  << endl;
    cout << "---------------------------" << endl;

}
int main(){
    bool verbosity = 1;
    bool manunal_input = 0;
    int bucket_size = DEFAULT_BUCKET_SIZE;
    int directory_depth = 1;
    int key;
    string value, input, *output;
    if (manunal_input){
        cout << "Bucket size: " ;
        cin >> bucket_size;
        cout << "Directory size: ";
        cin >> directory_depth;
    }
    Directory<string> dir {directory_depth, bucket_size};
    cout << endl << "Initialized directory: done!" << endl;
    if (verbosity)
        menu();
    do {
        cout << endl;
        cout << "eh>>>: ";
        cin >> input;
        if (!input.compare("insert")){
            cout <<"  >>> key: ";
            cin >> key;
            cout << "  >>> value: ";
            cin >> value;
            dir.insert(key, move(value));
        }
        if (!input.compare("update")){
            cout <<"  >>> key: ";
            cin >> key;
            cout << "  >>> value: ";
            cin >> value;
            dir.update(key, move(value));
        }
        if (!input.compare("delete")){
            cout <<"  >>> key: ";
            cin >> key;
            dir.remove(key);
        }
        if (!input.compare("search")){
            cout <<"  >>> key: ";
            cin >> key;
            output = dir.search(key);
            if (output)
                cout << *output << endl;
        }
        if (!input.compare("display")){
            dir.display();
        }


    }while (input.compare("exit"));
    return 0;
}