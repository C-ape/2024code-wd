#include <iostream>

#include <log4cpp/Category.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/RollingFileAppender.hh>

using std::cout;
using std::endl;
using namespace log4cpp;
 
void test0() 
{
	//0. 创建PatternLayout对象
	PatternLayout * ptnLayout = new PatternLayout();
	ptnLayout->setConversionPattern("%d [%c] [%p] %m%n");

	PatternLayout * ptnLayout2 = new PatternLayout();
	ptnLayout2->setConversionPattern("%d [%c] [%p] %m%n");

	PatternLayout * ptnLayout3 = new PatternLayout();
	ptnLayout3->setConversionPattern("%d [%c] [%p] %m%n");

	//1. 创建目的地对象
	OstreamAppender * posAp = new OstreamAppender("console", &cout);
	//2. 设置目的地的日志布局
	posAp->setLayout(ptnLayout);

	FileAppender * fileApp = new FileAppender("fileApp", "wd.log");
	//不同的Appender设置布局时，不能使用相同的，必须要
	//再建立一个新的Layout对象
	fileApp->setLayout(ptnLayout2);

	auto rollingFileApp = new RollingFileAppender("rollingFileApp",
			"rollingwd.log",
			5000,
			9);
	rollingFileApp->setLayout(ptnLayout3);

	//3. 创建Category对象
	Category & root = Category::getRoot();
	//4. 设置好优先级
	root.setPriority(Priority::DEBUG);// 给Category对象设置优先级
	root.setAppender(posAp);
	root.addAppender(fileApp);
	root.addAppender(rollingFileApp);
	Category & module1 = root.getInstance("register");

	for(int idx = 0; idx < 100; ++idx) {
	//5. 进行日志记录
		module1.emerg("this is an emerge message");//本条日志的级别是emerg级别
		module1.alert("this is an alert message");
		module1.crit("this is a crit message");
		//本条日志的级别小于Category的优先级级别，就会被过滤掉
		module1.error("this is an error message");
		module1.warn("this is a warn message");
		module1.notice("this is a notice message");
		module1.info("this is an info message");
		module1.debug("this is a debug message");
		module1.debug("xxxxxx");
	}

	//6. 释放申请资源,回收日志系统
	Category::shutdown();
} 
 
int main(void)
{
	test0();
	return 0;
}

