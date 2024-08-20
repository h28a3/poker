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
extern int player_score[N]; //プレイヤーとCOMのスコア(score関数参照)
extern vector<card> discard;

void initialization(void);
int score(vector<card> v);
vector<int> strategy(vector<card> v);
void show_card(int n, vector<card> cards);
void show_chip(int n, int chip);
void bid_or_pass(int& bet, int& flag, int &flag_2, int& n, player *players);
void call_or_raise(int& bet, int& flag, int& flag_2, int& n, player* players);
