#include <iostream>
#include <string>
#include <stdexcept>
#include "compresion.cpp"
#include "encriptacion.cpp"
#include <filesystem>
#include <thread>
#include <vector>

using namespace std;
using std::filesystem::directory_iterator;

string stripEnc(const string &path)
{
    if (path.size() >= 4 && path.substr(path.size() - 4) == ".enc")
        return path.substr(0, path.size() - 4);
    return path + ".dec";
}

void print_usage()
{
    cout << "Uso:\n"
         << "  -c              Comprimir\n"
         << "  -d              Descomprimir\n"
         << "  -e              Encriptar\n"
         << "  -u              Desencriptar\n"
         << "  -i PATH         Entrada\n"
         << "  -o PATH         Salida\n"
         << "  -k KEY          Clave\n"
         << "  -f              Modo carpeta\n";
}

void processFile(const string &input,
                 const string &output,
                 const string &key,
                 bool encrypt)
{
    string tmp = input + ".tmp";

    if (encrypt)
    {
        string tree = output + ".tree";

        encodeHuffman(input, tmp, tree);

        algoritmo_vigenere(tmp, output, key, true);

        remove(input.c_str());
        cout << "[Deleted original] " << input << endl;
    }
    else
    {
        algoritmo_vigenere(input, tmp, key, false);

        string tree = input + ".tree";

        string restored = stripEnc(input);

        decodeHuffman(tmp, tree, restored);

        cout << "[Decrypted] " << restored << endl;
    }

    remove(tmp.c_str());
}

void processFolder(const string &folder,
                   const string &key,
                   bool encrypt)
{
    vector<thread> threads;

    for (const auto &file : directory_iterator(folder))
    {
        if (!file.is_regular_file())
            continue;

        string input = file.path().string();
        string output = encrypt ? input + ".enc" : stripEnc(input);

        threads.emplace_back([=]() {
            processFile(input, output, key, encrypt);
        });
    }

    for (auto &t : threads)
        t.join();

    cout << "Folder processed: " << folder << endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        print_usage();
        return 1;
    }

    bool opt_compress=false, opt_decompress=false;
    bool opt_encrypt=false, opt_decrypt=false, folder_mode=false;

    string input, output, key;

    for (int i = 1; i < argc; i++)
    {
        string arg = argv[i];

        if (arg == "-c") opt_compress = true;
        else if (arg == "-d") opt_decompress = true;
        else if (arg == "-e") opt_encrypt = true;
        else if (arg == "-u") opt_decrypt = true;
        else if (arg == "-f") folder_mode = true;
        else if (arg == "-i" && i + 1 < argc) input = argv[++i];
        else if (arg == "-o" && i + 1 < argc) output = argv[++i];
        else if (arg == "-k" && i + 1 < argc) key = argv[++i];
        else {
            cout << "Unknown argument: " << arg << endl;
            return 1;
        }
    }

    if (key.empty()) { 
        cout << "Error: key (-k) required.\n"; 
        return 1; 
    }
    if (input.empty()) { 
        cout << "Error: input (-i) required.\n"; 
        return 1; 
    }

    if (folder_mode)
    {
        if (opt_encrypt || opt_compress)
            processFolder(input, key, true);
        else if (opt_decrypt || opt_decompress)
            processFolder(input, key, false);
        else {
            cout << "Error: choose -e/-c or -u/-d\n";
            return 1;
        }
        return 0;
    }

    if (opt_encrypt)
        processFile(input, output, key, true);
    else if (opt_decrypt)
        processFile(input, output, key, false);
    else if (opt_compress)
        encodeHuffman(input, output, output + ".tree");
    else if (opt_decompress)
        decodeHuffman(input, input + ".tree", output);
    else {
        cout << "Error: specify -c/-d or -e/-u\n";
        return 1;
    }

    return 0;
}
