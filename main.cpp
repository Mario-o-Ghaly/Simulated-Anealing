#include <bits/stdc++.h>
#include <fstream>
using namespace std;

struct netlist {
    int num_cells, num_nets, rows, cols;
    vector<vector<int>> Floorplan;
    vector<vector<int>> nets;
    vector<pair<int, int>> placed_cells;
};

netlist parse_netlist(string filepath){
    netlist mynet;
    ifstream file;
    file.open(filepath);
    if (!file.is_open()) {
        cout << "Error: Unable to open file " << filepath << endl;
        return mynet;
    }
    string line;
    int counter = 0, cc = 0;
    while(getline(file, line)){
        string a = "";
        cc = 0;
        for (int i = 0; i < line.size(); i++) {
            // First Line --> num_cells, num_nets, rows, cols
            if (line[i] != ' ') a += line[i];
            else if (a.size() > 0) {  //the number itself
                int x = stoi(a);
                a = "";
                if(counter == 0){
                    switch (cc)
                    {
                        case 0:
                            mynet.num_cells = x;
                            mynet.placed_cells.resize(x);
                            break;

                        case 1:
                            mynet.num_nets = x;
                            mynet.nets.resize(x);
                            break;
                        
                        case 2:
                            mynet.rows = x;
                            mynet.Floorplan.resize(x);
                            break;

                        case 3:
                            // cout<<x<<"\n";
                            mynet.cols = x;
                            for(int z = 0; z < mynet.rows; ++z){
                                mynet.Floorplan[z].resize(x, -1);
                            }
                            break;
                        
                        default:
                            cout<<"Problem in parisng\n";
                            break;
                    }
                    cc++;
                }

                else if (counter > 0){
                    // if it is not the first variable representing num of cells
                    if(cc > 0){
                        // cout<<x<<" ";
                        mynet.nets[counter - 1].push_back(x);
                    }
                    cc++;
                }
            }
        }
        
        // This is in case there is no space at the end of the line
        if(a != ""){
            int x = stoi(a);
            a = "";
            if(counter == 0){
                // cout<<x<<"\n";
                mynet.cols = x;
                for(int z = 0; z < mynet.rows; ++z){
                    mynet.Floorplan[z].resize(x, -1);
                }
            }

            else {mynet.nets[counter - 1].push_back(x);}
        }
        counter++;
    }
    
    return mynet;
}


int TWL(netlist &mynet){
    //
    int min_x = INT_MAX, min_y = INT_MAX;
    int max_x = INT_MIN, max_y = INT_MIN;
    int x, y, cell, sum = 0;

    for(int i = 0; i < mynet.num_nets; ++i){
        min_x = INT_MAX, min_y = INT_MAX;
        max_x = INT_MIN, max_y = INT_MIN;
        for(int j = 0; j < mynet.nets[i].size(); ++j){
            cell = mynet.nets[i][j];
            //placed_cells is index based, so we swap them
            tie(y, x) = mynet.placed_cells[cell];
            if(min_x > x){
                min_x = x;
            }
            if(min_y > y){
                min_y = y;
            }
            if(max_x < x){
                max_x = x;
            }
            if(max_y < y){
                max_y = y;
            }
        }
        sum += (max_x - min_x + max_y - min_y);
        // cout<<max_x<<" "<<min_x<<" "<<max_y<<" "<<min_y<<"\n";
    }

    return sum;
}