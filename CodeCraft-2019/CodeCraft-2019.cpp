/**
 * @project: 2019华为软件精英挑战赛
 * @code: C++
 * @author: hanlei
 * @post: 791026701@qq.com
 * @date: 2019/03/15
 * @version: 1.0
 * 此版本修改内容:
 *   1.对原始car.txt的的planTime进行排序
 */

#include "define.h"
#include "data_processing.h"
#include "Graph.h"
#include "SchedulingAlgorithm.h"
#include "Tools.h"

int main(int argc, char *argv[])
{
	std::cout << "Begin" << std::endl;

	if(argc < 5){
		std::cout << "please input args: carPath, roadPath, crossPath, answerPath" << std::endl;
		exit(1);
	}
	char *car[MAX_CAR_NUM];      //1000
	char *cross[MAX_CROSS_NUM];  //1000
	char *road[MAX_ROAD_NUM];    //1000
	int car_line_num;
	int cross_line_num;
	int road_line_num;

	std::string result;
	result += "#(carId,StartTime,RoadId...)";
	result +='\n';

/******************把数据读到内存里**********************************/
	std::cout<<"把数据读到内存里"<<std::endl;
	char *car_file= argv[1];
	car_line_num = read_file(car, MAX_CAR_NUM,car_file);

	char *road_file = argv[2];
	road_line_num = read_file(road, MAX_CROSS_NUM,road_file);

	char *cross_file = argv[3];
	cross_line_num = read_file(cross, MAX_ROAD_NUM,cross_file);
/*****************读取并解析数据到结构中******************************/
	std::cout<<"读取并解析数据到结构中"<<std::endl;
	DataProcess DP(car,car_line_num,cross,cross_line_num,road,road_line_num);
	DP.readcardata();
	DP.readroaddata();
	DP.readcrossdata();

/*********************构建图邻接矩阵**********************************/
	std::cout<<"构建图邻接矩阵"<<std::endl;
	AdjMatrix AM(DP);
	AM.Creategraph(DP);


/**********************调度算法***************************************/
	std::cout<<"调度算法"<<std::endl;
	SchedulingAlgorithm SA(DP,AM);

	result+=SA.dynamic_plan(DP,AM);

//	result+=SA.twocar(DP,AM);
//    result += SA.onecar(DP,AM);


/**********************测试区***************************************/
//    std::vector<int> as1{111,41,2,3,73,9,11,22,34,56,84,23,59,44,21,87,89,2,50,3};
//    std::vector<int> as2{1,4,2,3,7,9,50,22,34,56};
//    int count = Tools::Cal_similarity(as1, as2);
/**********************输出文件***************************************/

	char *output_file = argv[4];
	const char *result_file = result.c_str();
	write_result(result_file,output_file);
/**********************释放内存***************************************/
	std::cout<<"释放内存"<<std::endl;
	release_buff(car, car_line_num);
	release_buff(cross,cross_line_num);
	release_buff(road,road_line_num);

	std::cout<<"The code runs successful ！" <<std::endl;
	return 0;
}