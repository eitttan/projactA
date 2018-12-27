#ifndef CLASSES_H_
#define CLASSES_H_
#include "defs.h"

class job
{
public:
	job(int time);
	~job();
	static int get_num_of_jobs_in_system();
	void report_completion(test_params& params, int time);
protected:
	int income_time_;
	static int jobs_created_;
	static int jobs_in_system_;
};

class server
{
public:
	server(int index,double lambda);
	~server();
	void accept_job(job* incoming_job);
	void process_job(test_params& params, int time, entry* entries[], ofstream& log_file);
	int get_queue_size() const;
	void print() const;
protected:
	int server_index_;
	int completed_jobs_;
	std::geometric_distribution<int> completion_time_;
	queue<job*> waiting_jobs_;
	int time_to_finish_job_;
	
};

class entry
{
public:
	entry(double lambda);
	void accept_jobs(test_params& params, server* servers[], int time);
	virtual int choose_server(test_params& params, server* servers[]) = 0;
	virtual void report_empty_queue(test_params& params, int server_index) {};
	virtual void collect_data(test_params& params, server* servers[]) {};
protected:
	std::poisson_distribution<int> income_rate_;
};

class test_params
{
public:
	test_params(ALGORITHEM a, TEST_SCALE ts, SERVER_UNITY su, SERVER_SPEED_DISTRIBUTION ssd, ENTRIES_TO_SERVERS_RATIO esr);
	~test_params();
	ALGORITHEM ENTRY_ALGORITHEM;
	int NUM_OF_ENTRIES;
	float* ENTRY_PARAMS;
	int NUM_OF_SERVERS;
	float* SERVER_PARAMS;
	ofstream log_file;
};

#endif // !CLASSES_H_