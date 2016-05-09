#include <iostream>
#include "stdlib.h"
#include "QuadTree.h"
#include <time.h>
#include <fstream>
using namespace std;
int main()
{
	ofstream myfile;
	myfile.open("F:\\QuadTree.txt", ios::out);
	if(!myfile)
	{ 
		std::cout<<"create file failed!";
		system("pause"); 
		exit(1);
	}
	
	QuadTree *p_tree = new QuadTree(TREE_DEPTH, MAX_OBJECT);
	p_tree->InitQuadTreeNode(QuadTree::Rect(LB_X, LB_Y, RT_X, RT_Y));
	srand((unsigned)time(NULL));
	for (int i = 0; i < RAND_NUM; ++i)
	{
		QuadTree::PosInfo pos;
		pos.latitude = rand() % (RT_X - LB_X) + LB_X + 1;
		pos.longitude = rand() % (RT_Y - LB_Y) + LB_Y + 1;
		p_tree->Insert(pos, p_tree->GetTreeRoot());
	}
	p_tree->PrintAllQuadTreeLeafNode(p_tree->GetTreeRoot());

	int pos_x, pos_y;
	cout<<"input a pos to search near point:"<<endl;
	cin>>pos_x>>pos_y;
	QuadTree::PosInfo pos_source(pos_x, pos_y);
	std::vector<QuadTree::PosInfo> pos_list;

	clock_t start_time = clock();
	myfile<<endl<<"start time: "<<(double)start_time<<std::endl<<std::endl;
	p_tree->Search(SEARCH_NUM, pos_source, pos_list, p_tree->GetTreeRoot());
	myfile<<"pos_list:"<<std::endl;
	for (std::vector<QuadTree::PosInfo>::iterator it = pos_list.begin(); it != pos_list.end(); ++it)
	{
		myfile<<"["<<(*it).latitude<<", "<<(*it).longitude<<"]"<<std::endl;
	}
	clock_t end_time = clock();
	myfile<<std::endl<<"end time: "<<(double)end_time<<std::endl;
	myfile<<"duration:"<<(double)(end_time - start_time) / CLOCKS_PER_SEC<<std::endl;
	
	//delete p_tree;
	myfile.close();
	system("Pause");
	return 0;
}