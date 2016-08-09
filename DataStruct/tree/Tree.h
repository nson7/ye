#ifndef _TREE_H_
#define _TREE_H_

typedef void Tree;
typedef void TreeNode;

/* ������ */
Tree* Tree_Create();

/* �����Ѵ��ڵ��� */
void Tree_Destroy(Tree* tree);

/* ���Ѵ��ڵ������Ϊ���� */
void Tree_Clear(Tree* tree);

/* �����node���뵽tree�е�posλ�ô� */
int Tree_Insert(Tree* tree, TreeNode* node, int pos);

/* ��tree��posλ�õĽ��ɾ�������� */
TreeNode* Tree_Delete(Tree* tree, int pos);

/* ��tree��posλ�õĽ�㷵�� */
TreeNode* Tree_Get(Tree* tree, int pos);

/* ����tree�ĸ���� */
TreeNode* Tree_Root(Tree* tree);

/* ����tree�ĸ߶� */
int Tree_Height(Tree* tree);

/* �������Ľ���� */
int Tree_Count(Tree* tree);

/* �������Ķ��� */
int Tree_Degree(Tree* tree);

#endif
