#include <iostream>
#include <conio.h>
#include <string>
#include <ctime>
#include <vector>
#include <queue>
using namespace std;

// 配列を大きめに取っておく
const int MAX_SIZE = 30;
// 盤面を表す2次元配列
char board[MAX_SIZE][MAX_SIZE];

// n*nマスの盤面を表す定数(変更可)
int num;
// 最短距離を格納する変数(-1で初期化)
int min_distance = -1;
// プレイヤーの行動回数
int player_move = 0;

// プレイヤーの初期位置を表す変数
int def_playerX; 
int def_playerY; 

// プレイヤーの位置を表す変数
int playerX;     
int playerY;

// 二次元座標
struct Point {
    int x, y;
};


// ゴール可能かどうかをBFS(幅優先)で探索
bool canGoal() {
    // 周りの壁の文は引く
    int distances[num][num];

    // -1で初期化する
    for (int y = 0; y < num; y++) {
        for (int x = 0; x < num; x++) {
            distances[y][x] = -1;
        }
    }

    // プレイヤーの初期位置は0に設定
    distances[def_playerY][def_playerX] = 0;

    // キューを作成し、プレイヤーの初期位置を入れる
    queue<Point> q;
    q.push({def_playerX, def_playerY});

    // 上下左右マスへのオフセットを用意(組み合わせて使用する)
    int dx[4] = {0, 0, -1, 1};
    int dy[4] = {-1, 1, 0, 0};

    // キューが空になるまで繰り返す
    while (!q.empty()) {
        // 現在地を取り出す
        Point current = q.front();
        q.pop();

        for (int i = 0; i < 4; i++) {
            // 新たなマスの座標を取得(i=0は上、i=1は下、i=2は左、i=3は右を意味する)
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            // 範囲外(周りの壁を除く)の場合はスキップする
            if (nx < 1 || nx >= num-1 || ny < 1 || ny >= num-1 ) {
                continue;
            }

            // 壁の場合はスキップする
            if (board[ny][nx] == '#') {
                continue;
            }

            // すでに訪れている場合はスキップする
            if (distances[ny][nx] != -1) {
                continue;
            }

            // 新たなマスまでの距離を記録する(スタートから何マスか？)
            distances[ny][nx] = distances[current.y][current.x] + 1;

            // キューに追加する
            q.push({nx, ny});
        }
    }

    // 最短距離を格納
    min_distance = distances[1][1];

    // ゴール地点が-1(距離が記録されなかった)でなければゴール可能！
    if (distances[1][1] != -1) {
        return true;
    } else {
        return false;
    }
}

// ランダムな座標に壁を配置
void setWall() {
    // ランダム配置する壁の個数(全体の3割)
    int wall_num = (num - 1) * (num - 1) * 0.3;

    // シード値を変更
    //srand(static_cast<unsigned int>(time(0)));
    // 生成する乱数の範囲(0~5)
    int range = num - 2;

    // 移動可能範囲の3割を壁にする
    for (int i = 0; i < wall_num; i++) {
        int random_X = 1 + (rand() % range); // X座標に1~6の乱数
        int random_Y = 1 + (rand() % range); // Y座標に1~6の乱数

        // それぞれの座標に壁#を格納
        if (board[random_Y][random_X] == '.') {
            // まだ壁が配置されていなければ格納
            board[random_Y][random_X] = '#';
        } else {
            // すでに置かれていたらやり直し
            i--;
            continue;
        }
    }

    // ゴール不可能と判断した場合、setWallをもう一度呼ぶ
    if (!canGoal()) {
        // 盤面を初期化
        for (int y = 1; y < num-1; y++) {
            for (int x = 1; x < num-1; x++) {
                if (board[y][x] == '#') {
                    board[y][x] = '.';
                }
            }
        }

        // setWallを一度呼ぶ
        setWall();
    }
    return;
}

// プレイヤーの移動を処理する関数
void movePlayer(char direction) {
    switch (direction) {
        // 上に移動
        case 'w':
            // 移動先が壁(#)ではないなら移動する
            if (board[playerY-1][playerX] != '#') {
                // プレイヤーが居た場所を道に戻す
                board[playerY][playerX] = '.';
                // 移動先にプレイヤーを配置
                board[playerY-1][playerX] = 'P';

                // プレイヤーの座標更新
                playerX = playerX;
                playerY = playerY-1;

                // プレイヤーの行動回数を増やす
                player_move++;
            }
            break;
        // 右に移動
        case 'd':
            // 移動先が壁(#)ではないなら移動する
            if (board[playerY][playerX+1] != '#') {
                // プレイヤーが居た場所を道に戻す
                board[playerY][playerX] = '.';
                // 移動先にプレイヤーを配置
                board[playerY][playerX+1] = 'P';

                // プレイヤーの座標更新
                playerX = playerX+1;
                playerY = playerY;

                // プレイヤーの行動回数を増やす
                player_move++;
            }
            break;
        // 下に移動
        case 's':
            // 移動先が壁(#)ではないなら移動する
            if (board[playerY+1][playerX] != '#') {
                // プレイヤーが居た場所を道に戻す
                board[playerY][playerX] = '.';
                // 移動先にプレイヤーを配置
                board[playerY+1][playerX] = 'P';

                // プレイヤーの座標更新
                playerX = playerX;
                playerY = playerY+1;

                // プレイヤーの行動回数を増やす
                player_move++;
            }
            break;
        // 左に移動
        case 'a':
            // 移動先が壁(#)ではないなら移動する
            if (board[playerY][playerX-1] != '#') {
                // プレイヤーが居た場所を道に戻す
                board[playerY][playerX] = '.';
                // 移動先にプレイヤーを配置
                board[playerY][playerX-1] = 'P';

                // プレイヤーの座標更新
                playerX = playerX-1;
                playerY = playerY;

                // プレイヤーの行動回数を増やす
                player_move++;
            }
            break;       
    }
    return;
}

// PがGにたどり着いたかどうかをチェックする
bool checkGoal () {
    // PがGにたどり着いたらtrueを返す
    if (playerY == 1 && playerX == 1) {
        return true;
    }
    return false;
}

int main()
{
    int difficult; // 難易度を格納
    cout << "迷路へようこそ！難易度を半角で入力してください！\n";

    // 始まりの言葉と難易度設定
    while (1) {
        cout << "Easy: 1, Nomal: 2, Hard: 3, ESC: 中断 -> ";
        difficult = getch(); // キー入力を待つ

        // ESCキーが押されたら中断
        if (difficult == 27) {
            cout << "ゲームを中断します。\n";
            return 0;
        }

        // 入力チェック
        if (difficult != '1' && difficult != '2' && difficult != '3') {
            cout << " 正しく数字を入力してください\n";
        } else {
            break;
        }
    }

    // 難易度設定
    if (difficult == '1') {
        num = 10;
    } else if (difficult == '2') {
        num = 15;
    } else if (difficult == '3') {
        num = 20;
    }
    cout << "難易度設定完了: " << num << endl;

    // プレイヤーの位置をここで設定
    def_playerX = num - 2;
    def_playerY = num - 2;
    playerX = num - 2;
    playerY = num - 2;

    // 盤面を初期化
    for (int i = 0; i < num; i++) {
        for (int j = 0; j < num; j++) {
            board[i][j] = '#'; // 盤面を'#'で埋める
        }
    }
    // 通行可能な盤面を配置
    for (int i = 1; i < num-1; i++) {
        for (int j = 1; j < num-1; j++) {
            board[i][j] = '.'; // 盤面を'.'で埋める
        }
    }
    
    // ゴールを設定
    board[1][1] = 'G';
    // プレイヤーを設定
    board[num-2][num-2] = 'P';

    // 壁をランダム配置
    srand(static_cast<unsigned int>(time(0))); // シード値設定
    setWall();

    // キー入力のチェック用
    bool missKey = false;

    while (true) {
        // 画面を一度きれいに消去する
        system("cls");

        // 盤面を出力
        for (int i = 0; i < num; i++) {
            for (int j = 0; j < num; j++) {
                cout << board[i][j] << " "; // 盤面を出力
            }
            cout << '\n'; // 行の終わりで改行
        }

        // 不正な入力であることを知らせる
        if (missKey) cout << "無効な入力です。もう一度入力してください。\n";
        missKey = false;

        // プレイヤーの移動方向を入力
        cout << "移動する方向を入力してください (w: 上, s: 下, a: 左, d: 右, ESC: 終了) ";
        char ch = getch(); // キー入力を待つ

        // ESCキーが押されたら中断
        if (ch == 27) {
            cout << "ゲームを中断します。\n";
            break;
        }
        // 不正な入力は無視する
        if (ch != 'w' && ch != 's' && ch != 'a' && ch != 'd') {
            missKey = true;
            continue; // 無効な入力の場合は再度入力
        }

        // プレイヤーの移動を処理
        movePlayer(ch);

        // ゴールしたらゲーム終了
        if (checkGoal()) {
            // 画面を一度きれいに消去する
            system("cls");
            // 盤面を出力
            for (int i = 0; i < num; i++) {
                for (int j = 0; j < num; j++) {
                    cout << board[i][j] << " "; // 盤面を出力
                }
                cout << '\n'; // 行の終わりで改行
            }

            // 終了コメント
            cout << "ゴール！！ \n";
            cout << player_move << "手でクリアしました！";
            cout << "(最短距離は" << min_distance << "です)\n";
            if (player_move == min_distance) {
                cout << "最短ルートでクリア！！\n";
            }
            cout << "いずれかのキーで終了";
            getch();

            break;
        }
    }

    return 0;
}