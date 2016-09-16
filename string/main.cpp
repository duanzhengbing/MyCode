#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;


int main()
{
    int N;
    while(cin >> N)
    {
        map<int,vector<int>> mp;
        vector<int> qus_id;
        for(int i = 0; i < N; ++i)
        {
            int quas_id, ans_count;
            cin >> quas_id >> ans_count;
            qus_id.push_back(quas_id);
            for(int j = 0; j < ans_count; ++j)
            {
                int ans_id;
                cin >> ans_id;
                mp[quas_id].push_back(ans_id);
            }
        }

        set<int> breakRule_id;
        for(int i = 0; i < N; ++i)
        {
            for(int j = 0; j < mp[qus_id[i]].size(); ++j)
            {
                for(int k = 0; k < mp[mp[qus_id[i]][j]].size(); ++k)
                {
                    if(qus_id[i] == mp[mp[qus_id[i]][j]][k])
                    {
                        breakRule_id.insert(qus_id[i]);
                        breakRule_id.insert(mp[qus_id[i]][j]);
                    }
                }
            }
        }
        cout << "size : " << breakRule_id.size() << endl;
        for(auto i : breakRule_id)
            cout << i << endl;

        for(int i = 0; i < N; ++i)
        {
            int num = 0;
            for(int j = 0; j < mp[qus_id[i]].size(); ++j)
            {
                if(find(breakRule_id.begin(),breakRule_id.end(),mp[qus_id[i]][j]) != breakRule_id.end())
                {
                    num++;
                }
                if(num >= 2)
                {
                    breakRule_id.insert(qus_id[i]);
                    break;
                }
            }
        }
        cout << "size : " << breakRule_id.size() << endl;
        for(auto i : breakRule_id)
            cout << i << endl;
    }
    return 0;
}
