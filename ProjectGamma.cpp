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
#define POP_SIZE 1000 // Must be an even number!
#define ITERATIONS 1000

using namespace std;



struct City{
    int x_position;
    int y_position;
    bool visited;
    int city_number;
    City() {

    };
    City(const int x, const int y, const int num) : x_position(x), y_position(y), city_number(num){}

};

struct Agent{
    double x_distance;
    double y_distance;

};

class Policy{
    public:
    Policy();
    double calc_distance(int city_a, int city_b);
    double calc_fitness(list<int> path);

    bool check_city(int x, int y, int city_count);
    void initialize();
    void generate_population();
    void binary_elim();
    void mutate(list<int>&);
    void print_data();
    void run();
    void convergence_test();

    private:
    vector <City> cities;
    City starting_city;
    list<int> population[POP_SIZE];
};

Policy :: Policy (){
    srand(time(NULL));
    initialize();

}

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

        City city_temp (generate_x , generate_y, city_count); 
        city_temp.visited = false;
        cities.push_back(city_temp);
        city_count++;
    }

    starting_city = cities[0];

}

void Policy::generate_population(){
     vector<int> numbers;
    for(int i = 1; i < MAX_CITIES; i++){
      numbers.push_back(i);
    }

    for(int i = 0; i < POP_SIZE; i++){
        list<int> temp;
        temp.push_back(0);
        random_shuffle(begin(numbers), end(numbers));

        for(int& index : numbers){
            temp.push_back(index);
        }

        population[i] = temp;

    }

    for(int i = 0; i < POP_SIZE; i++){
        list<int> temp = population[i];
        list<int>::const_iterator index;
         for( index = temp.begin(); index != temp.end(); ++index)
            cout << *index << " ";

        cout << endl;
    }
}
  
void Policy::binary_elim(){

    random_shuffle(std::begin(population),std::end(population));
    for (int i=0; i < POP_SIZE; i = i+2){
        double fitness_a = calc_fitness(population[i]);
        double fitness_b = calc_fitness(population[i+1]);
        if (fitness_a < fitness_b){
            population[i+1] = population [i];
            mutate(population[i+1]);
        }
        else {
            population[i] = population [i+1];
            mutate(population[i]);  
        }
    }
    random_shuffle(std::begin(population),std::end(population));
}

void Policy::mutate(list<int>& path){
    int city_a = 1 + rand()%MAX_CITIES;
    int city_b = 1 + rand()%MAX_CITIES;

    auto first = next(path.begin(), city_a);
    auto second = next(path.begin(), city_b);

    iter_swap(first, second);
}


void Policy::print_data(){
    double most_fit = calc_fitness(population[0]);

    for(int i = 1; i < POP_SIZE; i++){
        double current = calc_fitness(population[i]);
        if (current < most_fit)
            most_fit = current;

    }

    cout << most_fit << endl;



    /*for(int i = 0; i < POP_SIZE; i++){
        list<int> temp = population[i];
        list<int>::const_iterator index;
         for( index = temp.begin(); index != temp.end(); ++index)
            cout << *index << " ";

        cout << endl;
    }*/
}

void Policy::run(){
    cout << "initialized" << endl;
    generate_population();
    cout << "made a poulation" << endl;

    for (int i = 0; i < ITERATIONS; i++){
    binary_elim();
    print_data();
    }
}

void Policy::convergence_test(){
    int city_count = 0;
    int generate_x = 5;
    for (int i = 0; i < MAX_CITIES; i++){
        int generate_y = i;
        City city_temp (generate_x , generate_y, city_count); 
        city_temp.visited = false;
        cities.push_back(city_temp);
        city_count++;
    }
    starting_city = cities[0];

    run();
}



int main() {
    Policy new_policy;
    new_policy.run();
    //new_policy.convergence_test();
    return 0;
}


    
