#ifndef _TREE_H_
#define _TREE_H_

typedef void Tree;
typedef void TreeNode;

/* 创建树 */
Tree* Tree_Create();

/* 销毁已存在的树 */
void Tree_Destroy(Tree* tree);

/* 将已存在的树清空为空树 */
void Tree_Clear(Tree* tree);

/* 将结点node插入到tree中的pos位置处 */
int Tree_Insert(Tree* tree, TreeNode* node, int pos);

/* 将tree中pos位置的结点删除并返回 */
TreeNode* Tree_Delete(Tree* tree, int pos);

/* 将tree中pos位置的结点返回 */
TreeNode* Tree_Get(Tree* tree, int pos);

/* 返回tree的根结点 */
TreeNode* Tree_Root(Tree* tree);

/* 返回tree的高度 */
int Tree_Height(Tree* tree);

/* 返回树的结点数 */
int Tree_Count(Tree* tree);

/* 返回树的度数 */
int Tree_Degree(Tree* tree);

#endif
