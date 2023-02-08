#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <utility>
#include <algorithm>
#include <math.h>

using namespace std;

#define vvi vector<vector<int>>
#define pvvi pair<int,pair<vvi,pair<int,pair<int,int>>>>

int comparar(vvi& final, vvi& x){
    int diff = 0;

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(final[i][j] != x[i][j]) diff+=1;
        }
    }

    return diff;
}

int absolutaDiferencia(vvi& x, vector<pair<int,int>>&coordValues){
    int sum = 0;

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            int num = x[i][j];
            sum = sum + abs(i-coordValues[num].first) + abs(j-coordValues[num].second);
        }
    }

    return sum;
}

pair<int,int>findCoord(vvi& x){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(x[i][j] == 0) return make_pair(i,j);
        }
    }

    return {0,0};
}

bool pushQueue(vvi&x, vvi&final, map<vvi,string>&closed, int curr, priority_queue<pvvi, vector<pvvi>, greater<pvvi>>&open, int i, int j, vector<pair<int,int>>&coordValues){
    if(closed[x].empty()){
        //int cost = comparar(final,x) + curr;
        int cost = absolutaDiferencia(x,coordValues) + curr;
        open.push(make_pair(cost,make_pair(x,make_pair(curr,make_pair(i,j)))));
        return true;
    }

    return false;
}

string formato(string s){
    int n = s.size();
    string nuevoFormato;

    for(int i = 1; i < n-1; i++){
        nuevoFormato.push_back(s[i]);
        nuevoFormato.push_back(',');
    }
    nuevoFormato.push_back(s[n-1]);

    return nuevoFormato;
}

string Astar(vvi&inicial, vvi&final, map<vvi,string>&closed, vector<pair<int,int>>&coordValues){
    vvi x = inicial;
    priority_queue<pvvi, vector<pvvi>, greater<pvvi>>open;
    pair<int,int>p = findCoord(inicial);
    open.push(make_pair(0,make_pair(x,make_pair(0,p))));

    while(x != final){
        x = open.top().second.first;
        int curr = open.top().second.second.first+1;
        int i = open.top().second.second.second.first;
        int j = open.top().second.second.second.second;
        string path = closed[x];
        open.pop();

        if(i != 0){
            swap(x[i][j], x[i-1][j]);
            if(pushQueue(x,final,closed,curr,open,i-1,j,coordValues)) closed[x] = path+"U";
            swap(x[i][j], x[i-1][j]);
        }
        if(i != 3){
            swap(x[i][j], x[i+1][j]);
            if(pushQueue(x,final,closed,curr,open,i+1,j,coordValues)) closed[x] = path+"D";
            swap(x[i][j], x[i+1][j]);
        }
        if(j != 0){
            swap(x[i][j], x[i][j-1]);
            if(pushQueue(x,final,closed,curr,open,i,j-1,coordValues)) closed[x] = path+"L";
            swap(x[i][j], x[i][j-1]);
        }
        if(j != 3){
            swap(x[i][j], x[i][j+1]);
            if(pushQueue(x,final,closed,curr,open,i,j+1,coordValues)) closed[x] = path+"R";
            swap(x[i][j], x[i][j+1]);
        }
    }

    return closed[x];
}

int main(){
    ifstream filein("Datos.txt");
    vvi inicial;
    vvi final;
    map<vvi, string>closed;
    vector<pair<int,int>>coordValues(16,{0,0});
    int divisor = 0;

    if(filein.is_open()){
        do{
            string text;
            filein >> text;

            int n = text.size(); 
            
            string tempNum = "";
            vector<int>aux;

            for(int i = 0; i < n; i++){
                if(text[i] == ','){
                    aux.push_back(stoi(tempNum));
                    tempNum.clear();
                }
                else{
                    tempNum.push_back(text[i]);
                }
            }

            if(tempNum.size() == 0) continue;

            aux.push_back(stoi(tempNum));

            if(divisor < 4) inicial.push_back(aux);
            else final.push_back(aux);

            divisor+=1;
        } while(!filein.fail());
        filein.close();
    }
    else{
        cout << "No se encontro el archivo" << endl;
    }

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            coordValues[final[i][j]] = make_pair(i,j);
        }
    }

    closed[inicial] = "X";
    cout << formato(Astar(inicial,final,closed,coordValues)) << endl;

    return 0;
}
