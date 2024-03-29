#include <iostream>
#include <vector>
#include <map>
#include <set>

#define UP 'u'
#define DOWN 'd'
#define LEFT 'l'
#define RIGHT 'r'

#define CRYSTAL '*'
#define EMPTY ' '
#define WALL '#'
#define TAKEN 'X'
#define VISITED '!'

#define MIR_SLASH '/'
#define MIR_BACKSLASH '\\'

using namespace std;
int crystals, width, height;

struct action {
    char mark;
    char direction;
    bool drop_mirror;
};


map<char, vector<action> > actions;

void print_map(vector<vector<char> > M) {
    for (int i = 0; i < M.size(); ++i) {
        for (int j = 0; j < M[0].size(); ++j)
            if (M[i][j] != WALL and M[i][j] != MIR_SLASH and M[i][j] != MIR_BACKSLASH and M[i][j] != CRYSTAL)
                cout << EMPTY;
            else
                cout << M[i][j];
        cout << endl;
    }
}

void initialize_actions() {
    action x, y, z;
    vector<action> up_act, down_act, left_act, right_act;

    x.mark = EMPTY;
    y.mark = MIR_SLASH;
    z.mark = MIR_BACKSLASH;

    x.drop_mirror = false;
    y.drop_mirror = z.drop_mirror = true;

    //UP
    x.direction = UP;
    y.direction = RIGHT;
    z.direction = LEFT;

    swap(y, z);
    up_act.push_back(x);
    up_act.push_back(y);
    up_act.push_back(z);
    actions[UP] = up_act;

    swap(y, z);
    //DOWN
    x.direction = DOWN;
    y.direction = LEFT;
    z.direction = RIGHT;

    swap(y, z);
    down_act.push_back(x);
    down_act.push_back(y);
    down_act.push_back(z);
    actions[DOWN] = down_act;
    swap(y, z);

    //LEFT
    x.direction = LEFT;
    y.direction = DOWN;
    z.direction = UP;

    swap(y, z);

    left_act.push_back(x);
    left_act.push_back(y);
    left_act.push_back(z);
    actions[LEFT] = left_act;
    swap(y, z);

    //RIGHT
    x.direction = RIGHT;
    y.direction = UP;
    z.direction = DOWN;
    swap(y, z);

    right_act.push_back(x);
    right_act.push_back(y);
    right_act.push_back(z);
    actions[RIGHT] = right_act;
    swap(y, z);

}

void move(short int old_x, short int old_y, short int &new_x, short int &new_y, char direction) {
    new_x = old_x, new_y = old_y;

    if (direction == UP) new_y = (short) (old_y - 1);
    else if (direction == DOWN) new_y = (short) (old_y + 1);
    else if (direction == LEFT) new_x = (short) (old_x - 1);
    else if (direction == RIGHT) new_x = (short) (old_x + 1);
}


bool solve(vector<vector<char> > &map, short int mirrors, short int covered, char direction, short int x, short int y,
           set<string> &history) {

    //went beyond the game map
    if (x < 0 or x == width or y < 0 or y == height) return false;

    //cannot use more mirrors than specified
    if (mirrors < 0) return false;

    //hit a wall without covering all the crystals
    if (map[y][x] == WALL) return false;

    //declare variables for the new coordinates
    short int new_x, new_y;
    bool result;

    //increment if covered a crystal and move further
    if (map[y][x] == CRYSTAL) {
        covered++;

        //covered all the crystals, can return
        if (covered == crystals) return true;

        move(x, y, new_x, new_y, direction);
        map[y][x] = TAKEN;

        result = solve(map, mirrors, covered, direction, new_x, new_y, history);

        map[y][x] = CRYSTAL;
        return result;
    }

    if (map[y][x] == TAKEN or map[y][x] == VISITED) {
        move(x, y, new_x, new_y, direction);

        result = solve(map, mirrors, covered, direction, new_x, new_y, history);

        return result;
    }

    if (map[y][x] == EMPTY) {
        for (int i = 0; i < 3; i++) {
            action next_action = actions[direction][i];

            char mark = next_action.mark;
            short int mirrors_used = next_action.drop_mirror;
            char new_direction = next_action.direction;

            move(x, y, new_x, new_y, new_direction);

            if (mark == EMPTY) mark = VISITED;
            map[y][x] = mark;
            result = solve(map, mirrors - mirrors_used, covered, new_direction, new_x, new_y, history);
            if (result) return true;

            map[y][x] = EMPTY;
        }
    }

    return false;
}


int main() {
    short int mirrors;
    vector<vector<char> > state;
    cin >> height >> width;
    cin >> mirrors;

    for (int i = 0; i < height; ++i) {
        vector<char> row;

        for (int j = 0; j < width; ++j) {
            char segment = '\n';
            while (segment == '\n') segment = (char) cin.get();
            if (segment == '*') crystals++;
            row.push_back(segment);
        }
        state.push_back(row);
    }

    initialize_actions();
    set<string> S;
    solve(state, mirrors, 0, RIGHT, 0, 1, S);
    cout << height << " " << width << endl;
    cout << mirrors << endl;
    print_map(state);
    return 0;
}