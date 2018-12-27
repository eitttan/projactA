#include "classes.h"

job::job(int time) : income_time_(time) { jobs_created_++; jobs_in_system_++; }//cout << setfill(' ') << setw(4) << time << " : job created" << endl; }
job::~job() { jobs_in_system_--; }
int job::get_num_of_jobs_in_system() { return jobs_in_system_; }
void job::report_completion(test_params& params, int time) {
	params.log_file << time - income_time_ << endl;
}
int job::jobs_in_system_ = 0;
int job::jobs_created_ = 0;


server::server(int index, double lambda) : server_index_(index) ,completion_time_(lambda), time_to_finish_job_(completion_time_(random_generator)), completed_jobs_(0) {}
server::~server() {
	//cout << "server " << server_index_ << " completed " << completed_jobs_ << " jobs. finised with " << waiting_jobs_.size() << " uncompleted jobs in its queue" << endl;
	while (!waiting_jobs_.empty()) {
		delete waiting_jobs_.front();
		waiting_jobs_.pop();
	}
}
void server::accept_job(job* incoming_job) { waiting_jobs_.push(incoming_job); }
void server::process_job(test_params& params, int time, entry* entries[] , ofstream& log_file) {
	if (!waiting_jobs_.empty()) {
		if (time_to_finish_job_ == 0) {
			time_to_finish_job_ = completion_time_(random_generator);
			(waiting_jobs_.front())->report_completion(params, time);
			delete waiting_jobs_.front();
			waiting_jobs_.pop();
			completed_jobs_++;
			if (get_queue_size() == 0) {
				entries[random_number() % params.NUM_OF_ENTRIES]->report_empty_queue(params ,server_index_);
			}
		}
		else {
			time_to_finish_job_--;
		}
	}
}
int server::get_queue_size() const { return waiting_jobs_.size(); }
void server::print() const {
	cout << "DEBUG - server - time left: " << time_to_finish_job_ << " queue size: " << waiting_jobs_.size() << endl;
}


entry::entry(double lambda) : income_rate_(lambda) {}
void entry::accept_jobs(test_params& params, server* servers[], int time) { //true if a job was accepted
	int incoming_jobs = income_rate_(random_generator);
	for (int i = 0; i < incoming_jobs; i++)
	{
		job* new_job = new job(time);
		(servers[choose_server(params, servers)])->accept_job(new_job);
	}
}


test_params::test_params(ALGORITHEM a, TEST_SCALE ts, SERVER_UNITY su, SERVER_SPEED_DISTRIBUTION ssd, ENTRIES_TO_SERVERS_RATIO esr) {
	char log_name[30];
	sprintf(log_name, "%d_%d_%d_%d_%f_log.txt", a, ts, su, ssd, esr);
	//printf("%s\n", log_name);//DEBUG
	log_file.open(log_name);
	
	if (esr <= 0 || esr > 1) {
		printf("ERROR - ENTRIES_TO_SERVERS_RATIO isn't between 0 and 1! (esr=%f)", esr);
		log_file << "ERROR - ENTRIES_TO_SERVERS_RATIO isn't between 0 and 1! esr=" << esr << endl;
		log_file.close();
		exit(1);
	}
	ENTRY_ALGORITHEM = a;
	NUM_OF_ENTRIES = (ts == LARGE) ? 10 : 1;
	NUM_OF_SERVERS = (ts == LARGE) ? 100 : 10;
	ENTRY_PARAMS = new float [NUM_OF_ENTRIES];
	SERVER_PARAMS = new float [NUM_OF_SERVERS];
	//float entry_sum = 0, server_sum = 0;//DEBUG
	for (int i = 0; i < NUM_OF_ENTRIES; i++)
	{
		ENTRY_PARAMS[i] = (float)1.8;
		//entry_sum += ENTRY_PARAMS[i];//DEBUG
		//printf("%f + ", ENTRY_PARAMS[i]);//DEBUG
	}
	//printf(" = %f (entry)\n", entry_sum);//DEBUG
	int fast_servers_num, slow_servers_num;
	if (su == HOMOGENEOUS) {
		fast_servers_num = 0;
	}
	else {
		switch (ssd) {
			case FAST_9_SLOW_1: fast_servers_num = 9; break;
			case FAST_5_SLOW_5: fast_servers_num = 5; break;
			case FAST_1_SLOW_9: fast_servers_num = 1; break;
		default: break;
		}
	}
	slow_servers_num = 10 - fast_servers_num;
	int speed_ratio;
	switch (su) {
		case HOMOGENEOUS:		speed_ratio = 1; break;
		case HETEROGENEOUS_X2:	speed_ratio = 2; break;
		case HETEROGENEOUS_X10: speed_ratio = 10; break;
	default: break;
	}
	float fast_server_speed, slow_server_speed;
	slow_server_speed = ENTRY_PARAMS[0] / (esr * (fast_servers_num*speed_ratio+slow_servers_num));
	fast_server_speed = slow_server_speed * speed_ratio;
	if (fast_server_speed > 1) {
		printf("ERROR - SERVER_PARAMS isn't between 0 and 1! (=%f)", fast_server_speed);
		log_file << "ERROR - SERVER_PARAMS isn't between 0 and 1! =" << fast_server_speed << endl;
		log_file.close();
		exit(1);
	}
	if (ts == LARGE)
		fast_servers_num *= 10;
	int i = 0;
	for ( ; i < fast_servers_num; i++)
	{
		SERVER_PARAMS[i] = fast_server_speed;
		//server_sum += SERVER_PARAMS[i];//DEBUG
		//printf("%f + ", SERVER_PARAMS[i]);//DEBUG
	}
	for ( ; i < NUM_OF_SERVERS; i++)
	{
		SERVER_PARAMS[i] = slow_server_speed;
		//server_sum += SERVER_PARAMS[i];//DEBUG
		//printf("%f + ", SERVER_PARAMS[i]);//DEBUG
	}
	//printf(" = %f (server)\n", server_sum);//DEBUG
	//printf("ratio = %f\n", entry_sum/server_sum);//DEBUG
}
test_params::~test_params() {
	log_file.close();
	delete[] ENTRY_PARAMS;
	delete[] SERVER_PARAMS;
}