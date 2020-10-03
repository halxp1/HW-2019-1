#ifndef GRAPH_H_
#define GRAPH_H_

#include "define.h"
#include "data_processing.h"
class DataProcess;


class AdjMatrix{
public:
    AdjMatrix();
    ~AdjMatrix();

    AdjMatrix(DataProcess &DP);
    void Creategraph(DataProcess &DP);                 //创建图
    std::vector<EdgeData> get_edges(DataProcess &DP);  //获取图中的边
    int get_map_edges(int from, int to);               //根据路口序列获取边_id
    std::vector<int> dijkstra(int St_point, int En_point);             //杰斯特拉（Dijkstra）最短路径算法
    void pathout(std::vector<int> &result, int St_poing, int En_point);  //输出最短路径
    void Show_AdjMatrix();    //显示邻接矩阵
    void updata_map(DataProcess &DP);                                     //更新地图
    void Restore_map();                                                   //还原地图

    std::map<int, int> seq_vertex; // {序列：顶点ID}
    std::map<int, int> vertex_seq; // {顶点ID：序列}
    std::map<int, int> seq_side;   // {序列：边ID}
    std::map<int, int> side_seq;   // {边ID  ：序列}
    std::map<int, int> seq_car;    // {序列 ： 车ID}
    std::map<int, int> car_seq;    // {车ID ： 序列}



private:
    std::vector<EdgeData> Edata;   //边的结构体向量
    std::vector<std::vector<int>> GA; //二维向量存储图
    std::vector<std::vector<int>> Backup_GA; //备份二维向量存储图
    std::vector<std::vector<int>>  roadID; //根据序列二维向量求roadid
//    std::map<int, std::vector> Re_roadID; //根据序列roadid求 x  y;
    std::vector<int> prev;         //记录已访问的前驱顶点 即，prev[i]的值是"顶点Spoint"到"顶点i"的最短路径所经历的全部顶点中，位于"顶点i"之前的那个顶点。
    std::vector<int> dist;            //长度向量。即，dist[i]是"起始点Spoint"到"顶点i"的最短路径的长度。
    int size;                      //顶点数
    int numE;                      //边数
};

#endif
