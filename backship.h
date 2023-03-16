//950181F63D0A883F183EC0A5CC67B19928FE896A
#include <iostream>
#include <vector>
#include <deque>
#include <stack>
using namespace std;

class backship {
    public:
    uint32_t num = 0; //record the number of policy
    char policy = '\0';//option s / q
    bool out_M = true;//if output is false, use Map; otherwise List
    
    //read_data
    uint32_t num_lev = 20; //the number of the level check <=10
    uint32_t side = 0; //the side of the squre level
    //details

    struct point {
        char direct = 'A'; //default A means undiscovered, change to nsew
        char def = '.'; // default .
        //bool discover = false;
        uint8_t number; //level of E
    };

    struct element{
        uint32_t row = 0;
        uint32_t col = 0;
        uint32_t lev = 0;
        //bool empty;
    };
    element ps; //the place of start
    element ph; // the place of hanger
    element curr; //the current one in the search
    element temp;
    uint32_t temp_n = 0;
    uint32_t temp_e = 0;
    uint32_t temp_s = 0;
    uint32_t temp_w = 0;
    char current; 
    char final; //record the last move
    bool result = false; //whether the map has the result

    char tempback;//the backtrack type
    uint32_t level; //record the level number
    element out;//output of list

    //functions
    void getmode(int argc, char *argv[]);//getmode can be a member function
    void read_data();
    void search();
    void equal(element &x,const element y);
    void inverse();
    void backtracing();
    void print();
    
    private:
    vector<vector<vector<point>>> map;
    //vector<vector<vector<point>>> map_out;
    deque<element> path;
    //stack<char> back_track;

};