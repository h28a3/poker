#include "header.h"

vector<card> deck; //山札
int player_score[N]; //プレイヤーとCOMのスコア(score関数参照)
vector<card> discard;

int main(void) {
    int judge = 1; //ループ判定用変数
    player players[N]; //プレイヤーとCOMのカード，チップ記憶
    
    while (judge > 0) { //誰かのチップがなくなるまで無限ループ
        initialization(); //山札生成
        int bet = 1; //場に一人が賭けているチップ数
        for (int i = 0;i < N;i++) { //一人に五枚ずつ配る
            vector<card> c = {};
            for (int j = 0;j < 5;j++) {
                int r = rand() % (52 - 5 * i - j);
                c.push_back(deck[r]);
                deck.erase(deck.begin() + r);
            }
            players[i].cards = c;
        }

        show_card(0, players[0].cards); //プレイヤーのカード出力

        for (int i = 0;i < N;i++) {
            player_score[i] = score(players[i].cards); //全員のスコア計算
        }

        int n = 0;
        int flag = 0;
        int flag_2 = 0;

        bid_or_pass(bet, flag, flag_2, n, players);
        call_or_raise(bet, flag, flag_2, n, players);

        if (flag_2 == 0) {
            cout << "How many cards would you like to exchange?";
            cin >> n;
            if (n == 5) { //5枚全て交換するとき
                for (int i = 0;i < 5;i++) {
                    int r = rand() % (32 - i);
                    players[0].cards[i] = deck[r];
                    deck.erase(deck.begin() + r);
                }
            }
            else if (n > 0) {
                cout << "Which card(s) would you like to replace? (Enter separated by spaces)";
                for (int i = 0;i < n;i++) {
                    int rep;
                    cin >> rep;
                    discard.push_back(players[0].cards[rep - 1]);
                    int r = rand() % (32 - i);
                    players[0].cards[rep - 1] = deck[r];
                    deck.erase(deck.begin() + r);
                }
            }
            show_card(0, players[0].cards); //交換後のプレイヤーのカード出力
        }

        for (int i = 1;i < N;i++) {
            vector<int> rep = strategy(players[i].cards); //COMが交換するカード
            for (int j : rep) {
                int r = rand() % (deck.size());
                players[i].cards[j] = deck[r];
                deck.erase(deck.begin() + r);
                if (deck.size() == 0) {
                    copy(discard.begin(), discard.end(), deck.begin());
                    discard.clear();
                }
            }
            cout << "COM" << i << ": exchanged " << rep.size() << " cards.\n";
        }

        n = 0;
        flag = 0;
        bid_or_pass(bet, flag, flag_2, n, players);
        int sum = 0;
        for (int i = 0;i < N;i++) {
            if (i == 0 && flag_2 == 1) {
                continue;
            }
            int a = min(bet, players[i].chip);
            sum += a;
            players[i].chip -= a;
        }

        vector<int> card_sum;
        for (int i = 0;i < N;i++) {
            if (i == 0 && flag_2 == 1) {
                card_sum.push_back(0);
                player_score[0] = 0;
                continue;
            }
            player_score[i] = score(players[i].cards);
            int a = 0;
            for (int j = 0;j < 5;j++) {
                if (players[i].cards[j].num == 1) {
                    a += 14;
                }
                else {
                    a += players[i].cards[j].num;
                }
            }
            card_sum.push_back(a);
            if (i > 0) {
                show_card(i, players[i].cards); //交換後のCOMのカード出力
            }
        }
        int winner = *max_element(begin(player_score), end(player_score));

        if (count(begin(player_score), end(player_score), winner) == 1) { //スコアで一人勝ちの場合はそのまま場のチップ総取り
            for (int i = 0;i < N;i++) {
                if (player_score[i] == winner) {
                    players[i].chip += sum;
                    if (i == 0) {
                        cout << "player win\n";
                    }
                    else {
                        cout << "COM" << i << " wins\n";
                    }
                }
            }
        }
        else { //スコアで同点の人がいる場合，カードの総和で勝負(ただしポーカーでは1が一番強いので，1は14としてカウント)
            vector<int> winners,tmp;
            for (int i = 0;i < N;i++) {
                if (player_score[i] == winner) {
                    winners.push_back(i);
                }
            }
            for (int i : winners) {
                tmp.push_back(card_sum[i]);
            }
            int max = *max_element(tmp.begin(), tmp.end());
            if (count(tmp.begin(), tmp.end(), max) == 1) { //カードの総和で一人勝ちの場合はそのまま場のチップ総取り
                for (int i : winners) {
                    if (card_sum[i] == max) {
                        players[i].chip += sum;
                        if (i == 0) {
                            cout << "player win\n";
                        }
                        else {
                            cout << "COM" << i << " wins\n";
                        }
                    }
                }
            }
            else { //カードの総和で同点の人がいる場合はそのまま場のチップを山分け
                int n = count(tmp.begin(), tmp.end(), max);
                for (int i : winners) {
                    if (card_sum[i] == max) {
                        players[i].chip += sum / n;
                        if (i == 0) {
                            cout << "player and";
                        }
                        else {
                            cout << "COM" << i << " and";
                        }
                    }
                }
                cout << " win\n";
            }
        }
        for (int i = 0;i < N;i++) {
            show_chip(i, players[i].chip);
        }
        cout << "\n";
        judge = 1;
        for(int i=0;i<N;i++){
            judge *= players[i].chip;
        }
    }
    return 0;
}
