#include "header.h"

vector<card> deck;

int main(void) {
    player players[N];
    int player_score[N];
    int judge = 1;
    
    while (judge > 0) {
        initialization();
        int bet = 1;
        for (int i = 0;i < N;i++) {
            vector<card> c = {};
            for (int j = 0;j < 5;j++) {
                int r = rand() % (52 - 5 * i - j);
                c.push_back(deck[r]);
                deck.erase(deck.begin() + r);
            }
            players[i].cards = c;
        }

        cout << "あなたのカードは，";
        for (int i = 0;i < 5;i++) {
            if (players[0].cards[i].c == 's') cout << "スペード";
            if (players[0].cards[i].c == 'h') cout << "ハート";
            if (players[0].cards[i].c == 'd') cout << "ダイヤ";
            if (players[0].cards[i].c == 'c') cout << "クローバー";
            cout << players[0].cards[i].num << "，";
        }
        cout << "です．\n";

        for (int i = 0;i < N;i++) {
            player_score[i] = score(players[i].cards);
        }

        int n = 0;
        int flag = 0;

        for (int i = 1;i < N;i++) {
            if (player_score[i] > 2) {
                if (bet >= players[i].chip) {
                    cout << "COM" << i << "：オールイン\n";
                    flag++;
                    continue;
                }
                cout << "COM" << i << "：ビッド，";
                int r = rand() % (MAX - bet) + 1;
                r = min(r, players[i].chip);
                cout << r << "枚追加\n";
                bet += r;
                n = i;
                flag = 0;
                break;
            }
            else {
                cout << "COM" << i << "：パス\n";
                flag++;
            }
        }

        if (n > 0 && n < N - 1) {
            for (int i = n + 1;i < N;i++) {
                if (player_score[i] > 4) {
                    if (bet >= players[i].chip) {
                        cout << "COM" << i << "：オールイン\n";
                        flag++;
                        continue;
                    }
                    cout << "COM" << i << "：レイズ，";
                    int r = rand() % 4 + 1;
                    r = min(r, players[i].chip);
                    cout << r << "枚追加\n";
                    bet += r;
                    flag = 0;
                }
                else {
                    cout << "COM" << i << "：コール\n";
                    flag++;
                }
            }
        }

        if (bet == 1) {
            cout << "パスorビッド(0：パス，1：ビッド)";
            int input;
            cin >> input;
            flag++;
            if (input == 1) {
                cout << "何枚追加しますか？(" << min(MAX, players[0].chip) - bet << "枚まで追加できます．)";
                cin >> input;
                bet += input;
                n = 0;
                flag = 0;
            }
            else if (input == 2)
                continue;
        }

        while (flag < N - 1 && bet < MAX) {
            n = (n + 1) % N;
            if (player_score[n] > 5 && n != 0) {
                if (bet >= players[n].chip) {
                    cout << "COM" << n << "：オールイン\n";
                    flag++;
                    continue;
                }
                cout << "COM" << n << "：レイズ，";
                int r = rand() % (MAX - bet) + 1;
                r = min(r, players[n].chip);
                cout << r << "枚追加\n";
                bet += r;
                flag = 0;
            }
            else if (n != 0) {
                cout << "COM" << n << "：コール\n";
                flag++;
            }
            else {
                cout << "コールorレイズ(0：コール，1：レイズ)";
                int input;
                cin >> input;
                flag++;
                if (input == 1) {
                    cout << "何枚追加しますか？(" << min(MAX, players[0].chip) - bet << "枚まで追加できます．)";
                    cin >> input;
                    bet += input;
                    flag = 0;
                }
            }
        }

        bet = 0;

        for (int i = 0;i < N;i++) {
            int a = min(bet, players[i].chip);
            players[i].chip -= a;
            bet += a;
        }

        int rest = 52 - 5 * N;
        cout << "何枚カードを交換しますか？";
        cin >> n;
        rest -= n;
        if (n > 0) {
            cout << "何枚目を交換しますか？(空白区切りで入力)";
            for (int i = 0;i < n;i++) {
                int rep;
                cin >> rep;
                int r = rand() % (32 - i);
                players[0].cards[rep-1] = deck[r];
                deck.erase(deck.begin() + r);
            }
        }

        cout << "あなたのカードは，";
        for (int i = 0;i < 5;i++) {
            if (players[0].cards[i].c == 's') cout << "スペード";
            if (players[0].cards[i].c == 'h') cout << "ハート";
            if (players[0].cards[i].c == 'd') cout << "ダイヤ";
            if (players[0].cards[i].c == 'c') cout << "クローバー";
            cout << players[0].cards[i].num << "，";
        }
        cout << "です．\n";

        for (int i = 1;i < N;i++) {
            vector<int> rep = strategy(players[i].cards);
            for (int j : rep) {
                int r = rand() % (rest);
                players[0].cards[j] = deck[r];
                deck.erase(deck.begin() + r);
                rest--;
            }
            cout << "COM" << i << "：" << rep.size() << "枚交換\n";
        }

        for (int i = 0;i < N;i++) {
            player_score[i] = score(players[i].cards);
        }
        int winner= *max_element(begin(player_score), end(player_score));
        
        judge = 1;
        for (int i = 0;i < N;i++) {
            if (player_score[i] == winner) {
                players[i].chip += bet;
                if (i == 0) {
                    cout << "プレイヤーの勝利\n";
                }
                else {
                    cout << "COM" << i << "の勝利\n";
                }
            }
            judge *= players[i].chip;
        }
    }
}
