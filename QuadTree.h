#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>

const int RAND_NUM = 10;			//���������
const int MAX_OBJECT = 5;			//ÿ���ڵ��������
const int SEARCH_NUM = 15;			//��Ҫ����λ������

const double LB_X = -180.0;
const double LB_Y = -90.0;
const double RT_X = 180.0;
const double RT_Y = 90.0;
const int TREE_DEPTH = 8;
const int CHILD_NUM = 4;

class QuadTree 
{
public:

	/* һ��������������޻��֣�
	
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
	
	// ��������
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

	// λ����Ϣ
	struct PosInfo
	{
		PosInfo():latitude(0), longitude(0){}
		PosInfo(int la, int lo):latitude(la), longitude(lo){}
		int longitude;
		int latitude;
	};

	//�ڵ�
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
		Rect		rect;					//�ڵ����ľ�������
		std::vector<PosInfo> pos_array;		//�ڵ��е�λ����Ϣ
		int			child_num;				//�ڵ�ĺ��ӽڵ�����
		QuadTreeNode *child[CHILD_NUM];		//ָ���ӽڵ��ָ��
		int			depth;					//�ڵ�����
	};

	void InitQuadTree(int depth, int max_objects);
	void InitQuadTreeNode(Rect rect);											//��ʼ���Ĳ���
	void CreateQuadTreeNode(int depth, Rect rect, QuadTreeNode *p_node);		//�����Ĳ����ڵ�
	void Split(QuadTreeNode *p_node);											//�ָ�ڵ�
	void Insert(PosInfo pos, QuadTreeNode *p_node);								//����λ����Ϣ
	int  GetIndex(PosInfo pos, QuadTreeNode *p_node);							//λ���ڽڵ���ĸ�����
	void Remove(PosInfo pos, QuadTreeNode *p_node);								//ɾ��λ����Ϣ
	void Find(PosInfo pos, QuadTreeNode *p_start, QuadTreeNode *p_target);		//����λ��
	void PrintAllQuadTreeLeafNode(QuadTreeNode *p_node);						//��ӡ����Ҷ�ӽڵ�

	//���Ҹ���
	void Search(int num, PosInfo pos_source, std::vector<PosInfo> &pos_list, QuadTreeNode *p_node);

	QuadTree(int depth, int maxojects):m_depth(depth), m_maxobjects(maxojects), m_root(NULL){}
	~QuadTree();

	QuadTreeNode* GetTreeRoot() { return m_root; }
	int			  GetDepth() { return m_depth; }
	int			  GetMaxObjects() { return m_maxobjects; }
private:
	int m_depth;				//�Ĳ������
	int m_maxobjects;			//�ڵ���������������
	QuadTreeNode *m_root;		//�Ĳ������ڵ�
};

#endif