#include "header.h"

vector<card> deck; //山札

int main(void) {
    player players[N]; //プレイヤーとCOMのカード，チップ記憶
    int player_score[N]; //プレイヤーとCOMのスコア(score関数参照)
    int judge = 1; //ループ判定用変数
    
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

        for (int i = 1;i < N;i++) {
            if (player_score[i] > 2) { //COMはスリーカードより良い役のときはビッド
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
                n = i;
                if (player_score[i] > 4) { //COMはフラッシュより良い役のときはレイズ
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

        if (bet == 1) { //COMが誰もビッドしなかったらプレイヤーはパスかビッドを選択
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

        int sum = 0; //場に出ているチップ

        for (int i = 0;i < N;i++) {
            int a = min(bet, players[i].chip);
            sum += a;
            players[i].chip -= a;
        }

        int rest = 52 - 5 * N; //山札の残り枚数
        cout << "何枚カードを交換しますか？";
        cin >> n;
        rest -= n;
        if (n == 5) { //5枚全て交換するとき
            for (int i = 0;i < 5;i++) {
                int r = rand() % (32 - i);
                players[0].cards[i] = deck[r];
                deck.erase(deck.begin() + r);
            }
        }
        else if (n > 0) {
            cout << "何枚目を交換しますか？(空白区切りで入力)";
            for (int i = 0;i < n;i++) {
                int rep;
                cin >> rep;
                int r = rand() % (32 - i);
                players[0].cards[rep-1] = deck[r];
                deck.erase(deck.begin() + r);
            }
        }

        show_card(0, players[0].cards); //交換後のプレイヤーのカード出力

        for (int i = 1;i < N;i++) {
            vector<int> rep = strategy(players[i].cards); //COMが交換するカード
            for (int j : rep) {
                int r = rand() % (rest);
                players[i].cards[j] = deck[r];
                deck.erase(deck.begin() + r);
                rest--;
                if (rest == 0) {
                    initialization();
                }
            }
            cout << "COM" << i << "：" << rep.size() << "枚交換\n";
        }

        vector<int> card_sum;
        for (int i = 0;i < N;i++) {
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
        
        judge = 1;

        if (count(begin(player_score), end(player_score), winner) == 1) { //スコアで一人勝ちの場合はそのまま場のチップ総取り
            for (int i = 0;i < N;i++) {
                if (player_score[i] == winner) {
                    players[i].chip += sum;
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
                            cout << "プレイヤーの勝利\n";
                        }
                        else {
                            cout << "COM" << i << "の勝利\n";
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
                            cout << "プレイヤーと";
                        }
                        else {
                            cout << "COM" << i << "と";
                        }
                    }
                }
                cout << "の勝利\n";
            }
        }
        for (int i = 0;i < N;i++) {
            show_chip(i, players[i].chip);
        }
        cout << "\n";
    }
    return 0;
}
