#include <iostream>
#include <vector>
#include <math.h>
#include <SFML/System/Vector2.hpp>

using namespace std;

double from_x_to_y(double a,double b,double c,double d,double x){
    return (x-c)*(b-d)/(a-c) + d;
}

double from_y_to_x(double a,double b,double c,double d,double y){
    return (y-d)*(a-c)/(b-d) + c;
}

bool can_be_seen(sf::Vector2i pos1, sf::Vector2i pos2, vector<vector<int>> map){
    //cout << "I'm in can be seen" << endl;

    int pos_min_x = min(pos1.x,pos2.x);
    int pos_max_x = max(pos1.x,pos2.x);
    int pos_min_y = min(pos1.y,pos2.y);
    int pos_max_y = max(pos1.y,pos2.y);


    if (pos1.x == pos2.x){
        //cout << "I'm in pos1.x == pos2.x \n";
        for(int j = pos_min_y; j <= pos_max_y; j++){
            int i = pos1.x;
            if (map[i][j] == 0) break; //Cell visible.
            last_y = j;
        }
        return pos_max_y ==last_y;
    }
    if (pos1.y == pos2.y){
        //cout << "I'm in pos1.y == pos2.y \n";
        for(int i = pos_min_x; i <= pos_max_x; i++){
            int j = pos1.y;
            if (map[i][j] == 0) break;
            last_x = i;
        }
        return pos_max_x == last_x;
    }

    auto f_x = [pos1,pos2](double x){
    return from_x_to_y((double) pos1.x + 0.5, (double) pos1.y + 0.5,
    (double) pos2.x + 0.5, (double) pos2.y + 0.5,x);
    };
    auto f_y = [pos1,pos2](double y){
    return from_y_to_x((double) pos1.x + 0.5, (double) pos1.y + 0.5,
    (double) pos2.x + 0.5, (double) pos2.y + 0.5,y);
    };


    int last_x = pos_min_x;
    int last_y = pos_min_y;
    for(int i = pos_min_x+1; i <= pos_max_x; i++){
        int j = floor(f_x(i));
        //cout << i << " f_x(i) " << f_x(i) << " " << j << endl;
        //cout << "map[i][j] " << map[i][j] << " map[i-1][j+1] " << map[i-1][j+1] << endl;
        if (f_x(i) != j && map[i][j] == 0) {
            cout << "f_x(i) != j \n";
            last_x = -1;
            break;
        }
        if (f_x(i) == j
        && map[i][j] == 0
        && i+1 < map.size()
        && j-1 > 0
        && map[i+1][j-1] == 0){ //ATENTION OUT OUT OF BAND
            cout << "wrong config f_y \n";
            last_x = -1;
            break;
        }
        last_x = i;
    }

    for(int j = pos_min_y+1; j <= pos_max_y; j++){
        int i = floor(f_y(j));
        cout << j << " f_y(j) " << f_y(j) << " " << i << endl;
        if (f_y(j) != i && map[i][j] == 0) {
            last_y = -1;
            break;
        }
        if (f_y(j) == i
        && map[i][j] == 0
        && j+1 < map[0].size()
        && i-1 > 0
        && map[i-1][j+1]){ //ATENTION OUT OF BAND
            cout << "wrong config f_y \n";
            last_y = -1;
            break;
        }
        last_y = j;
    }
    return (pos_max_y == last_y)&&(pos_max_x == last_x);
}



/* ******** HANDELING MY INPUT FORMAT ******** */


int main(){
    int n, m; cin >> n >> m;
    sf::Vector2i pos;
    vector<vector<int>> map(n, vector<int>(m,-1));
    //Saving the map
    for(int i = 0; i < n; i++){
        string s;
        cin >> s;
        for(int j = 0; j < m; j++){
            if(s[j] == 'o'){
                pos.x = i; pos.y = j;
                map[i][j] = 1;
            }
            if(s[j] == '#'){
                map[i][j] = 1;
            }
            if(s[j] == '/'){
                map[i][j] = 0;
            }
        }
    }
    // Launching the computation.
    //cout << "done for the map \n";
    sf::Vector2i pos1,pos2;
    int N;
    cin  >> N;
    for(int i = 0; i < N; i++){
        cin >> pos1.x >> pos1.y;
        cin >> pos2.x >> pos2.y;
        cout << can_be_seen(pos1,pos2,map) << endl;
    }
    return 0;
}
