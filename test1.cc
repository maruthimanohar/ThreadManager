#include <iostream>
#include <assert.h>
#include <stdio.h>

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

	void run(int val1, int *a, std::string str, char *ch, struct st& s)
	{
		std::cout << pthread_self() << ": Parent thread ID: " << pthread_self() << std::endl;
		std::cout << pthread_self() << ": Value of a before calling child " << *a << std::endl;
		Thread child(ThreadTest::testEntry, this, val1, a, str, ch, s);
		std::cout << pthread_self() << ": Parent waiting for the child to terminate" << std::endl;
		child.join();
		std::cout << pthread_self() << ": Child terminated." << std::endl;
		std::cout << pthread_self() << ": Value of a after child terminated " << *a << std::endl;
		assert(testCount == 1);
	}
	
private:
	int testCount;

	static void *testEntry(void *v)
	{
		std::cout << pthread_self() << ": In test entry code" << std::endl;
		void **thread_args = ( void **)v;

		if (thread_args[0] != NULL) {
			ThreadTest *thread_test = *(ThreadTest **)thread_args[0];
			thread_test->testRun();
		} else {
			std::cout << pthread_self() << ": Thread test is null" << std::endl;
		}
		std::cout << pthread_self() << ": Printing the thread arguments" << std::endl;
		int *val1 = (int *)thread_args[1];
		std::cout << pthread_self() << ": val1= " << *val1 << std::endl;
		
		int **a = (int **)thread_args[2];
		std::cout << pthread_self() << ": a = " << **a << std::endl;
		**a = 20;
		
		std::string *str = (std::string*)thread_args[3];
		std::cout << pthread_self() << ": str = " << *str << std::endl;
		
		char **ch = (char **)thread_args[4];
		std::cout << pthread_self() << ": ch = " << *ch << std::endl;

		struct st* s = (struct st *)thread_args[5];
		std::cout << pthread_self() << ": s.a = " << s->a << " s.b " << s->b << " s.c " << s->c << std::endl;
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
	struct st s;
	s.a = 1;
	s.b = 2;
	s.c = 3;
	int a = 10;
	std::string str = "string";
	char ch[10] = "abcdefgh";
	test.run(100, &a, str, ch , s);
	return 0;
}
