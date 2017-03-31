#include <iostream>
#include <fstream>
#include <assert.h>
#include <random>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <time.h>
#include <math.h>
#include <list>
#include <algorithm>

#define MAX_CITIES 10
#define POP_SIZE 10 // Must be an even number!

using namespace std;



struct City{
    int x_position;
    int y_position;
    bool visited;
    int city_number;
    City(const int x, const int y, const int num) : x_position(x), y_position(y), city_number(num){}

};

struct Agent{
    double x_distance;
    double y_distance;

};

class Policy{
    double calc_distance(int city_a, int city_b);
    double calc_fitness(list<int> path);

    bool check_city(int x, int y, int city_count);
    void initialize();
    void binary_elim();
    void mutate();
    void replicate();


    vector <City> cities;
    list<int> population[POP_SIZE];
};

double Policy::calc_distance(int city_a, int city_b){
    int x1 = cities[city_a].x_position;
    int x2 = cities[city_b].x_position;
    int y1 = cities[city_a].y_position;
    int y2 = cities[city_b].y_position;
    double distance = sqrt(((x2-x1)*(x2-x1)) + ((y2-y1)*(y2-y1)));
    return distance;
}

double Policy::calc_fitness(list<int> path){
    int size = path.size();
    auto previous = path.begin();
    double fitness = 0;

    for(int i = 0; i < size - 1; i++){
        auto current = next(previous, 1);
        fitness = fitness + calc_distance(*current, *previous);
        previous = current;
    }

    return fitness;
}

bool Policy::check_city(int x,int y,int city_count){
    for (int i=0; i < city_count; i++){
        if (x == cities[i].x_position && y == cities[i].y_position){
            return false;
        }
    }
    return true;
}

void Policy::initialize(){
    int city_count = 0;
    for (int i = 0; i < MAX_CITIES; i++){
        bool unique_city = false;
        int generate_x;
        int generate_y;
        while(!unique_city){
            generate_x = rand()%100;
            generate_y = rand()%100;  
            unique_city = check_city(generate_x,generate_y, city_count);
        }


        City city_temp (generate_x , generate_y, city_count +1); 
        city_temp.visited = false;
        cities.push_back(city_temp);
        city_count++;
    }
}

void Policy::binary_elim(){
    random_shuffle(std::begin(population),std::end(population));
    for (int i=0; i < POP_SIZE; i = i+2){
        double fitness_a = calc_fitness(population[i]);
        double fitness_b = calc_fitness(population[i+1]);
        if (fitness_a > fitness_b){
            population[i+1] = populaiton [i];
            mutate(population[i+1]);
        }
        else {
            population[i] = populaiton [i+1];
            mutate(population[i]);  
        }
    }
}

void Policy::mutate(){
    
}

void Policy::replicate(){

}




