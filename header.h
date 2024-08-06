#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#define N 4
#define MAX 5
using namespace std;

class card {
public:
    char c;
    int num;
    card(int num1, char ch) {
        c = ch;
        num = num1;
    }
};

class player {
public:
    vector<card> cards;
    int chip;
    player() {
        chip = 10;
    }
};

extern vector<card> deck;

void initialization(void);
int score(vector<card> v);
vector<int> strategy(vector<card> v);
