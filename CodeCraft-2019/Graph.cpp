#include<iomanip>
#include <stack>
#include "Graph.h"
#include "define.h"

AdjMatrix::AdjMatrix() {
    size = 0;
    numE = 0;
    seq_vertex.clear();
    vertex_seq.clear();
    seq_side.clear();
    side_seq.clear();
    Edata.clear();
    GA.clear();
}

AdjMatrix::~AdjMatrix() {

}

AdjMatrix::AdjMatrix(DataProcess &DP) {

    std::vector<Cross> cross =DP.get_crossList();
    std::map<int, Road> road = DP.get_roadList();
    std::vector<Car> carList = DP.get_carList();

    size = int (cross.size());   //顶点数
    numE = int (road.size());    //边数


    for(int j=0; j<carList.size();j++){
        seq_car.insert(std::make_pair(j,carList[j].id));
        car_seq.insert(std::make_pair(carList[j].id, j));
    }

    for (int i=0;i<size;i++){
        seq_vertex.insert(std::make_pair(i,cross[i].id));
        vertex_seq.insert(std::make_pair(cross[i].id,i));
    }

    for (auto iter = road.begin(); iter != road.end(); iter++){
        seq_side.insert(std::make_pair(iter->first,iter->second.id));
        side_seq.insert(std::make_pair(iter->second.id,iter->first));
    }


}

void AdjMatrix::Creategraph(DataProcess &DP) {
    int x,y=0;
    std::map<int, Road> road = DP.get_roadList();
    std::vector<int> a;
    //初始化邻接矩阵为无限大，并且对角线为0
    std::vector<std::vector<int>>  ga(size,std::vector<int>(size,INT_MAX));
    GA.assign(ga.begin(),ga.end());      //初始化
    roadID.assign(ga.begin(), ga.end()); //初始化

    for(int i=0; i<size; i++){
        GA[i][i] = 0;
    }

    for(auto iter = road.begin(); iter != road.end(); iter++){
        x = vertex_seq.find(iter->second.from)->second;
        y = vertex_seq.find(iter->second.to)->second;
        GA[x][y] = iter->second.length;
        roadID[x][y] = iter->second.id;

        if(iter->second.isDuplex==1){
            GA[y][x] = iter->second.length;
            roadID[y][x] = iter->second.id;

        }
        a.clear();
    }
    Backup_GA = GA; //备份地图
}

std::vector<EdgeData> AdjMatrix::get_edges(DataProcess &DP) {
    EdgeData edges;
    std::map<int, Road> road=DP.get_roadList();
    for(auto iter = road.begin(); iter != road.end(); iter++){
        edges.Dstart = side_seq.find(iter->second.from)->second;
        edges.Dend = side_seq.find(iter->second.to)->second;
        edges.weight = iter->second.length;

        Edata.push_back(edges);
    }
    return Edata;
}

int AdjMatrix::get_map_edges(int from, int to) {
    return roadID[from][to];

}

std::vector<int> AdjMatrix::dijkstra(int St_point, int En_point) {
    int min;
    int tmp;
    int k;
    prev.clear();
    dist.clear();
    std::vector<int> result;
    std::vector<int> flag;   //flag[i]=1表示"顶点vs"到"顶点i"的最短路径已成功获取。

    for(int i=0; i<size; i++){
        flag.push_back(0);              // 顶点i的最短路径还没获取到。
        prev.push_back(-1);           // 顶点i的前驱顶点为0。
        dist.push_back(GA[St_point][i]);  // 顶点i的最短路径为"顶点Spoint"到"顶点i"的权。
    }

    // 对"顶点Spoint自身进行初始化
    flag[St_point] = 1;
    dist[St_point] = 0;

    // 遍历size-1次；每次找出一个顶点的最短路径。
    for (int i = 0;i<size-1; i++){
        // 寻找当前最小的路径；
        // 即，在未获取最短路径的顶点中，找到离Spoint最近的顶点(k)。
        min = INT_MAX;
        for(int j = 0; j< size; j++){
            if(flag[j] == 0 && dist[j]<min){
                min = dist[j];
                k = j;
            }
        }
        // 标记"顶点k"为已经获取到最短路径
        flag[k] =1;

        // 修正当前最短路径和前驱顶点
        // 即，当已经"顶点k的最短路径"之后，更新"未获取最短路径的顶点的最短路径和前驱顶点"。
        for(int j=0; j< size; j++){
            tmp = (GA[k][j] == INT_MAX ? INT_MAX : (min+ GA[k][j])); //防止溢出
            if (flag[j] ==0 && (tmp < dist[j])) {
                dist[j]=tmp;
                prev[j] = k;
            }
        }
    }

    pathout(result,St_point, En_point);     //输出路径
    return result;       //返回路径
}

void AdjMatrix::pathout(std::vector<int> &result,int St_point, int En_point) {
    int i,j;
    std::stack<int> q;
    for(i=En_point; i<En_point+1; i++){
        j=i;
        while(prev[j] != -1){
            q.push(j);
            j = prev[j];
        }
        q.push(j);
        result.push_back(St_point);
        while (!q.empty()){
            result.push_back(q.top());
            q.pop();
        }
    }
}

void AdjMatrix::Show_AdjMatrix() {
    for (int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            std::cout<<std::setw(4)<<GA[i][j];
    std::cout<<std::endl;
}

void AdjMatrix::Restore_map() {
    GA = Backup_GA;
}

void AdjMatrix::updata_map(DataProcess &DP) {
    int k=160;  //参数值
    int x,y=0;
    std::map<int, Road> road = DP.get_roadList();
    for(auto iter = road.begin(); iter != road.end(); iter++){
        //获取x,y坐标序列
        x = vertex_seq.find(iter->second.from)->second;
        y = vertex_seq.find(iter->second.to)->second;

        GA[x][y] = (int)(k * (iter->second.Crowding_rate)*(iter->second.Crowding_rate) + iter->second.length);  //更新每条道路的权值
        if(iter->second.isDuplex==1){
            GA[y][x] = (int) (k * (iter->second.Crowding_rate)*(iter->second.Crowding_rate) + iter->second.length);
        }                   //k*(拥挤度+最短时间)
    }

}