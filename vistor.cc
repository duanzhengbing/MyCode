
#include <iostream>
#include <list>
#include <string>
using namespace std;

class Blog;
/**
 * 观察者接口
 */
class Observer
{
public:
    virtual void Update(Blog* p_blog) = 0;
    virtual ~Observer() {}
};

/**
 * 抽象通知者接口
 */
class Blog
{
public:
    //注册观察者对象的接口
    virtual void Attach(Observer *observer) = 0;
    //删除观察者对象的接口
    virtual void Remove(Observer* observer) = 0;
    //通知接口
    virtual void Notify() = 0;
    virtual void SetStatus(string s) = 0;
    virtual std::string GetStatus() = 0;
    virtual std::string GetName() = 0;
    virtual ~Blog() {}
};

/**
 * 具体通知者，保存了所有与之相关的或者登记过的观察者，一旦通知者
 * 状态变化，将调用Notify逐个通知登记列表中的观察者
 */
class BlogSubject : public Blog
{
public:
    BlogSubject(string name):m_name(name) {}
    virtual ~BlogSubject() {}

    virtual void Attach(Observer *observer)
    {
        m_observers.push_back(observer);
    }
    virtual void Remove(Observer* observer)
    {
        m_observers.remove(observer);
    }
    virtual void Notify()
    {
        list<Observer*>::iterator iter = m_observers.begin();
        for(;iter != m_observers.end(); iter++)
            (*iter)->Update(this);
    }
    virtual void SetStatus(string s) { m_status = s; }
    virtual std::string GetStatus() { return m_status; }
    virtual std::string GetName() { return m_name; }

private:
    std::string m_status;
    list<Observer* > m_observers;
    string m_name;
};

class ObserverBlog : public Observer
{
public:
    ObserverBlog(string name):m_name(name) {}
    virtual ~ObserverBlog() {}
    virtual void Update(Blog* p_blog)
    {
        std::string status = p_blog->GetStatus();
        cout << m_name  <<" : "<< p_blog->GetName() << status <<endl;
    }
private:
    std::string m_name;
};

int main()
{
    Blog *blog = new BlogSubject("onlycxue");
    Observer *observer1 = new ObserverBlog("Cristiano");
    Observer *observer2 = new ObserverBlog("duan");
    blog->Attach(observer1);
    blog->Attach(observer2);


    blog->SetStatus("发表博客！");
    blog->Notify();
    delete blog;
    delete observer1;
    delete observer2;
    return 0;
}