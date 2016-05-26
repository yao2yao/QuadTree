#include <iostream>
#include "QuadTree.h"
#include <fstream>

using namespace std;

QuadTree::~QuadTree()
{
	delete m_root;
}

void QuadTree::InitQuadTreeNode(Rect rect)
{
	m_root = new QuadTreeNode;
	m_root->rect = rect;

	for (int i = 0; i < CHILD_NUM; ++i) 
	{  
		m_root->child[i] = NULL; 
	}  
	m_root->depth = 0;
	m_root->child_num = 0;
	m_root->pos_array.clear();
}

void QuadTree::CreateQuadTreeNode(int depth, Rect rect, QuadTreeNode *p_node)
{
	p_node->depth = depth;
	p_node->child_num = 0;
	p_node->pos_array.clear();
	p_node->rect = rect;
	for (int i = 0; i < CHILD_NUM; ++i)  
	{  
		p_node->child[i] = NULL; 
	}
}

void QuadTree::Split(QuadTreeNode *pNode)
{
	if (pNode == NULL)
	{
		return;
	}

	double start_x = pNode->rect.lb_x;
	double start_y = pNode->rect.lb_y;
	//int sub_width = (pNode->rect.rt_x - pNode->rect.lb_x) / 2;
	//int sub_height = (pNode->rect.rt_y - pNode->rect.lb_y) / 2;
	double la_sum = 0, lo_sum = 0;
	for (std::vector<PosInfo>::iterator it = pNode->pos_array.begin(); it != pNode->pos_array.end(); ++it)
	{
		la_sum += (*it).latitude - start_x;
		lo_sum += (*it).longitude - start_y;
	}
	double sub_width = (la_sum * 1.0 / (int)pNode->pos_array.size());
	double sub_height = (lo_sum * 1.0 / (int)pNode->pos_array.size());
	double end_x = pNode->rect.rt_x;
	double end_y = pNode->rect.rt_y;

	QuadTreeNode *p_node0 = new QuadTreeNode;
	QuadTreeNode *p_node1 = new QuadTreeNode;
	QuadTreeNode *p_node2 = new QuadTreeNode;
	QuadTreeNode *p_node3 = new QuadTreeNode;

	CreateQuadTreeNode(pNode->depth + 1, Rect(start_x + sub_width, start_y + sub_height, end_x, end_y), p_node0);
	CreateQuadTreeNode(pNode->depth + 1, Rect(start_x, start_y + sub_height, start_x + sub_width, end_y), p_node1);
	CreateQuadTreeNode(pNode->depth + 1, Rect(start_x, start_y, start_x + sub_width, start_y + sub_height), p_node2);
	CreateQuadTreeNode(pNode->depth + 1, Rect(start_x + sub_width, start_y, end_x, start_y + sub_height), p_node3);

	pNode->child[0] = p_node0;
	pNode->child[1] = p_node1;
	pNode->child[2] = p_node2;
	pNode->child[3] = p_node3;

	pNode->child_num = 4;
}

void QuadTree::Insert(PosInfo pos, QuadTreeNode *p_node)
{
	if (p_node == NULL)
	{
		return;
	}

	if (p_node->child_num != 0)
	{
		int index = GetIndex(pos, p_node);
		if (index >= UR && index <= LR)
		{
			Insert(pos, p_node->child[index]);
		}
	}
	else
	{
		if ((int)p_node->pos_array.size() >= m_maxobjects && p_node->depth < m_depth)
		{
			Split(p_node);
			for (std::vector<PosInfo>::iterator it = p_node->pos_array.begin(); it != p_node->pos_array.end(); ++it)
			{
				Insert(*it, p_node);
			}
			p_node->pos_array.clear();
			Insert(pos, p_node);
		}
		else
		{
			p_node->pos_array.push_back(pos);
		}
	}
}

void QuadTree::Search(int num, PosInfo pos_source, std::vector<PosInfo> &pos_list, QuadTreeNode *p_node)
{
	if (p_node == NULL)
	{
		return;
	}

	if ((int)pos_list.size() >= num)
	{
		return;
	}

	if (p_node->child_num == 0)
	{
		for (std::vector<PosInfo>::iterator it = p_node->pos_array.begin(); it != p_node->pos_array.end(); ++it)
		{
			if ((int)pos_list.size() >= num)
			{
				return;
			}
			pos_list.push_back(*it);
		}
		return;
	}

	//目标节点
	int index = GetIndex(pos_source, p_node);
	if (index >= UR && index <= LR)
	{
		if (p_node->child[index] != NULL)
		{
			Search(num, pos_source, pos_list, p_node->child[index]);
		}
	}

	//目标节点的兄弟节点
	for (int i = 0; i < CHILD_NUM; ++i)
	{
		if (index != i && p_node->child[i] != NULL)
		{
			Search(num, pos_source, pos_list, p_node->child[i]);
		}
	}
}

int QuadTree::GetIndex(PosInfo pos, QuadTreeNode *pNode)
{
	double start_x = pNode->rect.lb_x;
	double start_y = pNode->rect.lb_y;
	double sub_width = (pNode->rect.rt_x - pNode->rect.lb_x) / 2;
	double sub_height = (pNode->rect.rt_y - pNode->rect.lb_y) / 2;
	double end_x = pNode->rect.rt_x;
	double end_y = pNode->rect.rt_y;
	
	//0象限
	if ((pos.latitude >= start_x + sub_width) 
	&& (pos.latitude <= end_x) 
	&& (pos.longitude >= start_y + sub_height) 
	&& (pos.longitude <= end_y))
	{
		return UR;
	}
	//1象限
	else if ((pos.latitude >= start_x) 
	&& (pos.latitude < start_x + sub_width) 
	&& (pos.longitude >= start_y + sub_height) 
	&& (pos.longitude <= end_y))
	{
		return UL;
	}
	//2象限
	else if ((pos.latitude >= start_x) 
	&& (pos.latitude < start_x + sub_width) 
	&& (pos.longitude >= start_y) 
	&& (pos.longitude < start_y + sub_height))
	{
		return LL;
	}
	//3象限
	else if ((pos.latitude >= start_x + sub_width) 
	&& (pos.latitude <= end_x) 
	&& (pos.longitude >= start_y) 
	&& (pos.longitude < start_y + sub_height))
	{
		return LR;
	}
	else
	{
		return INVALID;
	}
}

void QuadTree::Remove(PosInfo pos, QuadTreeNode* p_node)
{
	if (p_node == NULL)
	{
		return;
	}

// 	for (std::vector<PosInfo>::iterator it = p_node->pos_array.begin(); it != p_node->pos_array.end(); ++it)
// 	{
// 		if (pos.user_id == (*it).user_id)
// 		{
// 			p_node->pos_array.erase(it);
// 			break;
// 		}
// 	}
}

void QuadTree::Find(PosInfo pos, QuadTreeNode *p_start, QuadTreeNode *p_target)
{
	if (p_start == NULL)
	{
		return;
	}

	p_target = p_start;

	int index = GetIndex(pos, p_start);
	if (index >= UR && index <= LR)
	{
		if (p_start->child[index] != NULL)
		{
			Find(pos, p_start->child[index], p_target);
		}
	}
}

void QuadTree::PrintAllQuadTreeLeafNode(QuadTreeNode *p_node)
{
	if (p_node == NULL)
	{
		return;
	}
	
	ofstream myfile;
	myfile.open("F:\\Node.txt", ios::out|ios::app);
	if(!myfile)
	{ 
		return;
	}

	if (p_node->child_num == 0)
	{
		myfile<<"Node:["<<p_node->rect.lb_x<<", "<<p_node->rect.lb_y<<", "<<p_node->rect.rt_x<<", "<<p_node->rect.rt_y<<"], Depth:"<<p_node->depth<<", PosNum:"<<(int)p_node->pos_array.size()<<endl;
		myfile<<"{"<<endl;
		for (std::vector<PosInfo>::iterator it = p_node->pos_array.begin(); it != p_node->pos_array.end(); ++it)
		{
			myfile<<"["<<(*it).latitude<<", "<<(*it).longitude<<"]"<<std::endl;
		}
		myfile<<"}"<<endl;
		return;
	}
	else
	{
		for (int i = 0; i < CHILD_NUM; ++i)
		{
			PrintAllQuadTreeLeafNode(p_node->child[i]);
		}
	}
}