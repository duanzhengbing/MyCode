#include <iostream>
#include <vector>
#include <deque>
#include <stack>
#include <algorithm>
#include <cstring>
#include <string>
#include <sstream>
using namespace std;

struct TreeNode
{
    int value;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v):value(v),left(nullptr),right(nullptr) {}
};

TreeNode* createTree()
{
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->left = new TreeNode(6);
    root->right->right = new TreeNode(7);
    // root->left->left->right = new TreeNode(8);
    // root->left->right->left = new TreeNode(9);
    // root->left->right->right = new TreeNode(10);
    return root;
}
TreeNode* createTree2()
{
    TreeNode* root = new TreeNode(6);
    // root->left = new TreeNode(6);
    root->right = new TreeNode(7);
    // root->left->left = new TreeNode(4);
    // root->left->right = new TreeNode(7);
    // root->right->left = new TreeNode(12);
    // root->right->right = new TreeNode(18);
    return root;
}

void destroyTree(TreeNode* root)
{
    if(root == nullptr)
        return;
    if(root->left == nullptr && root->right == nullptr)
    {
        delete root;
        return;
    }
    destroyTree(root->left);
    destroyTree(root->right);
}

void printFromTopToBottom(TreeNode* root)
{
    if(root == nullptr)
        return;
    deque<TreeNode*> que;
    que.push_back(root);
    while(!que.empty())
    {
        TreeNode* tmp = que.front();
        if(tmp->left != nullptr)
            que.push_back(tmp->left);
        if(tmp->right != nullptr)
            que.push_back(tmp->right);
        que.pop_front();
        std::cout << tmp->value << " ";
    }
    cout << endl;
}


void findPath(TreeNode* root,
              std::vector<int>& path,
              std::vector<std::vector<int>>& pathSet)
{
    if(root == nullptr)
        return;
    path.push_back(root->value);
    if(root->left == nullptr && root->right == nullptr)
    {
        pathSet.push_back(path);
        path.pop_back();
        return;
    }
    if(root->left != nullptr)
        findPath(root->left,path,pathSet);
    if(root->right != nullptr)
        findPath(root->right,path,pathSet);

    // 访问完左右子树之后必须回退，将访问过的节点从路径中删除
    path.pop_back();
}

std::vector<std::vector<int>> findAllPath(TreeNode* root)
{
    std::vector<vector<int>> pathSet;
    std::vector<int> path; // 临时变量保存递归过程中的路径
    findPath(root,path,pathSet);
    return pathSet;
}

/*void maxPath(TreeNode* root,std::vector<int>& path,std::vector<int>& result,int& sum)
{
    if(root == nullptr)
        return;
    path.push_back(root->value);
    if(root->left == nullptr && root->right == nullptr)
    {
    }
}*/

int minDepthRecursion(TreeNode* root)
{
    if(root == nullptr)
        return 0;
    if(root->left == nullptr && root->right == nullptr)
    {
        return 1;
    }

    else if(root->left != nullptr && root->right == nullptr)
        return minDepthRecursion(root->left)+1;
    else if(root->left != nullptr && root->right != nullptr)
        return minDepthRecursion(root->right)+1;
    else
        return min(minDepthRecursion(root->left),
                minDepthRecursion(root->right))+1;
}

std::vector<std::vector<int>> findAllPathNoRecursion(TreeNode* root)
{
    std::vector<vector<int>> pathSet;
    if(root == nullptr)
        return pathSet;
    std::vector<int> path;

    TreeNode* cur = root;
    TreeNode* pre = nullptr;
    std::stack<TreeNode*> nodeStack; 
    while(cur || !nodeStack.empty())
    {
        // 当节点左孩子存在时，从左子树一直到底压入stack
        while(cur != nullptr)
        {
            nodeStack.push(cur);
            path.push_back(cur->value);
            cur = cur->left;
        }
        cur = nodeStack.top();

        if(cur->left == nullptr && cur->right == nullptr)
        {
            pathSet.push_back(path);
        }

        // 右子树存在且没有访问
        if(cur->right && pre != cur->right)
        {
            cur = cur->right;
        }
        else
        {
            nodeStack.pop();
            path.pop_back();
            pre = cur;  // 下一次访问的节点是新的栈顶节点，这次访问的节点就是下一次访问节点的前驱
            cur = nullptr;  // 右子树不存在，设当前访问节点为空，跳过了将右节点的左子树一直入栈的过程
        }
    }
    return pathSet; 
}


std::vector<vector<int>> pathOfSum(TreeNode* root,int sum)
{
    std::vector<vector<int>> ret;
    std::vector<int> path;
    if(root == nullptr)
        return ret;
    int tmp = 0;
    TreeNode* cur = root;
    TreeNode* pre = nullptr;
    stack<TreeNode*> nodeStack;
    while(cur != nullptr || !nodeStack.empty())
    {
        while(cur != nullptr)
        {
            nodeStack.push(cur);
            path.push_back(cur->value);
            tmp += cur->value;
            cur = cur->left;
        }
        cur = nodeStack.top();
        if(cur->left == nullptr && cur->right == nullptr && tmp == sum)
        {
            ret.push_back(path);
        }
        if(cur->right != nullptr && pre != cur->right)
        {
            cur = cur->right;
        }
        else
        {
            nodeStack.pop();
            path.pop_back();
            tmp -= cur->value; // 弹出节点的同时，路径和路径上节点值的和也需要弹出或减去
            pre = cur;
            cur = nullptr;
        }
    }

    return ret;
}

int minDepth(TreeNode* root)
{
    stack<TreeNode*> nodeStack;
    TreeNode* cur = root;
    TreeNode* pre = nullptr;
    int depth = 10000;
    int tmp = 0;
    while(cur != nullptr || !nodeStack.empty())
    {
        while(cur)
        {
            tmp++;
            nodeStack.push(cur);
            cur = cur->left;
        }
        cur = nodeStack.top();
        if(!cur->right && !cur->left && tmp < depth)
            depth = tmp;
        if(cur->right && pre != cur->right)
        {
            cur = cur->right;
        }
        else
        {
            nodeStack.pop();
            tmp--;
            pre = cur;
            cur = NULL;
        }
    }
    return depth;
}

/**
 * 迭代算法中序遍历，使用栈
 */
vector<int> inorderTraversal(TreeNode* root) 
{
    std::vector<int> ret;
    TreeNode* cur = root;
    TreeNode* pre = nullptr;
    stack<TreeNode*> nodeStack;
    while(cur != nullptr || !nodeStack.empty())
    {
        while(cur != nullptr)
        {
            nodeStack.push(cur);
            cur = cur->left;
        }
        cur = nodeStack.top();
        // 每次都访问栈顶元素
        ret.push_back(cur->value);

        /* 右子树存在并且没有访问过，那么当前栈顶元素肯定被访问过，需要将栈顶元素继续弹出 */
        if(cur->right && pre != cur->right)
        {
            nodeStack.pop();
            cur = cur->right;
        }
        /* 右子树不存在或者已经被访问过，栈顶元素 */
        else
        {
            nodeStack.pop();
            pre = cur;
            cur = nullptr;
        }
    }       
    return ret;
}


/**
 * 迭代算法先序遍历，使用栈
 */
void preorderTraversal(TreeNode* root) 
{
    TreeNode* cur = root;
    TreeNode* pre = nullptr;
    stack<TreeNode*> nodeStack;
    while(cur != nullptr || !nodeStack.empty())
    {
        while(cur != nullptr)
        {
            cout << cur->value << " "; // 访问节点
            nodeStack.push(cur);
            cur = cur->left;
        }
        cur = nodeStack.top();

        /* 右子树存在并且没有访问过，那么当前栈顶元素肯定被访问过，需要将栈顶元素继续弹出 */
        if(cur->right && pre != cur->right)
        {
            nodeStack.pop(); // 先序遍历，这句代码可有可无
            cur = cur->right;
        }
        /* 右子树不存在或者已经被访问过，栈顶元素弹出 */
        else
        {
            nodeStack.pop();
            pre = cur;
            cur = nullptr;
        }
    }       
}

void postorderTraversal(TreeNode* root)
{
    TreeNode* cur = root;
    TreeNode* pre = nullptr;
    stack<TreeNode*> nodeStack;
    while(cur != nullptr || !nodeStack.empty())
    {
        while(cur != nullptr)
        {
            nodeStack.push(cur);
            cur = cur->left;
        }
        cur = nodeStack.top();
        if(cur->right != nullptr && pre != cur->right)
        {
            cur = cur->right;
        }
        else
        {
            cout << cur->value << " ";
            nodeStack.pop();
            pre = cur;
            cur = nullptr;
        }

    }
}

bool isSubtree(TreeNode* root1, TreeNode* root2)
{
    if(root1 == NULL)
        return true;
    if(root2 == NULL)
        return true;

    bool left, right;
    if(root2->value == root1->value)
    {
        return isSubtree(root1->left, root2->left) && isSubtree(root1->right, root2->right);
    }
    else if(root2->value < root1->value)
    {
        return isSubtree(root1->left, root2);
    }
    else
        return isSubtree(root1->right, root2);
}

/**
 * 非递归中序遍历
 */
std::vector<int> InorderTraverse(TreeNode* root)
{
    if(root == nullptr)
        return std::vector<int>();

    std::vector<int> v;
    std::stack<TreeNode*> S;
    S.push(root); //根节点入栈
    while(!S.empty())
    {
        TreeNode* p = S.top();
        while(p != nullptr) //沿着左子树一直往左遍历并压入栈
        {
            S.push(p->left);
            p = p->left;
        }

        S.pop();
        if(!S.empty())
        {
            p = S.top();
            /* 访问节点，并将访问过的节点弹出栈 */
            S.pop();
            v.push_back(p->value);
            cout << p->value << " ";
            /* 继续向右探索，右子树根节点压入栈 */
            S.push(p->right);
        }

    }
    cout << endl;
    return v;
}

/**
 * 非递归前序遍历，每次访问栈顶元素，同时将栈顶元素的右孩子，左孩子
 * 一次入栈。每次循环都是左孩子处在栈顶
 */
std::vector<int> PreorderTraverse(TreeNode* root)
{
    if(root == nullptr)
        return std::vector<int>();

    std::vector<int> v;
    std::stack<TreeNode*> S;
    S.push(root);
    v.push_back(root->value);

    while(!S.empty())
    {
        TreeNode* p = S.top();
        S.pop();
        v.push_back(p->value);
        cout << p->value << " ";
        
        if(p->right != nullptr)
            S.push(p->right);
        if(p->left != nullptr)
            S.push(p->left);
    }

    cout << endl;
    return v;
}

void PostorderTraverse(TreeNode* root)
{
    if(root == nullptr)
        return;

    std::stack<TreeNode*> S;
    S.push(root);
    TreeNode* pre = nullptr;
    while(!S.empty())
    {
        TreeNode* cur = S.top();
        if((cur->left == nullptr && cur->right == nullptr) ||
            (pre != nullptr && (cur->left == pre || cur->right == pre)))
        {
            cout << cur->value << " ";
            S.pop();
            pre = cur;
        }
        else
        {
            if(cur->right != nullptr)
                S.push(cur->right);
            if(cur->left != nullptr)
                S.push(cur->left);
        }
    }

    cout << endl;
}

string printPath(const std::vector<int>& v)
{
    stringstream ss;
    int length = v.size();
    for (int i = 0; i < length; ++i)
    {
        if(i == length - 1)
            ss << v[i];
        else
            ss << v[i] << "->";
    }
    
    return ss.str();
}

vector<vector<int>> pathSum(TreeNode* root, int sum)
{
    std::vector<vector<int>> ret;
    if(root == nullptr)
        return ret;

    int tmp = 0;
    std::vector<int> path;
    std::stack<TreeNode*> S;
    TreeNode* pre = nullptr;
    TreeNode* cur = root;

    while(!S.empty() || cur != nullptr)
    {
        while(cur != nullptr)
        {
            S.push(cur);
            path.push_back(cur->value);
            tmp += cur->value;
            cur = cur->left;
        }

        cur = S.top();
        if(cur->left == nullptr && cur->right == nullptr)
        {
            if(tmp == sum)
                ret.push_back(path);
        }

        if(cur->right != nullptr && cur->right != pre)
        {
            cur = cur->right;
        }
        else
        {
            S.pop();
            path.pop_back();
            tmp -= cur->value;
            pre = cur;
            cur = nullptr;
        }
    }

    return ret;

}

void PrintAllPath(TreeNode* root)
{
    if(root == nullptr)
        return;

    std::vector<int> path;
    std::stack<TreeNode*> S;
    TreeNode* pre = nullptr;
    TreeNode* cur = root;
    while(!S.empty() || cur != nullptr)
    {
        while(cur != nullptr)
        {
            S.push(cur);
            path.push_back(cur->value);
            cur = cur->left;
        }

        cur = S.top();
        if(cur->left == nullptr && cur->right == nullptr)
        {
            cout << printPath(path) << endl;
        }

        if(cur->right != nullptr && cur->right != pre)
        {
            cur = cur->right;
        }
        else
        {
            S.pop();
            path.pop_back();
            pre = cur;
            cur = nullptr;
        }

    }
}

TreeNode* invertTree(TreeNode* root) 
{
    if(root == nullptr)
        return root;
    if(root->left == nullptr && root->right == nullptr)
        return root;

    std::swap(root->left, root->right);
    invertTree(root->left);        
    invertTree(root->right);

    return root;        
}

int main(int argc, char const *argv[])
{
    TreeNode* tree = createTree();

    // preorderTraversal(tree);

    PreorderTraverse(tree);
    // InorderTraverse(tree);
    // PostorderTraverse(tree);
    PrintAllPath(tree);
    // invertTree(tree);
    // PreorderTraverse(tree);
    std::vector<vector<int>> v = pathSum(tree,10);
    for (int i = 0; i < v.size(); ++i)
    {
        for(auto j : v[i])
            cout << j << " ";
        cout << endl;
    }
    destroyTree(tree);
    return 0;
}