#include <iostream>
#include <filesystem>
#include <fstream>
#include <Windows.h>
#include <thread>
#include <string>
#include <atomic>

using namespace std::filesystem;
std::atomic<int> count{ 0 };
std::atomic<int> count_of_files{ 1000 };

path my_essay = "./essay of reflection.txt";

void create_folder(path folderPath) {
    // 1. Создание папки
    if (!exists(folderPath)) {
        create_directory(folderPath);
        std::cout << "Папка " << folderPath << " создана.\n";
    }
    else {
        std::cout << "Папка уже существует.\n";
    }
}


void fill_directory(path folderPath) {
    while (count < count_of_files) {
        // 2. Создание файла внутри папки
        std::string name = "example_file" + std::to_string(count) + ".txt";
        path filePath = folderPath / name;
        count++;
        std::ofstream file(filePath);
        std::ifstream essay(my_essay);
        if (file.is_open() && essay.is_open()) {
            std::string some_line;
            while (std::getline(essay, some_line)) {
                file << some_line;
            }
            file.close();
            essay.close();
            std::cout << "Файл " << filePath << " создан и записан.\n";
            
        }
    }
}


int main() {
    SetConsoleOutputCP(1251);
    int count_of_threads{ 8 };
    
    path folderPath = "example_folder";
    
    std::vector<std::thread> threads;
    std::thread thread1(create_folder, folderPath);

    thread1.join();

    auto start = std::chrono::high_resolution_clock::now();

    for (int j = 0; j < count_of_threads; j++) {   
        threads.emplace_back(fill_directory, folderPath);
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    //+-----------------------example in single thread----------------------------+
    int count_of_files_local = count_of_files;
    path folderPath2 = "example_folder2";
    auto start2 = std::chrono::high_resolution_clock::now();

    create_directory(folderPath2);

    for (int i = 0; i < count_of_files_local; i++) {
        fill_directory(folderPath2);
    }

    auto end2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    std::chrono::duration<double> duration2 = end2 - start2;
    std::cout << "Час виконання многопотоці: " << duration.count() << " секунд\n";
    std::cout << "Час виконання однопотоці: " << duration2.count() << " секунд\n";

    return 0;
}