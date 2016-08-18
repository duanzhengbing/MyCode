#include <iostream>
#include <vector>
#include <deque>
#include <stack>
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
    TreeNode* root = new TreeNode(0);
    root->left = new TreeNode(1);
    root->right = new TreeNode(2);
    root->left->left = new TreeNode(3);
    root->left->right = new TreeNode(4);
    root->right->left = new TreeNode(5);
    root->right->right = new TreeNode(6);
    root->left->left->right = new TreeNode(7);
    root->left->right->left = new TreeNode(9);
    root->left->right->right = new TreeNode(10);
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



int main(int argc, char const *argv[])
{
    TreeNode* tree = createTree();
    printFromTopToBottom(tree);
    // inorderTraversal(tree);
    postorderTraversal(tree);
    cout << endl;
    /*std::vector<vector<int>> pathSet = findAllPathNoRecursion(tree);
    for (unsigned i = 0; i < pathSet.size(); ++i)
    {
        for(auto v : pathSet[i])
            std::cout << v << " ";
        cout << endl;
    }*/

/*  std::vector<vector<int>> ret = pathOfSum(tree,14);
    for (unsigned i = 0; i < ret.size(); ++i)
    {
        for(auto v : ret[i])
            std::cout << v << " ";
        cout << endl;
    }
    cout << minDepth(tree) << endl;*/
    // cout << minDepthRecursion(tree) << endl;
    
    preorderTraversal(tree);

    destroyTree(tree);
    return 0;
}