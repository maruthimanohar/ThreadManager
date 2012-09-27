#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sched.h>
#include <iostream>

typedef void* (*threadEntry)(void*);
// Thread Managing class.
// An instance of this class will create a new thread and will start its execution.
// The class takes the function pointer and variable arguments that needs to be
// passed to the new created thread.

// Assumptions:
// 1. The created thread will get all the arguments in an void * array.
// 2. The arguments passed by the parent thread will stored by allocating memory
// and pointers to this newly allocated memory will be passed to the child.
// 3. so for example, if the parent thread passes arguments like
//  (int, char, struct st, int *)
//  we will allocate memory to all the variables passed and
//  pass an array of pointers to the child thread.
//  the child thread need to do an extra level of derefernce.
//  so child will get arguments like (int *, char *, struct st*, int **, NULL).

class Thread
{
public:
	template<typename... Args>
	Thread(threadEntry entry, Args... args)
	{
		_arg_count = sizeof...(args);
		std::cout << pthread_self() << ": arg_count " << _arg_count << std::endl;
		_thread_args = (void **)malloc(sizeof(void *) * (_arg_count)+1);
		_thread_args[_arg_count] = NULL;  	// the void ** array that we pass to the
							// created thread will be null terminated.
		if (_arg_count > 0) {
			store_args(_thread_args, args...); // store all the thread arguments
						  	   // in a void ** array.
		}

		_joined = false;
		pthread_mutex_init(&_lock, 0);
		pthread_create(&_tid, 0, entry, (void *)_thread_args);
		sched_yield();  // The thread is moved to the end of the queue for its
				// static priority and a new thread gets to run.
	}

	~Thread()
	{
		join();    // join the thread before the object is deleted.
			   // so the created thread will have access to all the parameters.
		int i = 0;
		for (i=0; i<_arg_count; i++)
			free(_thread_args[i]);
		free(_thread_args);
	}

	// join the thread only if the the thread was not joined already.
	void join() /* optional: implement only if you have time */
	{
		pthread_mutex_lock(&_lock);
		if (!_joined) {
			pthread_join(_tid, 0);
			_joined = true;
		}
		pthread_mutex_unlock(&_lock);
	}

	template<typename T, typename... Args>
	static void
	store_args(void **thread_args, T& val, Args... args)
	{
		T* new_val  = (T*) malloc(sizeof(T));
		memcpy(new_val, &val, sizeof(T));
		thread_args[0] = (void *)new_val;
		if (sizeof...(args) == 0)
        		return;
		store_args(thread_args+1, args...);
	}

	template<typename... Args>
	static void
	store_args(void **thread_args, Args... args)
	{
		return;
	}

private:
	int _arg_count;
	void **_thread_args;
	pthread_t _tid;
	pthread_mutex_t _lock;  // lock to protect joining.
	bool _joined;           // boolean to keep track if the thread is joined or not
				// because we need to call join only once, and should not have
				// any effect in destructor, if join was already called.
};
