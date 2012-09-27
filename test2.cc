#include <iostream>
#include <assert.h>

#include "thread.h"

struct st{
	int a;
	int b;
	int c;
};

class ThreadTest
{
public:
	ThreadTest() : testCount(0) { }
	void run()
	{
		std::cout << pthread_self() << ": Parent thread ID: " << pthread_self() << std::endl;
		Thread child(ThreadTest::testEntry);
		std::cout << pthread_self() << ": Parent waiting for the child to terminate" << std::endl;
		child.join();
		std::cout << pthread_self() << ": Child terminated. So returning from the parent." << std::endl;
		assert(testCount == 0);
	}
private:
	int testCount;

	static void *testEntry(void *v)
	{
		std::cout << pthread_self() << ": In test entry code" << std::endl;
		void **thread_args = ( void **)v;

		if (thread_args[0] != NULL) {
			std::cout << pthread_self() << ": Thread test not null" << std::endl;
			ThreadTest *thread_test = *(ThreadTest **)thread_args[0];
			thread_test->testRun();
		} else {
			std::cout << pthread_self() << ": Thread test is null" << std::endl;
		}

		std::cout << pthread_self() << ": Child terminating" << std::endl;
	}

	void testRun()
	{
		std::cout << pthread_self() << ": Child thread ID: " << pthread_self() << std::endl;
		++testCount;
	}
};

int main()
{
	ThreadTest test;
	test.run();
	return 0;
}
