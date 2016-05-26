#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>

const int RAND_NUM = 10;			//随机点数量
const int MAX_OBJECT = 5;			//每个节点最大数量
const int SEARCH_NUM = 15;			//需要搜索位置数量

const double LB_X = -180.0;
const double LB_Y = -90.0;
const double RT_X = 180.0;
const double RT_Y = 90.0;
const int TREE_DEPTH = 8;
const int CHILD_NUM = 4;

class QuadTree 
{
public:

	/* 一个矩形区域的象限划分：
	
	UL(1)   |    UR(0) 
	--------|----------- 
	LL(2)   |    LR(3) 

	*/  

	enum QuadrantEnum
	{
		INVALID = -1,
		UR = 0,
		UL = 1, 
		LL = 2,
		LR = 3,
	};
	
	// 矩形区块
	struct Rect
	{
		Rect():lb_x(0), lb_y(0), rt_x(0), rt_y(0){}
		Rect(double lb_x, double lb_y, double rt_x, double rt_y): lb_x(lb_x), lb_y(lb_y), rt_x(rt_x), rt_y(rt_y){}
		Rect& operator=(Rect &rect)
		{
			lb_x = rect.lb_x;
			lb_y = rect.lb_y;
			rt_x = rect.rt_x;
			rt_y = rect.rt_y;
			return *this;
		}
		double lb_x;
		double lb_y;
		double rt_x;
		double rt_y;
	};

	// 位置信息
	struct PosInfo
	{
		PosInfo():latitude(0), longitude(0){}
		PosInfo(int la, int lo):latitude(la), longitude(lo){}
		int longitude;
		int latitude;
	};

	//节点
	struct QuadTreeNode
	{
		QuadTreeNode(){}
		~QuadTreeNode()
		{
			pos_array.clear();
			for (int i = 0; i < CHILD_NUM; ++i)
			{
				if (child[i] == NULL)
				{
					break;
				}
				delete child[i];
			}
		}
		Rect		rect;					//节点代表的矩形区域
		std::vector<PosInfo> pos_array;		//节点中的位置信息
		int			child_num;				//节点的孩子节点数量
		QuadTreeNode *child[CHILD_NUM];		//指向孩子节点的指针
		int			depth;					//节点的深度
	};

	void InitQuadTree(int depth, int max_objects);
	void InitQuadTreeNode(Rect rect);											//初始化四叉树
	void CreateQuadTreeNode(int depth, Rect rect, QuadTreeNode *p_node);		//创建四叉树节点
	void Split(QuadTreeNode *p_node);											//分割节点
	void Insert(PosInfo pos, QuadTreeNode *p_node);								//插入位置信息
	int  GetIndex(PosInfo pos, QuadTreeNode *p_node);							//位置在节点的哪个象限
	void Remove(PosInfo pos, QuadTreeNode *p_node);								//删除位置信息
	void Find(PosInfo pos, QuadTreeNode *p_start, QuadTreeNode *p_target);		//查找位置
	void PrintAllQuadTreeLeafNode(QuadTreeNode *p_node);						//打印所有叶子节点

	//查找附近
	void Search(int num, PosInfo pos_source, std::vector<PosInfo> &pos_list, QuadTreeNode *p_node);

	QuadTree(int depth, int maxojects):m_depth(depth), m_maxobjects(maxojects), m_root(NULL){}
	~QuadTree();

	QuadTreeNode* GetTreeRoot() { return m_root; }
	int			  GetDepth() { return m_depth; }
	int			  GetMaxObjects() { return m_maxobjects; }
private:
	int m_depth;				//四叉树深度
	int m_maxobjects;			//节点包含的最大对象个数
	QuadTreeNode *m_root;		//四叉树根节点
};

#endif