#ifndef SCHEDULINGALGORITHM_H_
#define SCHEDULINGALGORITHM_H_

#include "data_processing.h"
#include "Graph.h"

/*************调度算法**********************/
class SchedulingAlgorithm{
public:
    SchedulingAlgorithm();
    ~SchedulingAlgorithm();

    SchedulingAlgorithm(DataProcess &DP, AdjMatrix &AM);
    std::string onecar(DataProcess &DP, AdjMatrix &AM);
    std::string twocar(DataProcess &DP, AdjMatrix &AM);
    std::string dynamic_plan(DataProcess &DP, AdjMatrix &AM);
    void Departure_schedule(DataProcess &DP, AdjMatrix &AM,std::vector<std::vector<int>> &result, int &real_time);
    void Update_path(std::vector<std::vector<int>> &result, DataProcess &DP, AdjMatrix &AM);
    void Reorder(std::vector<std::vector<int>> &result);


    int Cal_max_cap(Road road);
    int Cal_Max_capacity(std::vector<int> one_road, DataProcess &DP, AdjMatrix &AM);
    std::vector<int> crossid_to_roadid(int from, int to, DataProcess &DP, AdjMatrix &AM);
    std::vector<int>  car_to_result(std::vector<Car> now_car, DataProcess &DP, AdjMatrix &AM, int flag,
                                    std::vector<std::vector<int>> &result);


    void Split_data_set_1(DataProcess &DP, AdjMatrix &AM);      //按照正序下三角 逆序上三角
    void Split_data_set_2(DataProcess &DP, AdjMatrix &AM);      //按照正序下三角 逆序上三角

private:
    std::vector<std::vector<int>> answer;  //根据发车时间的结果
    std::vector<std::vector<int>> PositiveRun;  //下三角部分
    std::vector<std::vector<int>> NegativeRun;  //上三角部分
    std::vector<std::vector<int>> PositiveRunend;
    std::vector<std::vector<int>> NegativeRunstart;

    std::vector<Car> Upper_triangle;          //上三角
    std::vector<Car> Lower_triangle;          //下三角



};

#endif