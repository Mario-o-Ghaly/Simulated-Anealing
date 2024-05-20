
#include<iostream>
#include<string>
#include<fstream>
#include<algorithm>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include <chrono>
#include<unordered_map>
#include<iomanip>
#include<random>
#include<set>
#include <vector>
#include <sstream>
#include <climits>
#include <tuple>
#include "CImg.h"
using namespace std;
using namespace cimg_library;


struct netlist {
    int num_cells, num_nets, rows, cols;
    vector<vector<int>> Floorplan;
    vector<vector<int>> nets;
    vector<pair<int, int>> placed_cells;
    vector<vector<int>> cell_nets;
};
void save_img(const std::string& saved_fname, netlist &mynet)
{
const int cellSize = 30;
const int imageSizeX = cellSize * mynet.cols;
const int imageSizeY = cellSize * mynet.rows;

CImg<unsigned char> img(imageSizeX, imageSizeY, 1, 1, 255);

for (int i = 0; i < mynet.rows; i++) {
for (int j = 0; j < mynet.cols; j++) {
int cell = mynet.Floorplan[i][j];
unsigned char color[3] = {255, 255, 255}; 

if (cell != -1) {
color[0] = color[1] = color[2] = 0; // color the occupied cell with black
}

img.draw_rectangle(j * cellSize, i * cellSize, (j + 1) * cellSize - 1, (i + 1) * cellSize - 1, color); // color all cells with white initially
}
}

img.save_bmp(saved_fname.c_str());
}

netlist parse_netlist(string filepath){
    netlist mynet;
    ifstream file;
    file.open(filepath);
    if (!file.is_open()) {
        cout << "Error: Unable to open file " << filepath << endl;
        exit(0);
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
                            mynet.cell_nets.resize(x);
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
                        mynet.cell_nets[x].push_back(counter - 1);
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

            else {mynet.nets[counter - 1].push_back(x); mynet.cell_nets[x].push_back(counter - 1);}
        }
        counter++;
    }
    
    return mynet;
}


void random_placement(netlist &mynet){
    vector<int> cells;
    for(int i = 0; i < mynet.rows*mynet.cols; ++i){
        cells.push_back(i);
    }
    random_shuffle(cells.begin(), cells.end());
    for(int i = 0; i < mynet.rows; ++i){
        for(int j = 0; j < mynet.cols; ++j){
            if(cells[i*mynet.cols + j] < mynet.num_cells){
                mynet.Floorplan[i][j] = cells[i*mynet.cols + j];
                mynet.placed_cells[cells[i*mynet.cols + j]] = {i, j};
            }
            else mynet.Floorplan[i][j] = -1;
        }
    }
        save_img("./images/image_" + to_string(0) + ".jpg", mynet);

}


void BinaryGrid(const netlist &mynet){
    for(int i = 0; i < mynet.rows; ++i){
        for(int j = 0; j < mynet.cols; ++j){
            if(mynet.Floorplan[i][j] == -1)
                cout<<1;
            else cout<<0;
        }
        cout<<"\n";
    }
}


int TWL(netlist &mynet){
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
    }

    return sum;
}


int partial_TWL(const netlist &oldnet, const netlist &newnet, int c1, int c2){
    int min_x = INT_MAX, min_y = INT_MAX;
    int max_x = INT_MIN, max_y = INT_MIN;
    int x, y, cell, sum = 0;

    int old_min_x = INT_MAX, old_min_y = INT_MAX;
    int old_max_x = INT_MIN, old_max_y = INT_MIN;
    int old_x, old_y, old_sum = 0;

    vector<bool> updated(newnet.num_nets, false);
    for(int i = 0; i < newnet.cell_nets[c1].size(); ++i){
        int net = newnet.cell_nets[c1][i];
        if(updated[net]) continue;
        updated[net] = true;

        min_x = INT_MAX; min_y = INT_MAX;
        max_x = INT_MIN; max_y = INT_MIN;
        old_min_x = INT_MAX; old_min_y = INT_MAX;
        old_max_x = INT_MIN; old_max_y = INT_MIN;
        
        for(int j = 0; j < newnet.nets[net].size(); ++j){
            cell = newnet.nets[net][j];
            //placed_cells is index based, so we swap them
            tie(y, x) = newnet.placed_cells[cell];
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

            //old
            tie(old_y, old_x) = oldnet.placed_cells[cell];
            if(old_min_x > old_x){
                old_min_x = old_x;
            }
            if(old_min_y > old_y){
                old_min_y = old_y;
            }
            if(old_max_x < old_x){
                old_max_x = old_x;
            }
            if(old_max_y < old_y){
                old_max_y = old_y;
            }
        }
        sum += (max_x - min_x + max_y - min_y);
        old_sum += (old_max_x - old_min_x + old_max_y - old_min_y);
    }

    if(c2 != -1){
        for(int i = 0; i < newnet.cell_nets[c2].size(); ++i){
            int net = newnet.cell_nets[c2][i];
            if(updated[net]) continue;
            updated[net] = true;

            min_x = INT_MAX; min_y = INT_MAX;
            max_x = INT_MIN; max_y = INT_MIN;
            old_min_x = INT_MAX; old_min_y = INT_MAX;
            old_max_x = INT_MIN; old_max_y = INT_MIN;
            
            for(int j = 0; j < newnet.nets[net].size(); ++j){
                cell = newnet.nets[net][j];
                //placed_cells is index based, so we swap them
                tie(y, x) = newnet.placed_cells[cell];
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

                //old
                tie(old_y, old_x) = oldnet.placed_cells[cell];
                if(old_min_x > old_x){
                    old_min_x = old_x;
                }
                if(old_min_y > old_y){
                    old_min_y = old_y;
                }
                if(old_max_x < old_x){
                    old_max_x = old_x;
                }
                if(old_max_y < old_y){
                    old_max_y = old_y;
                }
            }
            sum += (max_x - min_x + max_y - min_y);
            old_sum += (old_max_x - old_min_x + old_max_y - old_min_y);
        }
    }

    return sum - old_sum;
}


void SA(netlist &mynet, double cooling_rate){

    double initial_cost = TWL(mynet);
    cout<<"Total wire length = "<<initial_cost<<"\n";
    int final_twl = initial_cost;
    double T = 500 * initial_cost;
    double final_T = 5*0.00001*initial_cost / mynet.num_nets;
    double L1 = initial_cost, L2;
    double prob, diff, delta_L;
    netlist temp = mynet;
    int img=1;

    int cell1, cell2, x1, y1, x2, y2;
    while(T > final_T){
        for(int i = 0; i < 20*mynet.num_cells; ++i){
            // temp. = mynet;
            //swap logic
            cell1 = rand() % temp.num_cells;
            tie(x1, y1) = temp.placed_cells[cell1];

            x2 = rand() % temp.rows;
            y2 = rand() % temp.cols;
            cell2 = temp.Floorplan[x2][y2];

            while(true){
                if(x1 == x2 && y1 == y2){
                    x2 = rand() % temp.rows;
                    y2 = rand() % temp.cols;
                    cell2 = temp.Floorplan[x2][y2];
                }
                else{
                    temp.Floorplan[x1][y1] = cell2;
                    temp.Floorplan[x2][y2] = cell1;

                    temp.placed_cells[cell1] = {x2, y2};
                    if(cell2 != -1)
                        temp.placed_cells[cell2] = {x1, y1}; 

                    break;
                }
            }

            delta_L = partial_TWL(mynet, temp, cell1, cell2);
            L2 = L1 + delta_L;
            // L2 = TWL(temp);
            // delta_L = L2 - L1;
            double random_number = static_cast < double > (rand()) / RAND_MAX;
            double e = 1 - exp( (-delta_L) / T);
            bool replace = (L2 <= L1 || e < random_number); 

            if(replace){
                // mynet = temp;
                mynet.Floorplan[x1][y1] = cell2;
                mynet.Floorplan[x2][y2] = cell1;

                mynet.placed_cells[cell1] = {x2, y2};
                if(cell2 != -1)
                    mynet.placed_cells[cell2] = {x1, y1}; 

                L1 = L2;
                final_twl += delta_L;
            }
            else{
                temp.Floorplan[x1][y1] = cell1;
                temp.Floorplan[x2][y2] = cell2;

                temp.placed_cells[cell1] = {x1, y1};
                if(cell2 != -1)
                    temp.placed_cells[cell2] = {x2, y2};                 
            }
        }

        T *= cooling_rate;
                save_img("./images/image_" + to_string(img) + ".jpg", mynet);
                        img++;


    }

    cout<<"\n---After SA---\n";
    BinaryGrid(mynet);
    cout<<" Placement: \n";
    for(int i = 0; i < mynet.rows; ++i){
        for(int j = 0; j < mynet.cols; ++j){
            if(mynet.Floorplan[i][j] == -1)
                cout<<setw(3)<<" -- ";
            else cout<<setw(3)<<mynet.Floorplan[i][j]<<" ";}
        cout<<"\n";
    }  
    cout<<"Total wire length = "<<final_twl<<"\n";
  
}



int main(){
    string filepath = "d0.txt";
    //cin>>filepath;
    netlist mynet = parse_netlist(filepath);
    auto start = chrono::high_resolution_clock::now();
    random_placement(mynet);
    cout<<"\n---Before SA---\n";
    BinaryGrid(mynet);
    cout<<" Placement: \n";
    for(int i = 0; i < mynet.rows; ++i){
        for(int j = 0; j < mynet.cols; ++j){
            if(mynet.Floorplan[i][j] == -1)
                cout<<setw(3)<<" -- ";
            else cout<<setw(3)<<mynet.Floorplan[i][j]<<" ";
        }
    cout<<"\n";
    }
    SA(mynet, 0.95);
    
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Time taken by function: " << duration.count() << " seconds" << endl;
}