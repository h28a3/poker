#include "header.h"

void initialization() { //山札生成
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    srand((unsigned int)time(NULL));
    char set_card[4] = { 's','h','d','c' };
    for (int i = 1;i <= 13; i++) {
        for (char j : set_card) {
            card c(i, j);
            deck.push_back(c);
        }
    }
}

int score(vector<card> v) { //スコア計算
    vector<int> set, sorted;
    vector<char> set_c;
    for (card i : v) {
        if (i.num == 1) {
            set.push_back(14);
            sorted.push_back(14);
        }
        else {
            set.push_back(i.num);
            sorted.push_back(i.num);
        }
        set_c.push_back(i.c);
    }
    sort(sorted.begin(), sorted.end());

    sort(set.begin(), set.end());
    auto result1 = unique(set.begin(), set.end());
    set.erase(result1, set.end());

    sort(set_c.begin(), set_c.end());
    auto result2 = unique(set_c.begin(), set_c.end());
    set_c.erase(result2, set_c.end());

    if (set.size() == 5 && sorted[0] == 10 && set_c.size() == 1) //ロイヤルストレートフラッシュ
        return 9;
    else if (set.size() == 5 && sorted[0] + 4 == sorted[4] && set_c.size() == 1) //ストレート・フラッシュ
        return 8;
    else if (sorted[0] == sorted[3] || sorted[1] == sorted[4]) //フォア・カード
        return 7;
    else if (set.size() == 2) //フルハウス
        return 6;
    else if (set_c.size() == 1) //フラッシュ
        return 5;
    else if (set.size() == 5 && sorted[0] + 4 == sorted[4]) //ストレート
        return 4;
    else if (sorted[0] == sorted[2] || sorted[1] == sorted[3] || sorted[2] == sorted[4]) //スリーカード
        return 3;
    else if (set.size() == 3) //ツウ・ペア
        return 2;
    else if (set.size() == 4) //ワン・ペア
        return 1;
    else
        return 0;
}

vector<int> strategy(vector<card> v) { //COMのカード交換戦略
    vector<int> ans, set_num, sorted;
    vector<char> e, set_c;
    for (card i : v) {
        if (i.num == 1) {
            set_num.push_back(14);
            sorted.push_back(14);
        }
        else {
            set_num.push_back(i.num);
            sorted.push_back(i.num);
        }
        e.push_back(i.c);
        set_c.push_back(i.c);
    }
    unique(set_c.begin(), set_c.end());
    sort(sorted.begin(), sorted.end());

    if (set_c.size() == 1 || set_num.size() == 2) {
        return ans;
    }
    else if (set_num.size() < 5) {
        for (int i = 0;i < 5;i++) {
            int cnt = count(sorted.begin(), sorted.end(), sorted[i]);
            if (cnt == 1) {
                ans.push_back(i);
            }
        }
        return ans;
    }
    else if ((set_c.size() == 2) && (count(e.begin(),e.end(),set_c[0]) == 1 || count(e.begin(), e.end(), set_c[0]) == 4)) {
        for (char c : set_c) {
            int cnt = count(e.begin(), e.end(), c);
            if (cnt == 1) {
                for (int i = 0;i < 5;i++) {
                    if (v[i].c == c) ans.push_back(i);
                    break;
                }
            }
        }
        return ans;
    }
    else if (set_num.size() == 5) {
        if (sorted[0] + 4 == sorted[4]) {
            return ans;
        }
        else if (sorted[0] + 3 == sorted[3]) {
            for (int i = 0;i < 5;i++) {
                if (v[i].num == sorted[4])
                    ans.push_back(i);
            }
        }
        else if (sorted[1] + 3 == sorted[4]) {
            for (int i = 0;i < 5;i++) {
                if (v[i].num == sorted[0])
                    ans.push_back(i);
            }
        }
        if (ans.size() > 0) {
            return ans;
        }
    }
    for (int i = 0;i < 5;i++) {
        ans.push_back(i);
    }
    return ans;
}

void show_card(int n, vector<card> cards) { //カード出力
    if (n == 0) {
        cout << "Your ";
    }
    else {
        cout << "COM" << n << "'s ";
    }
    cout << "cards is ";
    for (int i = 0;i < 5;i++) {
        if (cards[i].c == 's') cout << "spade";
        if (cards[i].c == 'h') cout << "heart";
        if (cards[i].c == 'd') cout << "diamond";
        if (cards[i].c == 'c') cout << "club";
        cout << cards[i].num << ", ";
    }
    cout << ".\n";
}

void show_chip(int n, int chip) { //チップの残り枚数出力
    if (n == 0) {
        cout << "You have ";
    }
    else {
        cout << "COM" << n << " has ";
    }
    cout << chip << " chip(s) left.\n";
}

void bid_or_pass(int &bet, int &flag, int &n, player *players) {
    int tmp = bet;
    for (int i = 1;i < N;i++) {
        if (player_score[i] > 2) { //COMはスリーカードより良い役のときはビッド
            if (bet >= players[i].chip) {
                cout << "COM" << i << ": all in\n";
                flag++;
                continue;
            }
            cout << "COM" << i << ": bid，";
            int r = rand() % (MAX - bet) + 1;
            r = min(r, players[i].chip);
            cout << "add " << r << " more.\n";
            bet += r;
            n = i;
            flag = 0;
            break;
        }
        else {
            cout << "COM" << i << ": pass\n";
            flag++;
        }
    }

    if (tmp == bet) {
        cout << "pass or bid(0: pass，1: bid)";
        int input;
        cin >> input;
        flag++;
        if (input == 1) {
            cout << "How many would you like to add?(Up to " << min(MAX, players[0].chip) - bet << " can be added.)";
            cin >> input;
            bet += input;
            n = 0;
            flag = 0;
        }
    }
}
