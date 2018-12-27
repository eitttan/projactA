#include "classes.h"
#include "algorithems.h"

float simulate(test_params& params) {

	entry** entries = new entry*[params.NUM_OF_ENTRIES];
	server** servers = new server*[params.NUM_OF_SERVERS];
	switch (params.ENTRY_ALGORITHEM)
	{
	case RANDOM: 
		for (int i = 0; i < params.NUM_OF_ENTRIES; i++)
			entries[i] = new random(params.ENTRY_PARAMS[i], params);
		break;
	case JSQ:
		for (int i = 0; i < params.NUM_OF_ENTRIES; i++)
			entries[i] = new jsq(params.ENTRY_PARAMS[i], params);
		break;
	case JIQ:
		for (int i = 0; i < params.NUM_OF_ENTRIES; i++)
			entries[i] = new jiq(params.ENTRY_PARAMS[i], params);
		break;
	case PI:
		for (int i = 0; i < params.NUM_OF_ENTRIES; i++)
			entries[i] = new pi(params.ENTRY_PARAMS[i], params);
		break;
	default: break;
	}
	for (int i = 0; i < params.NUM_OF_SERVERS; i++)
		servers[i] = new server(i, params.SERVER_PARAMS[i]);
	
	unsigned int sum_of_jobs = 0;

	for (int time = 1; time <= SIMULATION_TIME; time++) {
		for (int i = 0; i < params.NUM_OF_ENTRIES; i++) {
			entries[i]->collect_data(params, servers);
		}
		for (int i = 0; i < params.NUM_OF_ENTRIES; i++) {
			entries[i]->accept_jobs(params, servers,time);
		}
		for (int i = 0; i < params.NUM_OF_SERVERS; i++) {
			servers[i]->process_job(params, time,entries, params.log_file);
			sum_of_jobs += servers[i]->get_queue_size();
		}
	}

	for (int i = 0; i < params.NUM_OF_ENTRIES; i++)
	{
		delete entries[i];
	}
	for (int i = 0; i < params.NUM_OF_SERVERS; i++)
	{
		delete servers[i];
	}
	delete[] entries;
	delete[] servers;

	float average = (float)sum_of_jobs / SIMULATION_TIME;
	cout << "average of jobs per time slot: " << average << endl  << "========================================="  << endl ;

	return average;
};

int main(int argc, char* argv[]) {
	string filepath = argv[1];
	std::ifstream infile(filepath);
	string line;
	while (!infile.eof())
	{
		std::getline(infile, line);
		std::istringstream iss(line);
		int a, b, c, d;
		float e;
		iss.clear();
		iss.str(line);
		iss >> a >> b >> c >> d >> e;
		ALGORITHEM al = (ALGORITHEM)a;
		TEST_SCALE tS = (TEST_SCALE)b;
		SERVER_UNITY sU = (SERVER_UNITY)c;
		SERVER_SPEED_DISTRIBUTION ssd = (SERVER_SPEED_DISTRIBUTION)d;
		ENTRIES_TO_SERVERS_RATIO rat = (ENTRIES_TO_SERVERS_RATIO)e;
		test_params params(al,tS,sU,ssd,rat);
		//test_params params(JSQ, SMALL, HETEROGENEOUS_X10, FAST_5_SLOW_5, 0.7);

		simulate(params);
	}
	std::string input;
	std::getline(std::cin, input);

	return 0;
}
