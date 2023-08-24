#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
/*
The dummy data file provided with the work package provides you a time series data in csv
format where each entry has a
timestamp, id, x_position, y_position, unique_id,sensor_id.

It is a data of a scenario where some objects are moving around and detected by
a sensor network.
	**Each object might be detected by more than one sensor.
Task is to write an algorithm that
1- reads the data sequentially (one by one)
2- according to **timestamp** and **cluster the data coming from different sensors** (use sensor_id)
3- save the fused data to a new csv file where each entry should have f_timestamp, f_id, cluster_data, f_u_id.
	**The data in the new csv file must be updated sequentially and should not be done all at once.
*/
using namespace std;
struct sensor_data
{
	string timestamp_id;
	string sensor_id;
	int id;
	double x_position;
	double y_position;
	int unique_id;

};
struct cluster_data
{
	double x_position;
	double y_position;
	string sensor_id;
};
struct fused_data
{
	string f_timestamp;
	int f_id;
	vector<cluster_data> f_data;
	int f_u_id;

};


vector<sensor_data> readCSV(const string& filename)
{
	vector<sensor_data> data;

	ifstream file(filename);

	if (!file.is_open()) {
		cerr << "Error opening file:" << filename << endl;
		return data;
	}
	string line;
	//read and discard the  header line
	getline(file, line);

	while (getline(file, line)) {
		sensor_data entry;

		istringstream iss(line);
		string token;

		// Read each column using a stringstream
		getline(iss, entry.timestamp_id, ',');
		getline(iss, entry.sensor_id, ',');
		string id;
		getline(iss, id,',');
		entry.id = stod(id);
		string x_position_str, y_position_str;
		getline(iss, x_position_str, ',');
		getline(iss, y_position_str, ',');
		entry.x_position = stod(x_position_str);
		entry.y_position = stod(y_position_str);
		string unique_id;
		getline(iss, unique_id, ',');
		entry.unique_id = stod(unique_id);

		data.push_back(entry);

	}
	file.close();
	return data;

}

// function to calculate Euclidean distance between two points
double calculateDistance(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}
int main()
{
	string filename = "test_Data.csv";
	vector<sensor_data> input_data = readCSV(filename);
	std::ofstream file("fused_data.csv");
	if (!file.is_open()) {
		std::cerr << "Error: Could not open file for writing." << std::endl;
		return 1;
	}

	//write a header to csv file
	file << "timestamp_id,sensor_id,id,x_position,y_position,unique_id" << std::endl;

	vector<fused_data> output;

	map<int, int> unique_id_mapping;

	for (size_t i = 1; i < input_data.size(); ++i) {
		const sensor_data& data = input_data[i];

		//checking if object is known
		if (data.unique_id != 0)
		{
			//check if the unique_id has been seen before
			if (unique_id_mapping.count(data.unique_id) > 0) {
				fused_data fus;
				cluster_data clus;
				
				clus.sensor_id = data.sensor_id;
				clus.x_position = data.x_position;
				clus.y_position = data.y_position;

				fus.f_data.push_back(clus);
				
				fus.f_timestamp = data.timestamp_id;
				fus.f_id = rand();
				fus.f_u_id = unique_id_mapping[data.unique_id];

				output.push_back(fus);

				file << fus.f_timestamp << ","
					<< fus.f_id << ","
					<< clus.x_position << ","
					<< clus.y_position << ","
					<< clus.sensor_id << ","
					<< fus.f_u_id << endl;
			}
			// create new cluster
			else {
				unique_id_mapping[data.unique_id] = unique_id_mapping.size() + 1;
				fused_data fus;
				cluster_data clus;

				clus.sensor_id = data.sensor_id;
				clus.x_position = data.x_position;
				clus.y_position = data.y_position;

				fus.f_data.push_back(clus);

				fus.f_timestamp = data.timestamp_id;
				fus.f_id = rand();
				fus.f_u_id = unique_id_mapping[data.unique_id];
				output.push_back(fus);
				file << fus.f_timestamp << ","
					<< fus.f_id << ","
					<< clus.x_position << ","
					<< clus.y_position << ","
					<< clus.sensor_id << ","
					<< fus.f_u_id << endl;
			}
		}
		// search the matching cluster
		for (size_t i = 1; i < output.size(); ++i) {
			const fused_data& search = output[i];
			//check if they at the same timestamp and same distance
			

		}

		// If no matching cluster is found, create a new cluster

	}
	// Close the file when done
	file.close();


	cout << endl << output.size();
	return 0;

}
