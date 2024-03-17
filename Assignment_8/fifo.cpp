#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <queue>
#include <unordered_map>

using namespace std;

void handle_page_fault(unordered_map<int, bool>& main_memory, int number_of_frames, int requested_page, queue<int>& temp_queue, unordered_map<int, int>& page_table, int& number_of_frames_in_memory) {
    temp_queue.push(requested_page);

    if (number_of_frames_in_memory < number_of_frames) {
        number_of_frames_in_memory++;
        cout << "No page is removed from the main memory\n";
    } else {
        int removing_page_number = temp_queue.front();
        main_memory[page_table[removing_page_number]] = false;
        temp_queue.pop();
        cout << "Removed page from the main memory: " << removing_page_number << endl;
    }

    main_memory[page_table[requested_page]] = true;
    return;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cout << " Syntax: ./a.out <Number of pages> <Number of Frames in Main Memory> <Number of Blocks> <Path to the file containing requests>\n";
        exit(EXIT_FAILURE);
    }

    int number_of_pages, number_of_frames, number_of_blocks;

    try {
        number_of_pages = atoi(argv[1]), number_of_frames = atoi(argv[2]), number_of_blocks = atoi(argv[3]);
        cout << "Number of pages: " << number_of_pages << endl;
        cout << "Number of frames in main memory: " << number_of_frames << endl;
        cout << "Number of blocks: " << number_of_blocks << endl << endl;
    } catch (exception e) {
        cout << " Syntax: ./a.out <Number of pages> <Number of Frames in Main Memory> <Number of Blocks> <Path to the file containing requests>\n";
        exit(EXIT_FAILURE);
    }

    ifstream request_file;

    try {
        request_file.open(argv[4]);
    } catch (exception e) {
        cout << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    unordered_map<int, int> page_table;
    unordered_map<int, bool> main_memory;
    queue<int> temp_queue;
    int max_alloted_frame = 0, page_fault = 0, number_of_frames_in_memory = 0;

    while (!request_file.eof()) {
        int requested_page;
        request_file >> requested_page;
        cout << "Requested Page is: " << requested_page << endl;
        if (requested_page > number_of_pages) {
            cout << "Segmentation Fault!\n\n";
            continue;
        }

        if (page_table.find(requested_page) == page_table.end()) {
            max_alloted_frame++;
            if (max_alloted_frame > number_of_blocks) {
                cout << "Need more Blocks!\n";
                exit(EXIT_FAILURE);
            }
            page_table[requested_page] = max_alloted_frame;
            cout << "New Frame Assigned\n";
        }

        if (main_memory.find(page_table[requested_page]) != main_memory.end() && main_memory[page_table[requested_page]]) {
            cout << "Page found in the main memory\n";
        } else {
            cout << "Page Fault\n";
            page_fault++;
            handle_page_fault(main_memory, number_of_frames, requested_page, temp_queue, page_table, number_of_frames_in_memory);
        }
        cout << endl;
    }

    cout << page_fault << endl;

    return EXIT_SUCCESS;
}