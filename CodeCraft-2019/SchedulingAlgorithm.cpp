#include "define.h"
#include "SchedulingAlgorithm.h"
#include "Tools.h"

SchedulingAlgorithm::SchedulingAlgorithm() {
    answer.clear();
    PositiveRun.clear();
    NegativeRun.clear();
    PositiveRunend.clear();
    NegativeRunstart.clear();
}

SchedulingAlgorithm::~SchedulingAlgorithm() {

}

SchedulingAlgorithm::SchedulingAlgorithm(DataProcess &DP, AdjMatrix &AM) {
    std::vector<Car> carlist =DP.get_carList();
    std::vector<std::vector<int>> tt(carlist.size());
    std::vector<int> result;
    for(int i=0;i<carlist.size();i++){
        tt[i].push_back(carlist[i].id);
        tt[i].push_back(carlist[i].planTime);
        result =crossid_to_roadid(carlist[i].from,carlist[i].to,DP, AM);
        tt[i].insert(tt[i].end(),result.begin(), result.end());
    }
    answer =tt;
}

std::string SchedulingAlgorithm::onecar(DataProcess &DP, AdjMatrix &AM) {
    std::string str;
    std::vector<int> car;
    std::vector<Car> carlist = DP.get_carList();
    std::map<int, Road> roadList = DP.get_roadList();
    int flag=0;   //标志位
    int real_time=1;
    for(int i =0; i<carlist.size(); i++){
        str +='('+ std::to_string(carlist[i].id) + ','+' ';
        if(real_time<carlist[i].planTime){
            real_time = carlist[i].planTime;
        }
        if(flag<12){   //发车关键标志位
            str+=std::to_string(real_time)+','+' ';
            flag+=1;
        }
        else{
            real_time+=1;
            str+=std::to_string(real_time)+','+' ';
            flag =0;
        }
        car=crossid_to_roadid(carlist[i].from, carlist[i].to, DP, AM);  //返回每一条线路
        for(int k=0; k<car.size()-1;k++) {
            str += std::to_string(car[k]) + ','+' ';
        }
        str += std::to_string(car.back()) + ')' +'\n';
        car.clear();
    }
    str.pop_back();
    return str;
}

std::string SchedulingAlgorithm::twocar(DataProcess &DP, AdjMatrix &AM) {
    std::string str;
    std::vector<Car> carlist = DP.get_carList();
    std::map<int,Road> roadList = DP.get_roadList();
    std::vector<std::vector<int>> result(carlist.size());
    std::vector<int> car1;
    std::vector<int> car2;
    int flag =20;
    Split_data_set_1(DP,AM);  //按方向分割
    int i=0;   //result
    int j=0;   //answer
    int k=1;   //实际发车时间
    int min=0;
    int pos=0;
    int carnum=1;

    result[i].insert(result[i].end(),PositiveRun[j].begin(),PositiveRun[j].end());
    PositiveRun.erase(PositiveRun.begin());
    while(PositiveRun.size()){
        car1.insert(car1.end(), result[i].begin() + 2, result[i].end());    //插入 每个时间片累计在一起
        if(PositiveRun.size()<100) {
            if(car1.size() > roadList.size()) {    //同一发车时间，有多少条车
                i += 1;
                carnum=1;
                car1.clear();
                car1.insert(car1.end(), PositiveRun[j].begin() + 2, PositiveRun[j].end());
                result[i].insert(result[i].end(), PositiveRun[j].begin(), PositiveRun[j].end());
                PositiveRun.erase(PositiveRun.begin());
                k += 1;
                result[i][1] = k;
            }
        }
        else {
            if(car1.size() > roadList.size()) {    //同一发车时间，有多少条车
                i += 1;
                carnum=1;
                car1.clear();
                car1.insert(car1.end(), PositiveRun[j].begin() + 2, PositiveRun[j].end());
                result[i].insert(result[i].end(), PositiveRun[j].begin(), PositiveRun[j].end());
                PositiveRun.erase(PositiveRun.begin());
                k += 1;
                result[i][1] = k;

            }
        }
        car2.assign(PositiveRun[j].begin() + 2, PositiveRun[j].end());    //赋值
        min = Tools::Cal_similarity(car1, car2);
        int xx = 1000;
        if (xx>PositiveRun.size())
            xx=PositiveRun.size();
        for (int m = 0; m < xx; m++) {
            car2.assign(PositiveRun[m].begin() + 2, PositiveRun[m].end());    //赋值
            if (min > Tools::Cal_similarity(car1, car2) && PositiveRun[m][1]<=k){  //满足搜到最小值或者预计发车时间大于真实发车时间
                min = Tools::Cal_similarity(car1, car2);
                pos = m;

            }
            if (min==0)
                break;
        }
        result[i + 1].insert(result[i + 1].end(), PositiveRun[pos].begin(), PositiveRun[pos].end());
        result[i + 1][1] = k;
        carnum+=1;
        PositiveRun.erase(PositiveRun.begin()+pos);
        i += 1;
        pos=0;
    }

    while(PositiveRunend.size()){
        i+=1;
        result[i].insert(result[i].end(), PositiveRunend[0].begin(),PositiveRunend[0].end());
        PositiveRunend.erase(PositiveRunend.begin());
        k+=1;
        result[i][1]=k;
    }

    k+=1;
    /************************************NegativeRun***************************************************************/
    while(NegativeRunstart.size()){
        i+=1;
        result[i].insert(result[i].end(), NegativeRunstart[0].begin(),NegativeRunstart[0].end());
        NegativeRunstart.erase(NegativeRunstart.begin());
        k+=1;
        result[i][1]=k;
    }


    car1.clear();
    car2.clear();
    i+=1;
    carnum=1;
    result[i].insert(result[i].end(),NegativeRun[j].begin(),NegativeRun[j].end());
    result[i][1]=k;
    NegativeRun.erase(NegativeRun.begin());
    while(NegativeRun.size()){
        car1.insert(car1.end(), result[i].begin() + 2, result[i].end());    //插入 每个时间片累计在一起

        if(flag!=0){
            if(car1.size()>roadList.size()){     //同一发车时间，有多少条车
                i+=1;
                carnum=1;
                car1.clear();
                car1.insert(car1.end(), NegativeRun[j].begin() + 2, NegativeRun[j].end());
                result[i].insert(result[i].end(),NegativeRun[j].begin(),NegativeRun[j].end());
                NegativeRun.erase(NegativeRun.begin());
                k+=1;
                result[i][1] = k;
                flag-=1;
            }
        }
        else{
            if(car1.size()>roadList.size()){     //同一发车时间，有多少条车
                i+=1;
                carnum=1;
                car1.clear();
                car1.insert(car1.end(), NegativeRun[j].begin() + 2, NegativeRun[j].end());
                result[i].insert(result[i].end(),NegativeRun[j].begin(),NegativeRun[j].end());
                NegativeRun.erase(NegativeRun.begin());
                k+=1;
                result[i][1] = k;
            }
        }

        car2.assign(NegativeRun[j].begin() + 2, NegativeRun[j].end());    //赋值
        min = Tools::Cal_similarity(car1, car2);
        int yy=1000;
        if (yy>NegativeRun.size())
            yy=NegativeRun.size();
        for (int m = 0; m < yy; m++) {
            car2.assign(NegativeRun[m].begin() + 2, NegativeRun[m].end());    //赋值
            if (min > Tools::Cal_similarity(car1, car2) && NegativeRun[m][1]<=k){  //满足搜到最小值或者预计发车时间大于真实发车时间
                min = Tools::Cal_similarity(car1, car2);
                pos = m;

            }
            if (min==0)
                break;
        }
        result[i + 1].insert(result[i + 1].end(), NegativeRun[pos].begin(), NegativeRun[pos].end());
        result[i + 1][1] = k;
        carnum+=1;
        NegativeRun.erase(NegativeRun.begin()+pos);
        i += 1;
        pos=0;

    }
    for(int n=0; n< result.size(); n++){
        str+='(';
        for(int l=0; l<result[n].size()-1; l++){
            str+=std::to_string(result[n][l])+','+' ';
        }
        str+=std::to_string(result[n].back())+')'+'\n';
    }
    str.pop_back();

    return str;

}

std::string SchedulingAlgorithm::dynamic_plan(DataProcess &DP, AdjMatrix &AM) {
    std::string str;
    int startcar=150;
    std::vector<Car> carlist = DP.get_carList();
    std::vector<std::vector<int>> result;
    std::vector<std::vector<int>> Middle_result;
    Split_data_set_2(DP,AM); //划分数据集
    std::vector<Car> now_car;
    std::vector<int> road ; //一次时间片返回的所有路径
    int real_time=1;
    int random=0;
    /*********************************升序********************************************/
    now_car.insert(now_car.end(), Lower_triangle.begin(), Lower_triangle.begin()+100);
    now_car.insert(now_car.end(), Upper_triangle.begin(), Upper_triangle.begin()+100);
    Lower_triangle.erase(Lower_triangle.begin(), Lower_triangle.begin()+100);
    Upper_triangle.erase(Upper_triangle.begin(), Upper_triangle.begin()+100);
    std::random_shuffle(now_car.begin(),now_car.end());  //打乱顺序
    road=car_to_result(now_car, DP, AM, real_time, result); //结果存储到resul
    real_time=0;
    now_car.clear();
    road.clear();
    while(Lower_triangle.size()||Upper_triangle.size()){
        if(Lower_triangle.size()>=startcar){
            //逆向
            real_time+=1;
            now_car.insert(now_car.end(),Lower_triangle.begin(), Lower_triangle.begin()+startcar);
            Lower_triangle.erase(Lower_triangle.begin(), Lower_triangle.begin()+startcar);
            //正向
            if(Upper_triangle.size()>=startcar){
                now_car.insert(now_car.end(), Upper_triangle.begin(), Upper_triangle.begin()+startcar);
                Upper_triangle.erase(Upper_triangle.begin(), Upper_triangle.begin()+startcar);
            }
            else{
                now_car.insert(now_car.end(), Upper_triangle.begin(), Upper_triangle.end());
                Upper_triangle.erase(Upper_triangle.begin(), Upper_triangle.end());
            }

        }
        else{
            //逆向
            real_time+=1;
            now_car.insert(now_car.end(),Lower_triangle.begin(), Lower_triangle.end());
            Lower_triangle.erase(Lower_triangle.begin(), Lower_triangle.end());
            //正向
            if(Upper_triangle.size()>=startcar){
                now_car.insert(now_car.end(), Upper_triangle.begin(), Upper_triangle.begin()+startcar);
                Upper_triangle.erase(Upper_triangle.begin(), Upper_triangle.begin()+startcar);
            }
            else{
                now_car.insert(now_car.end(), Upper_triangle.begin(), Upper_triangle.end());
                Upper_triangle.erase(Upper_triangle.begin(), Upper_triangle.end());
            }

        }


        std::random_shuffle(now_car.begin(),now_car.end());  //打乱顺序
        road=car_to_result(now_car, DP, AM, real_time, Middle_result); //结果存储到result

        Departure_schedule(DP,AM,Middle_result, real_time);          //动态发车

        result.insert(result.end(), Middle_result.begin(), Middle_result.end());
        Middle_result.clear();

        DP.UpdataCrowding_rate();      //更新每条路上的拥挤度
        AM.updata_map(DP);             //更新地图 ：为下一次规划路径做准备
        if(random==1){
            real_time+=4;
            random=0;
        }
        else
            real_time+=4;

        random+=1;

        now_car.clear();             //清除为下一时间片做准备
        road.clear();                //清除为下一时间片做准备
    }


//    AM.Restore_map();                  //还原地图
//    AM.updata_map(DP);                 //更新地图
//    Update_path(result, DP, AM);    //路径过长重新修改

    for(int n=0; n< result.size(); n++){
        str+='(';
        for(int l=0; l<result[n].size()-1; l++){
            str+=std::to_string(result[n][l])+','+' ';
        }
        str+=std::to_string(result[n].back())+')'+'\n';
    }
    str.pop_back();
    return str;
}

void SchedulingAlgorithm::Update_path(std::vector<std::vector<int>> &result, DataProcess &DP, AdjMatrix &AM) {
    std::vector<Car> carList =DP.get_carList();
    int length=0;
    int carid=0;
    int carseq=0;
    int from , to;
    int real_time=0;
    std::vector<int> road;
    for(int i =0; i<result.size(); i++){
        length = result[i].size()-2;
        if(length>14){
            carid = result[i][0];
            real_time = result[i][1];
            carseq = AM.car_seq[carid];
            from = carList[carseq].from;
            to = carList[carseq].to;
            road = crossid_to_roadid(from, to, DP, AM);
            result[i].clear();
            result[i].push_back(carid);
            result[i].push_back(real_time);
            result[i].insert(result[i].end(), road.begin(), road.end());
            road.clear();
        }

    }

}

void SchedulingAlgorithm::Departure_schedule(DataProcess &DP, AdjMatrix &AM, std::vector<std::vector<int>> &result, int &real_time) {
    DP.Clearroad_car();            //先清除道路上每一条车
    std::vector<int> road;
    int Waiting_state=0;
    std::map<int,Road> roadList = DP.get_roadList();
    std::vector<Car> carList =DP.get_carList();
    float max_time, time;

    for(int i=0; i<result.size(); i++){
        if(i%2 == 0){  //每10次进行评估
            double car_path_count =0.1;  //每辆车走过的路径
            int speed =0;
            max_time=0;
            for (int j=Waiting_state; j<i; j++){
                time=0;
                road.insert(road.end(),result[j].begin()+2,result[j].end());
                car_path_count+=result[j].size()-2; //记录每10辆车的路径数量
                DP.Updataroad_car(road,AM);    //更新每条路上的车辆
                for(int k=0; k<road.size();k++){
                    speed = roadList[AM.side_seq[road[k]]].speed <  carList[AM.car_seq[result[j][0]]].speed ?
                            roadList[AM.side_seq[road[k]]].speed :  carList[AM.car_seq[result[j][0]]].speed ; //获得最大速度
                    time = time + roadList[AM.side_seq[road[k]]].length/speed;
                    max_time = max_time > time ? max_time : time;
                }
                road.clear();

            }
            if(car_path_count/DP.get_road_cap() >= 0.2){ //总的容积率大于0.3，后面所有车子需要延时  DP.get_road_cap()=1912
                Waiting_state = i;
                DP.Clearroad_car();            //先清除道路上每一条车
                road.clear();
                for(int m = i; m<result.size(); m++){
                    result[m][1] = real_time + (int) (max_time/84)+1;
                    road.insert(road.end(),result[m].begin()+2, result[m].end());
                    DP.Updataroad_car(road,AM);    //更新每条路上的车辆
                    road.clear();
                }
            }
            real_time = result.back()[1];  //记录最后的发车时刻
        }
    }
    Reorder(result);

}

void SchedulingAlgorithm::Reorder(std::vector<std::vector<int>> &result) {  //同一个时间片内   出发点尽量不同
    int f_road_id =0;  //每辆车的起点
    int f_road_id_last =0;
    int real_time = 0;
    int real_time_last = 0;
    for(int i=0; i<result.size(); i++){
        for(int j=i; j<result.size(); j++){
            f_road_id = result[i][2];
            f_road_id_last= result[j][2];
            real_time = result[i][1];
            real_time_last = result[j][1];
            if(f_road_id==f_road_id_last && real_time==real_time_last && real_time>11){
                result[j][1] = real_time-2;
            }
        }
    }
}


void SchedulingAlgorithm::Split_data_set_1(DataProcess &DP,AdjMatrix &AM) {
    /****************把数据分割为正向逆向**************************/
    std::vector<Car> carlist = DP.get_carList();
    std::map<int,Road> roadList = DP.get_roadList();
    int Positiveflag =10;
    int NegativeRunflag =10;
    int roadstart = roadList[0].id;
    int roadend = roadList[roadList.size()-1].id;
    int middle = int((roadstart+roadend)/2);


    for (int i = 0; i < answer.size(); i++) {
        if(answer[i][2] < answer[i].back())
            PositiveRun.push_back(answer[i]);
        else
            NegativeRun.push_back(answer[i]);
    }

    for(int j =0; j< PositiveRun.size(); j++){
        if(PositiveRun[j][2]>middle && PositiveRun[j].back()>middle){
            PositiveRunend.push_back(PositiveRun[j]);
            PositiveRun.erase(PositiveRun.begin()+j);
            j-=1;
            Positiveflag-=1;
            if(Positiveflag==0)
                break;
        }
    }

    for(int k=0; k<NegativeRun.size(); k++){
        if(NegativeRun[k][2]<middle && NegativeRun[k].back()<middle){
            NegativeRunstart.push_back(NegativeRun[k]);
            NegativeRun.erase(NegativeRun.begin()+k);
            k-=1;
            NegativeRunflag-=1;
            if(NegativeRunflag==0)
                break;
        }
    }

}

void SchedulingAlgorithm::Split_data_set_2(DataProcess &DP, AdjMatrix &AM) {
    int x,y=0;
    std::vector<Car> carlist = DP.get_carList();
    for(int i=0; i<carlist.size();i++){
        x =carlist[i].from;
        y = carlist[i].to;
        if(x<y)
            Lower_triangle.push_back(carlist[i]);
        else
            Upper_triangle.push_back(carlist[i]);
    }

}


std::vector<int> SchedulingAlgorithm::crossid_to_roadid(int from, int to, DataProcess &DP,AdjMatrix &AM) {
    /*******************根据cross_id转为road_id**************************/
    std::vector<int> cross;
    std::vector<int> road;     //转为最后road结果
    int roadid = 0;
    from = AM.vertex_seq[from];  //获得路口from节点
    to = AM.vertex_seq[to];      //获得路口to节点

    cross = AM.dijkstra(from, to); //节点路口序列
    for(int i=0; i<cross.size()-1; i++){
        roadid = AM.get_map_edges(cross[i], cross[i+1]);
        road.push_back(roadid);
    }
    return road;
}

std::vector<int> SchedulingAlgorithm::car_to_result(std::vector<Car> now_car, DataProcess &DP, AdjMatrix &AM, int flag, std::vector<std::vector<int>> &result){
    int x,y=0;
    std::vector<int> road;
    std::vector<int> roadID;
    std::vector<std::vector<int>> tt(now_car.size());
    for(int i=0; i<now_car.size();i++){
        x=now_car[i].from;
        y=now_car[i].to;   //顶点
        road = crossid_to_roadid(x,y,DP,AM);

        tt[i].push_back(now_car[i].id);  //车子ID
        tt[i].push_back(flag);          //发车时间片
        tt[i].insert(tt[i].begin()+2,road.begin(), road.end()); //把最短路劲放进去
        roadID.insert(roadID.end(),road.begin(), road.end());
        road.clear();
    }
    result.insert(result.end(),tt.begin(), tt.end());
    return roadID;
}


int SchedulingAlgorithm::Cal_max_cap(Road road) {
    return road.length * road.channel;
}

int SchedulingAlgorithm::Cal_Max_capacity(std::vector<int> one_road, DataProcess &DP,AdjMatrix &AM) {
    /***********输入一条线路_id，输出这条线路的最大容量*************************/
    int sum=0;
    std::map<int, Road> roadList = DP.get_roadList();
    for(int i=0;i<one_road.size();i++){
        sum += Cal_max_cap(roadList[AM.side_seq[one_road[i]]]);
    }
    return sum;
}