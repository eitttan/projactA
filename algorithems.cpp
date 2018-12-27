#include "algorithems.h"

random::random(double lambda, test_params& params) : entry(lambda) {}
int random::choose_server(test_params& params, server* servers[]) {
	return (random_number() % params.NUM_OF_SERVERS);
}


round_robin::round_robin(double lambda, test_params& params) : entry(lambda), server_to_choose_(0) {}
int round_robin::choose_server(test_params& params, server* servers[]) {
	server_to_choose_++;
	if (server_to_choose_ == params.NUM_OF_SERVERS) { server_to_choose_ = 0; }
	return server_to_choose_;
}


jsq::jsq(double lambda, test_params& params) : entry(lambda) {
	for (int i = 0; i < params.NUM_OF_SERVERS; i++)
		queue_sizes_.push_back(0);
}
int jsq::choose_server(test_params& params, server * servers[])
{
	int minVal = INT_MAX;
	vector <int> minValArray;
	for (int i = 0; i < params.NUM_OF_SERVERS; i++) {
		int currVal = queue_sizes_[i];
		if (currVal < minVal) {
			minVal = currVal;
			minValArray.clear();
			minValArray.push_back(i);
		}
		if (currVal == minVal) {
			minValArray.push_back(i);
		}

	}
	int minSErverIndex = (random_number() % minValArray.size());
	return minValArray.at(minSErverIndex);
}
void jsq::collect_data(test_params& params, server* servers[]) {
	for (int i = 0; i < params.NUM_OF_SERVERS; i++) {
		queue_sizes_[i] = servers[i]->get_queue_size();
	}
}


jiq::jiq(double lambda, test_params& params) : entry(lambda) {
	for (int i = 0; i < params.NUM_OF_SERVERS; i++)
	{
		idle_servers_.push_back(i);
	}
}
int jiq::choose_server(test_params& params, server* servers[]) {
	if (idle_servers_.empty()) {
		return (random_number() % params.NUM_OF_SERVERS);
	}
	else
	{
		int random_index = random_number() % idle_servers_.size();
		int random_server = idle_servers_[random_index];
		idle_servers_.erase(idle_servers_.begin() + random_index);
		return random_server;
	}
}
void jiq::report_empty_queue(test_params& params, int server_index) {
	for (vector<int>::iterator it = idle_servers_.begin(); it != idle_servers_.end(); ++it) {
		if (*it == server_index)
			return;
	}
	idle_servers_.push_back(server_index);
}


pi::pi(double lambda, test_params& params) : entry(lambda), last_idle_server_(random_number() % params.NUM_OF_SERVERS) {
	for (int i = 0; i < params.NUM_OF_SERVERS; i++)
	{
		idle_servers_.push_back(i);
	}
}
int pi::choose_server(test_params& params, server* servers[]) {
	if (idle_servers_.empty()) {
		return last_idle_server_;
	}
	else
	{
		int random_index = random_number() % idle_servers_.size();
		int random_server = idle_servers_[random_index];
		idle_servers_.erase(idle_servers_.begin() + random_index);
		if (idle_servers_.empty()) {
			last_idle_server_ = random_server;
		}
		return random_server;
	}
}
void pi::report_empty_queue(test_params& params, int server_index) {
	for (vector<int>::iterator it = idle_servers_.begin(); it != idle_servers_.end(); ++it) {
		if (*it == server_index)
			return;
	}
	idle_servers_.push_back(server_index);
}
