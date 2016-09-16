#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <set>
#include <string>

using namespace std;

struct ListNode
{
	int val;
	ListNode* next;
	ListNode(int _val):
	val(_val), next(nullptr) {}
};

ListNode* createList(const std::vector<int>& v)
{
	if(v.empty())
		return nullptr;
	ListNode* head = new ListNode(v[0]);
	ListNode* cur = head;
	for (int i = 1; i < v.size(); ++i)
	{
		cur->next = new ListNode(v[i]);
		cur = cur->next;
	}
	cur->next = head;

	return head;
}

void deleteList(ListNode*& head)
{
	if(head == nullptr)
		return;
	ListNode* cur = head;
	ListNode* tmp;
	unordered_set<ListNode*> hash;

	while(cur != nullptr)
	{
		tmp = cur;
		cur = cur->next;
		if(!hash.count(tmp))
		{
			hash.insert(tmp);
			delete tmp;
		}
		else
			cur = nullptr;
	}

	head = nullptr;
}
void printList(ListNode* head)
{
	ListNode* cur = head;
	unordered_set<ListNode*> hash;
	while(cur != nullptr)
	{
		if(!hash.count(cur))
		{
			hash.insert(cur);
			std::cout << cur->val << " -> ";
			cur = cur->next; 
		}
		else
			cur = nullptr;
	}
	std::cout << std::endl;
}

/**
 * 判断链表是否有环
 * @param  head       [description]
 * @param  circleNode [快指针和慢指针相交的节点]
 * @return            [description]
 */
bool isCircle(ListNode* head, ListNode*& circleNode, ListNode*& lastNode)
{
	if(head == nullptr)
		return false;
	ListNode* slow = head;
	ListNode* fast = head;
	while(fast && fast->next)
	{
		slow = slow->next;
		fast = fast->next->next;
		if(fast == slow)
		{
			circleNode = fast;
			return true;
		}
	}

	circleNode = nullptr;
	lastNode = fast;
	return false;
}

bool isIntersect(ListNode* list1, ListNode* list2)
{
	ListNode* circleNode1 = nullptr;
	ListNode* circleNode2 = nullptr;
	ListNode* lastNode1   = nullptr;
	ListNode* lastNode2   = nullptr;
	bool circle1 = isCircle(list1, circleNode1, lastNode1);
	bool circle2 = isCircle(list2, circleNode2, lastNode2);
	if(circle1 != circle2)
		return false;
	else if(!circle1 && !circle2)
	{
		return (lastNode1 == lastNode2);
	}
	else
	{
		ListNode* tmp = circleNode1;
		while(tmp != circleNode1)
		{
			if(tmp == circleNode2)
				return true;
			else
				tmp = tmp->next;
		}

		return false;
	}
}

ListNode* lastKth(ListNode* head, int k)
{
	assert(k > 0);
	if(head == nullptr)
		return nullptr;
	ListNode* p = head;
	ListNode* q = head;
	for (; q != nullptr && k > 0; --k)
	{
		q = q->next;
	}
	/* k大于链表的长度返回nullptr */
	if(k > 0)
		return nullptr;
	while(q != nullptr)
	{
		p = p->next;
		q = q->next;
	}

	return p;
}

void test()
{
	std::vector<int> v {1,2,3,4,5,6,7,8,9,10,11,12};
	ListNode* list1 = createList(v);
	ListNode* list2 = createList(v);
	printList(list1);
	// int k = 9;
	// ListNode* node = lastKth(list, k);
	// if(node != nullptr)
	// 	cout << node->val << endl;
	// else
	// 	cout << "not found" << endl;

	// cout << boolalpha << isCircle(list) << endl;
	cout << boolalpha << isIntersect(list1,list2) << endl;

	deleteList(list1);
	deleteList(list2);
	// printList(list);
}

int main(int argc, char const *argv[])
{
	test();
	return 0;
}
