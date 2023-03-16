//950181F63D0A883F183EC0A5CC67B19928FE896A

#include <getopt.h>
#include <string>
#include <vector>
#include <algorithm>
#include <deque>
#include <iostream>
#include "backship.h"
using namespace std;

//first get the option policy
void backship::getmode(int argc, char * argv[]) {
    // These are used with getopt_long()
    opterr = false; // Let us handle all error output for command line options
    int cho;
    int opt_index = 0;
    option long_options[] = {
        {"stack", no_argument, nullptr, 's'},
        {"queue",no_argument, nullptr, 'q'},
        {"output",required_argument, nullptr, 'o'},
        {"help", no_argument,       nullptr, 'h'},
        { nullptr, 0,                 nullptr, '\0' }
    };
    while ((cho = getopt_long(argc, argv, "sqo:h", long_options, &opt_index)) != -1) {
        switch (cho) {
        case 'h':
            cout << "Help! This project need chose stacks or queue.\n"; 
            exit(0);

        case 's':
            num++;
            policy = 's';
            break;

        case 'q':
            num++;
            policy = 'q';
            break;

        case 'o':
            if(*optarg == 'M'){ //check m / l
                out_M = true;
                break;
            } else if(*optarg == 'L'){
                out_M = false;
                break;
            } else {
                cerr << "Invalid output mode specified" << endl;
            }

        default:
            cerr << "Unknown command line option" << endl;
            exit(1);
        } // switch
    } // while
    if (num > 1) {
        cerr << "Multiple routing modes specified" << endl;
        exit(1);
    }
    if (num == 0) {
        cerr << "No routing mode specified" << endl;
        exit(1);
    }
}
//read the levels data information from the input file
void backship::read_data() {
    char form;// the form of the input whether map or list;
    cin >> form;
    string detail; //the line of the chars
    uint32_t row = 0; // the number of the row number to cal the level
    if (form == 'M') {
        cin >> num_lev; //level number
        cin >> side; //side of square
        getline(cin, detail); //read the first line
        map.resize(num_lev, vector<vector<point>>(side, vector<point>(side)));//side resize
        while(getline(cin,detail)){
            //cout << com <<endl;
            if (detail.substr(0,1) == "/") {
                continue;
            } else {
                if(row < (side * num_lev)) {
                    for(uint32_t i = 0;i < side;i++) {
                        if(detail[i] != '.' && detail[i] != 'S' && detail[i] != 'H' && 
                            detail[i] != 'E' && detail[i] != '#') {
                            cerr << "Invalid map character" << endl;
                            cerr << "  Invalid character is: " << detail[i] << endl;
                            exit(1);
                        } else {
                            map[row/side][row%side][i].def = detail[i];
                                if (detail[i] == 'H') {
                                    ph.lev = row/side;
                                    ph.col = i;
                                    ph.row = row%side;
                                } else if(detail[i] == 'S') {
                                    ps.lev = row/side;
                                    ps.col = i;
                                    ps.row = row%side;
                                }
                        }//read a line
                    }
                    row++;//record the num of row 
                }
            }
            //map_out = map; //may be use pass by reference
            }
        } else if(form == 'L') {
        uint32_t col;
        uint32_t level;
        char item;
        cin >> num_lev;
        cin >> side;
        map.resize(num_lev, vector<vector<point>>(side, vector<point>(side)));
        char first;
        while(cin >> first) { //cin the first "("
            if(first == '/') {
                getline(cin,detail);
            } else {
                cin >> level >> item >> row >> item >> col >> item >> item;
                if(level >= num_lev) { // level < 0?
                    cerr << "Invalid level number" <<endl;
                    exit(1);
                }
                if(row >= side) {
                    cerr << "Invalid row number" <<endl;
                    exit(1);
                }
                if(col >= side) {
                    cerr << "Invalid column number" <<endl;
                    exit(1);
                }
                if(item != 'H' && item != 'S' && item != '.' && item != 'E' && 
                item != '#') {
                    cerr << "Unknown list character" <<endl;
                    exit(1);
                }
                if(item != '.') {
                   map[level][row][col].def = item;
                }
                if (item == 'S') {
                    ps.lev = level;
                    ps.row = row;
                    ps.col = col;
                } else if (item == 'H') {
                    ph.lev = level;
                    ph.row = row;
                    ph.col = col;
                }
                    cin >> item;// ")"
                }
            }
        }
       
}
void backship::equal(element &x, const element y) { //make x = y
    x.lev = y.lev;
    x.col = y.col;
    x.row = y.row;
}

void backship::search() {
        equal(curr, ps);
        map[ps.lev][ps.row][ps.col].direct = 'y'; //mark discovered.
        path.push_back(curr);
        while(!path.empty()) {
            if(!path.empty()) {
                if (policy == 's') {
                    curr = path.back();
                    path.pop_back();
                } else if(policy == 'q') {
                    curr = path.front();
                    path.pop_front();
                } else {
                    result = false;
                    break;
                }
            }
            //n row -1
            if(curr.row > 0 && map[curr.lev][curr.row - 1][curr.col].direct == 'A'
                && map[curr.lev][curr.row - 1][curr.col].def != '#') {
                temp_n = curr.row - 1;
                current = map[curr.lev][temp_n][curr.col].def;
                equal(temp, curr);
                temp.row = temp_n;
                path.push_back(temp);
                map[temp.lev][temp.row][temp.col].direct = 'n';
                if(current == 'H') {
                    result = true;
                    final = 'n';
                    break;

                }
            }

            //e col +1
            if((curr.col + 1) < side && map[curr.lev][curr.row][curr.col + 1].direct == 'A'
                && map[curr.lev][curr.row][curr.col + 1].def != '#') {
                temp_e = curr.col + 1;
                current = map[curr.lev][curr.row][temp_e].def; 
                equal(temp, curr);
                temp.col = temp_e;
                path.push_back(temp);
                map[temp.lev][temp.row][temp.col].direct = 'e';
                if(map[temp.lev][temp.row][temp.col].def == 'H') {
                    result = true;
                    final = 'e';
                    break;
                }
            }

            //s row + 1
            if((curr.row + 1) < side && map[curr.lev][curr.row + 1][curr.col].direct == 'A'
                && map[curr.lev][curr.row + 1][curr.col].def != '#') {
                temp_s = curr.row + 1;
                current = map[curr.lev][temp_s][curr.col].def;
                equal(temp, curr);
                temp.row = temp_s;
                path.push_back(temp);
                map[curr.lev][temp_s][curr.col].direct = 's';
                if(current == 'H') {
                    result = true;
                    final = 's';
                    break;
                }
            }

            //w col - 1
            if(curr.col > 0 && map[curr.lev][curr.row][curr.col - 1].direct == 'A'
                && map[curr.lev][curr.row][curr.col - 1].def != '#') {
                temp_w = curr.col - 1;
                current = map[curr.lev][curr.row][temp_w].def;
                equal(temp, curr);
                temp.col = temp_w;
                path.push_back(temp);
                map[curr.lev][curr.row][temp_w].direct = 'w';
                if(current == 'H') {
                    result = true;
                    final = 'w';
                    break;

                }
            }
            if(map[curr.lev][curr.row][curr.col].def == 'E') {
                for(uint32_t i = 0; i < num_lev; i++) {
                    if(map[i][curr.row][curr.col].def == 'E' && map[i][curr.row][curr.col].direct == 'A') {
                        map[i][curr.row][curr.col].direct = static_cast<char>(i + '0'); //the trans level
                        map[i][curr.row][curr.col].number = static_cast<uint8_t>(curr.lev);//which level to go
                        equal(temp, curr);
                        temp.lev = i;
                        path.push_back(temp);
                    }
                }
            }

        }
}


void backship::backtracing(){
    element curr1;
    equal(curr1, ph);
    //back_track.push(final);
    tempback = final;
    while(curr1.lev != ps.lev || curr1.row != ps.row || curr1.col != ps.col) {
        if(tempback == 'n') {
            tempback = map[curr1.lev][curr1.row + 1][curr1.col].direct;
            map[curr1.lev][curr1.row + 1][curr1.col].def = 'n';
            curr1.row = curr1.row + 1;
        } else if (tempback == 'e') {
            tempback = map[curr1.lev][curr1.row][curr1.col -1].direct;
            map[curr1.lev][curr1.row][curr1.col -1].def = 'e';
            curr1.col = curr1.col - 1;
        } else if (tempback == 's') {
            tempback = map[curr1.lev][curr1.row - 1][curr1.col].direct;
            map[curr1.lev][curr1.row - 1][curr1.col].def = 's';
            curr1.row  = curr1.row -1;
        } else if(tempback == 'w') {
            tempback = map[curr1.lev][curr1.row][curr1.col + 1].direct;
            map[curr1.lev][curr1.row][curr1.col + 1].def = 'w';
            curr1.col = curr1.col + 1;
        } else if(isdigit(tempback)) {
            //level = static_cast<uint32_t>(tempback - '0');
            level = map[curr1.lev][curr1.row][curr1.col].number;//the next level to go
            tempback = map[level][curr1.row][curr1.col].direct;
            //map[level][curr1.row][curr1.col].number = curr1.lev;
            //back_track.pop();
            //back_track.push(static_cast<char>(curr1.lev + '0'));
            map[level][curr1.row][curr1.col].def = map[curr1.lev][curr1.row][curr1.col].direct;
            curr1.lev = level;
        }
        //back_track.push(tempback);
    }
}
void backship::print() {
    if(out_M){
        cout << "Start in level " << ps.lev << ", row " << ps.row 
        << ", column " << ps.col << "\n";
        for(uint32_t n = 0; n < num_lev; n++) {
            cout << "//level " << n << '\n';
            for(uint32_t i = 0; i < side; i++) {
                for(uint32_t j = 0; j < side; j++) {
                    cout << map[n][i][j].def;
                }
                cout << '\n';
            }
        }
    } else if (!result) {
        cout << "//path taken\n";
    } else {
        cout << "//path taken\n";
        //back_track.pop();
        //char direct = back_track.top();
        equal(out,ps);
        char direct = map[out.lev][out.row][out.col].def;
        //cout << "(" << ps.lev << "," << ps.row << "," << ps.col << "," << direct << ")\n";
        //while(!back_track.empty()) {
        while(out.lev != ph.lev || out.row != ph.row || out.col != ph.col){
            cout << "(" << out.lev << "," << out.row << "," << out.col << ","<< direct << ")\n";
            if(direct == 'n') {
               out.row = out.row - 1;
            } else if(direct == 'e') {
               out.col = out.col + 1;
            } else if(direct == 's') {
               out.row = out.row + 1;
            } else if(direct == 'w') {
               out.col = out.col - 1;
            } else if(isdigit(direct)) {
                out.lev = static_cast<uint32_t>(direct - '0');
                //out.lev = map[out.lev][out.row][out.col].direct;
            }
            direct = map[out.lev][out.row][out.col].def;
            //cout << "(" << out.lev << "," << out.row << "," << out.col << ","<< direct << ")\n";
            //back_track.pop();
            //if(!back_track.empty()) {
            //    direct = back_track.top();
            //    cout << "(" << out.lev << "," << out.row << "," << out.col << ","<< direct << ")\n";
            //}
        }

    }
}

int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(false);
    backship sophy;
    sophy.getmode(argc, argv);
    sophy.read_data();
    sophy.search();
    if(!sophy.result) {
        sophy.print();
        return 0;
    }
    sophy.backtracing();
    sophy.print();
    return 0;
}


