#include <iostream>
#include <deque>
#include <unordered_set>
#include <list>
#include <fstream>

using namespace std;

class PageReplacement {
    int numPages, numFrames, numSwapBlocks;
    int pageFaults = 0;
    deque<int> frames; // memory of size numFrames
    unordered_set<int> frameSet; // set to track frames in memory
    unordered_set<int> swapSpace; // swap space of size numSwapBlocks

public:
    PageReplacement(int numPages, int numFrames, int numSwapBlocks)
        : numPages(numPages), numFrames(numFrames), numSwapBlocks(numSwapBlocks) {}

    void referencePage(int page) {

        if (frameSet.find(page) == frameSet.end()) { //page fault
            pageFaults++;
            swapSpace.erase(page);

            if (frames.size() < (long unsigned int)numFrames) { // space available in memory
                frames.push_back(page);
            } else { // otherwise, evict from the front
                int evictedPage = frames.front();
                frames.pop_front();
                frameSet.erase(evictedPage);

                if (swapSpace.size() < (long unsigned int)numSwapBlocks) {
                    swapSpace.insert(evictedPage);
                } else {
                    throw "Swap space exceeded!";
                }

                frames.push_back(page);
            }

            frameSet.insert(page);

        } else { //page hit
            // send page to the back of deque
            auto it = frames.begin();
            for (; it != frames.end(); it++) {
                if (*it == page)
                    break;
            }
            frames.erase(it);
            frames.push_back(page);
        }
    }

    int getPageFaults() {
        return this->pageFaults;
    }
};

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cout << "Usage: ./page_replacement <numPages> <numFrames> <numSwapBlocks> <inputFile>" << endl;
        return 1;
    }

    int numPages = atoi(argv[1]);
    int numFrames = atoi(argv[2]);
    int numSwapBlocks = atoi(argv[3]);
    string fileName = argv[4];

    try {
        ifstream infile(fileName);

        PageReplacement* pr = new PageReplacement(numPages, numFrames, numSwapBlocks);
        int page;
        while (infile >> page) {
            pr->referencePage(page);
        }
        cout << "Number of page faults: " << pr->getPageFaults() << "\n";
        delete pr;

        infile.close();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}