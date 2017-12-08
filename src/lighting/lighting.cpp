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
/*auto f_x = [pos1,pos2](double x){
return from_x_to_y((double) pos1.x + 0.5, (double) pos1.y + 0.5,
(double) pos2.x + 0.5, (double) pos2.y + 0.5,x);
};
auto f_y = [pos1,pos2](double y){
return from_y_to_x((double) pos1.x + 0.5, (double) pos1.y + 0.5,
(double) pos2.x + 0.5, (double) pos2.y + 0.5,y);
};
cout << (f_x((double) pos1.x + 0.5)) << endl;
cout << (f_y((double) pos1.y + 0.5)) << endl;
cout << (f_x((double) pos2.x + 0.5)) << endl;
cout << (f_y((double) pos2.y + 0.5)) << endl;*/

vector<sf::Vector2i> cell_on_the_way(sf::Vector2i pos1, sf::Vector2i pos2, vector<vector<int>> map){
    cout << "pouet" << '\n';

    // Problème : faire attention à ce que pos1.x < pos2.x;
    vector<sf::Vector2i> list_cell; //size à definir ou faire des push back...
    if (pos1.x == pos2.x){
        for(int j = pos1.y; j <= pos2.y; j++){
            int i = pos1.x;
            if (map[i][j] == 0) break;
            sf::Vector2i cell_ij;
            cell_ij.x = i; cell_ij.y = j;
            list_cell.push_back(cell_ij);
        }
        return list_cell;
    }
    if (pos1.y == pos2.y){
        for(int i = pos1.x; i <= pos2.x; i++){
            int j = pos1.y;
            if (map[i][j] == 0) break;
            sf::Vector2i cell_ij;
            cell_ij.x = i; cell_ij.y = j;
            list_cell.push_back(cell_ij);
        }
        return list_cell;
    }

    auto f_x = [pos1,pos2](double x){
    return from_x_to_y((double) pos1.x + 0.5, (double) pos1.y + 0.5,
    (double) pos2.x + 0.5, (double) pos2.y + 0.5,x);
    };
    auto f_y = [pos1,pos2](double y){
    return from_y_to_x((double) pos1.x + 0.5, (double) pos1.y + 0.5,
    (double) pos2.x + 0.5, (double) pos2.y + 0.5,y);
    };

    cout << (f_x((double) pos1.x + 0.5)) << endl;
    cout << (f_y((double) pos1.y + 0.5)) << endl;
    cout << (f_x((double) pos2.x + 0.5)) << endl;
    cout << (f_y((double) pos2.y + 0.5)) << endl;

    for(int i = pos1.x; i <= pos2.x; i++){
        int j = floor(f_x(i));
        if (map[i][j] == 0) break;
        sf::Vector2i cell_ij;
        cell_ij.x = i; cell_ij.y = j;
        list_cell.push_back(cell_ij);
    }
    for(int j = pos1.y+1; j <= pos2.y; j++){
        int i = floor(f_y(j));
        cout << j << endl;
        cout << i << " " << f_y(j) << endl;
        if (map[i][j-1] == 0) break;
        //cout << "ok" << endl;
        sf::Vector2i cell_ij;
        cell_ij.x = i; cell_ij.y = j;
        list_cell.push_back(cell_ij);
    }
    return list_cell;

}

int main(){
    int n, m; cin >> n >> m;
    sf::Vector2i pos;
    vector<vector<int>> map(n, vector<int>(m,-1));
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

    sf::Vector2i pos1,pos2;
    pos1.x = 0; pos1.y = 0;
    pos2.x = 1; pos2.y = 2;
    vector<sf::Vector2i> list_cell = cell_on_the_way(pos1,pos2,map);
    for(int i = 0; i < list_cell.size() ; i++)
        cout << list_cell[i].x << " " << list_cell[i].y << endl;
    return 0;
}
