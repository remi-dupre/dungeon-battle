#include <iostream>
#include <vector>
#include <sf>

using namespace std;

double from_x_to_y(double a,double b,double c,double d,double x){
    return (x-c)*(b-d)/(a-c) + d;
}

double from_y_to_x(double a,double b,double c,double d,double y){
    return (y-d)*(a-c)/(b-d) + c;
}

vector<Vector2i> cell_on_the_way(Vector2i pos1, Vector2i pos2, vector<vector<int>> map){
    Vector2<double> cent1,cent2;
    cent1.x = (double) pos1.x + 0.5; cent1.y = (double) pos1.y + 0.5;
    cent2.x = (double) pos2.x + 0.5; cent2.y = (double) pos2.y + 0.5;
    function<double> f_x = [](double x){
        return from_x_to_y(cent1.x,cent1.y,cent2.x,cent2.y,x);
    };
    function<double> f_y = [](double y){
        return from_y_to_x(cent1.x,cent1.y,cent2.x,cent2.y,y);
    };
    // Problème : faire attention à ce que pos1.x < pos2.x;
    vector<Vector2i> list_cell; //size à definir ou faire des push back...
    for(int i = pos1.x; i <= pos2.x; i++){
        int j = floor(f_x(x));
        if (map[i][j] == 0) break;
        Vector2i cell_ij;
        cell_ij.x = i; cell_ij.y = j;
        list_cell.push_back(cell_ij);
    }
    for(int j = pos1.y; j <= pos2.y; j++){
        int i = floor(f_y(y));
        if (map[i][j] == 0) break;
        Vector2i cell_ij;
        cell_ij.x = i; cell_ij.y = j;
        list_cell.push_back(cell_ij);
    }
    return list_cell;
}

int main(){
    int n, m; cin >> n >> m;
    Vector2i pos;
    vector<vector<int>> map(n, vector<int>(m,0));
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
        }
    }


    return 0;
}
