#include <iostream>
#include <log4cpp/Appender.hh>
#include <log4cpp/Category.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/RollingFileAppender.hh>

using std::cout;
using std::endl;
using namespace log4cpp;
using std::string;



//单例模式的log类
class Mylogger
{
public:
    //3、定义一个返回值为类指针的静态成员函数
    //因为要使用类名::获取指针，只能通过static成员函数，不能创建对象调用
    static Mylogger* getInstance();
    static void destory();
    void initModule();
    void warn(const char *msg);
	void error(const char *msg);
	void debug(const char *msg);
	void info(const char *msg);

private:
    //1、将构造函数私有化
    Mylogger()
    :_root(Category::getRoot())
    ,_module(_root.getInstance("c++"))
    {
        cout << "Mylogger()" << endl;
        initModule();
    }
    ~Mylogger()
    {
        cout << "~Mylogger()" << endl;
    }

private:
    //2、在类中定义一个静态类对象指针
    static Mylogger* _pInstance;    //静态成员函数只能访问静态成员，所以这里也要用static的
    Category & _root;       //引用的初始化要放在构造函数中
    Category & _module;
};

Mylogger* Mylogger::_pInstance = nullptr;   //静态成员只能在类外初始化

//三种输出方式
//1、标准输出，2、输出到文件 3、输出到回卷文件
//每个module初始化自己文件，后面可以封装一个类为基类，初始化root，其他的模块创建不同的module
void Mylogger::initModule()
{
    //创建布局，设置布局
    PatternLayout * mylayout1 = new PatternLayout();
    mylayout1->setConversionPattern("%d [%c] [%p] %m%n");

    PatternLayout * mylayout2 = new PatternLayout();
    mylayout2->setConversionPattern("%d [%c] [%p] %m%n");

    PatternLayout * mylayout3 = new PatternLayout();
    mylayout3->setConversionPattern("%d [%c] [%p] %m%n");
    //创建目的地对象，这里设置为标准输出
    OstreamAppender * osAppender = new OstreamAppender("console", &cout);
    //设置目的地的布局
    osAppender->setLayout(mylayout1);

    //不同类型的Appender设置Layout时，不能使用相同的，必须要再建立一个Layout对象
    FileAppender* fileAppender = new FileAppender("fileApp", "mylog.log");
    fileAppender->setLayout(mylayout2);

    //设置回卷日志，每个大小5000k，9个文件
    RollingFileAppender * rollingFileAppender = new RollingFileAppender("rollingFileApp",
                                                "rollingmy.log", 5000, 9); 
    rollingFileAppender->setLayout(mylayout3);

    //4. 设置好优先级
    //用module设置，这样就可以一个root有多个module了，各个module使用各自的日志
	_module.setPriority(Priority::DEBUG);// 给Category对象设置优先级
	_module.setAppender(osAppender);
	_module.addAppender(fileAppender);
	_module.addAppender(rollingFileAppender);

}


//声明的时候使用static，定义的时候就不需要了
void Mylogger::destory()
{
    if(_pInstance)
    {
        delete _pInstance;
        _pInstance = nullptr;

    test1();    }
}

Mylogger* Mylogger::getInstance()
{
    if(_pInstance == nullptr)
    {
        _pInstance = new Mylogger();
    }
    return _pInstance;
}

void Mylogger::warn(const char *msg)
{
    _module.warn(msg);
}
void Mylogger::error(const char *msg)
{
    _module.error(msg);
}
void Mylogger::debug(const char *msg)
{
    _module.debug(msg);
    test1();
}
void Mylogger::info(const char *msg)
{
    _module.info(msg);
}


#define prefix(msg) string("[").append(__FILE__).append(":").\
                                append(__FUNCTION__).append(":").\
                                append(std::to_string(__LINE__)).\
                                append("]").append(msg).c_str()


#define logDebug(msg)   Mylogger::getInstance()->debug(prefix(msg))
#define logError(msg)   Mylogger::getInstance()->error(prefix(msg))
#define logInfo(msg)   Mylogger::getInstance()->info(prefix(msg))
#define logWarn(msg)   Mylogger::getInstance()->warn(prefix(msg))


void test1()
{
    logDebug("hahahahhaha");
    test1();
    logWarn("hahahahhaha");
    logError("hahahahhaha");
    logInfo("hahahahhaha");
}


void test0()
{
    Mylogger* logger = Mylogger::getInstance();
    logger->debug(prefix("sdafsdfasdfasfd"));
    logger->warn(prefix("sdafsdfasdfasfd"));
    logger->error(prefix("sdafsdfasdfasfd"));
    logger->info(prefix("sdafsdfasdfasfd"));
    logger->destory();

}


int main()
{
    //test0();
    test1();
    return 0;
}
