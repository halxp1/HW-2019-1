#include <iostream>
#include "define.h"
#include "data_processing.h"
#include <algorithm>
#include <functional>

DataProcess::DataProcess() {
    crossList.clear();
    roadList.clear();
    carList.clear();

    input_car_num=0;
    input_cross_num =0;
    input_road_num = 0;
}

DataProcess::~DataProcess() {

}

DataProcess::DataProcess(char *car[MAX_CAR_NUM],int car_line_num, char *cross[MAX_CROSS_NUM],
                         int cross_line_num, char *road[MAX_ROAD_NUM], int road_line_num)
{
    inputCar = car;
    input_car_num = car_line_num;

    inputCross = cross;
    input_cross_num = cross_line_num;

    inputRoad = road;
    input_road_num = road_line_num;

}

void DataProcess::readcardata() {
    /*************读取Car********************/
    for(int i=1;i<input_car_num;i++){  //略过第一注释行

        std::string strRow = inputCar[i];
        if(i==input_car_num-1)         //最后一行添加"\n"字符
            strRow+="\n";
        std::vector<std::string> st = Tools::split(strRow, ","); //解析每一行

        Car car;
        car.id = std::stoi(st[0]);
        car.from = std::stoi(st[1]);
        car.to = std::stoi(st[2]);
        car.speed = std::stoi(st[3]);
        car.planTime = std::stoi(st[4]);

        carList.push_back(car);
    }

    carList = Sortcar(carList, 0);  //根据发车时间排名
}

void DataProcess::readroaddata() {
    /*************读取Road**************************/
    int total_cap=0;
    for(int i=1; i<input_road_num;i++){  //略过第一行注释
        std::string strRow = inputRoad[i];
        if(i==input_road_num-1)
            strRow+="\n";                //最后一行添加"\n"字符
        std::vector<std::string> st = Tools::split(strRow, ","); //解析每一行

        Road road;
        road.id = std::stoi(st[0]);
        road.length=std::stoi(st[1]);
        road.speed=std::stoi(st[2]);
        road.channel=std::stoi(st[3]);
        road.from  = std::stoi(st[4]);
        road.to=std::stoi(st[5]);
        road.isDuplex=std::stoi(st[6]);
        road.Max_capacity=road.channel*road.speed; //最大容量
        road.cars=0;  //这条道路上car数
        road.min_time = (int)(road.length/road.speed);  //最大时间
        road.Crowding_rate =0;

        total_cap+=road.Max_capacity;   //累加所有road的容量

        roadList.insert(std::make_pair(i-1,road));
    }
    road_total_cap = total_cap;

}

void DataProcess::readcrossdata() {
    /*************读取Cross**************************/
    for(int i=1; i<input_cross_num;i++){   //略过第一行注释
        std::string strRow = inputCross[i];
        if(i==input_cross_num-1)
            strRow+="\n";
        std::vector<std::string> st = Tools::split(strRow,",");

        Cross cross;
        cross.id = std::stoi(st[0]);
        cross.up_roadid = std::stoi(st[1]);
        cross.right_roadid = std::stoi(st[2]);
        cross.down_roadid = std::stoi(st[3]);
        cross.left_roadid = std::stoi(st[4]);

        crossList.push_back(cross);
    }
}

std::vector<Car> DataProcess::Sortcar(std::vector<Car> &carList , int flag) {
    //按照car.plantime进行排序 ,flag=0:代表升序  flag=1：代表降序
    if (flag==0)
        std::sort(carList.begin(), carList.end(), lessmark);
    else
        std::sort(carList.begin(), carList.end(), greatermark);

    return carList;
}


void DataProcess::Updataroad_car(std::vector<int> road,AdjMatrix &AM){       //更新每条道路上的车辆
    int roadid=0;
    for(int i=0; i<road.size();i++){
        roadid=AM.side_seq.find(road[i])->second;
        roadList[roadid].cars+=1;                                            //注意这里用序列来索引  而不是id
    }
}


void DataProcess::Clearroad_car() {                                         //清除每条道路上的车辆
    for(auto iter = roadList.begin();iter!=roadList.end();iter++){
        iter->second.cars=0;
    }
}

void DataProcess::UpdataCrowding_rate(){                                    //更新每条道路的拥挤率
    for(auto iter = roadList.begin(); iter != roadList.end(); iter++){
        iter->second.Crowding_rate = (double)iter->second.cars/(double)iter->second.Max_capacity;
    }
}

double DataProcess::countCrowd() {                                                //统计所有的拥堵率
    double count=0;
    for(auto iter = roadList.begin();iter!=roadList.end();iter++){
        count+=iter->second.Crowding_rate;
    }
    return count;
}

int DataProcess::get_road_cap() {
    return road_total_cap;
}

std::map<int, Road> DataProcess::get_roadList() {

    return roadList;
}

std::vector<Cross> DataProcess::get_crossList() {

    return crossList;
}

std::vector<Car> DataProcess::get_carList() {

    return carList;
}


bool   lessmark(const Car& s1, const  Car&   s2)
{
    return   s1.planTime   <   s2.planTime;
}

bool   greatermark(const Car&   s1, const Car&  s2)
{
    return   s1.planTime   >   s2.planTime;
}
