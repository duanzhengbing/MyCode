//#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <cstdio>

using namespace std;

struct RandomListNode
{
	int label;
	RandomListNode* next;
	RandomListNode* random;
	RandomListNode(int v):label(v),next(nullptr),random(nullptr) {}
	
};

RandomListNode* createList()
{
	RandomListNode* head = new RandomListNode(0);
	RandomListNode* cur = head;
	for (int i = 0; i < 10; ++i)
	{
		cur->next = new RandomListNode(rand() % 20);
		cur = cur->next;
	}

	head->random = head->next->next;

	return head;
}

void destroyList(RandomListNode* list)
{
	RandomListNode* cur = list;
	while(cur != nullptr)
	{
		RandomListNode* tmp = cur;
		cur = cur->next;
		delete tmp;
	}
}

void printList(RandomListNode* list)
{
	RandomListNode* cur = list;
	while(cur != nullptr)
	{
		if(cur->next == nullptr)
		{
			cout << cur->label << endl;
			break;
		}
		else
			cout << cur->label << " -> ";
		cur = cur->next;
	}
}

RandomListNode* deepCopy(RandomListNode* list)
{
	if(list == nullptr)
		return nullptr;
	RandomListNode* newList = new RandomListNode(list->label);
	RandomListNode* newcur = newList;
	RandomListNode* cur = list->next;

	while(cur != nullptr)
	{
		newcur->next = new RandomListNode(cur->label);
		newcur = newcur->next;
		cur = cur->next;
	}

	return newList;
}

RandomListNode* copyRandomList(RandomListNode *list) 
{
	if(list == nullptr)
		return nullptr;
	RandomListNode* newList = new RandomListNode(list->label);
	RandomListNode* newcur = newList;
	RandomListNode* cur = list;
	unordered_map<RandomListNode*,RandomListNode*> hash;
	hash.insert({list,newList});
	while(cur != nullptr)
	{
		if(cur->random == nullptr)
			newcur->random = nullptr;
		else
		{
			auto iter = hash.find(cur->random);
			if(iter == hash.end())
			{
				newcur->random = new RandomListNode(cur->random->label);
				hash.insert({cur->random,newcur->random});
			}
			else
				newcur->random = iter->second;
		}

		if(cur->next == nullptr)
			newcur->next = nullptr;
		else
		{
			auto iter = hash.find(cur->next);
			if(iter == hash.end())
			{
				newcur->next = new RandomListNode(cur->next->label);
				hash.insert({cur->next,newcur->next});
			}
			else
				newcur->next = iter->second;			
		}

		newcur = newcur->next;
		cur = cur->next;
		
	}

	return newList;
}

int singleNumber(vector<int>& nums) 
{
    for(unsigned i = 1; i < nums.size(); ++i) 
    	nums[0] ^= nums[i];
    return nums[0];    
}


int main(int argc, char const *argv[])
{
	RandomListNode* list = createList();
	printList(list);

	RandomListNode* newlist = copyRandomList(list);
	printList(newlist);

	destroyList(newlist);
	destroyList(list);


	return 0;
}