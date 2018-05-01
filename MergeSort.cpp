
#include<stdlib.h>
#include<stdio.h>
#include<iostream>
#include <fstream>
#include <vector>
#include <thread>
#include<sstream>
using namespace std;
int arrSize;
long * a = NULL;
//Sees "arr[]" as two subarrays.  a->b and b-> last
void merge(long arr[], long a, long b, long last)
{
	long x;
	long y;
	long Merged;
	long temp0 = b - a + 1;
	long temp1 = last - b;

	//breaks down "arr[]" into two temporary arrays
	long Left[temp0];
	long Right[temp1];
	//Copy data to the left subarray
	for (x = 0; x < temp0; x++)
	{
		Left[x] = arr[a + x];
	}
	//Copy data to the right subarray
	for (y = 0; y < temp1; y++)
	{
		Right[y] = arr[b + 1 + y];
	}
	
	x = 0; 
	y = 0; 
	Merged = a; //First value of the new merged array
	//Sorts into the merged array by comparing the leading values
	while (x < temp0 && y < temp1)
	{
		if (Left[x] <= Right[y])
		{
			arr[Merged] = Left[x];
			x++;
		}
		else
		{
			arr[Merged] = Right[y];
			y++;
		}
		Merged++; //Move to next value in array
	}
	while (x < temp0)
	{
		arr[Merged] = Left[x];
		x++;
		Merged++;  //Move to next value in array
	}
	while (y < temp1)
	{
		arr[Merged] = Right[y];
		y++;
		Merged++; //Move to next value in array
	}
}
//Recursive method for merging
void mergeSort(long arr[], long a, long last)
{
	if (a < last)
	{
		long b = a + (last - a) / 2;
		mergeSort(arr, a, b);
		mergeSort(arr, b + 1, last);
		merge(arr, a, b, last);
	}
}
//Thread Function
void threadFunction()
{
	auto myid = this_thread::get_id();
	stringstream ss;
	ss << myid;
	string threadName = ss.str();
	long arr[arrSize] = { *a };
	if (threadName.compare("first") == 0)
	{
		//Sort first 25% of array
		mergeSort(arr, 0, (arrSize - 1)/4-1);
	}
	if (threadName.compare("second") == 0)
	{
		//Sort second 25% of array
		mergeSort(arr,(arrSize - 1) / 4, (arrSize - 1) / 2 - 1);
	}
	if (threadName.compare("third") == 0)
	{
		//Sort third 25% of array
		mergeSort(arr,(arrSize - 1) / 2, (3/4) * (arrSize-1) -1);
	}
	if (threadName.compare("fourth") == 0)
	{
		//Sort fourth 25% of array
		mergeSort(arr, (3 / 4) * (arrSize - 1), arrSize -1);
	}
}
//Driver
int main(int argc, char *argv[])
{
	if (argc == 1) {
		cout << "No command line argument given.\n"
			"Please provide the input file name as an argument.\n"
			"Ex: MergeSort file.txt\n";
		return -1;
	}
	vector<long> vector;
	ifstream inputFile;
	ofstream outputFile;
	string inputFileName(argv[1]);
	string outputFileName = inputFileName;
	inputFile.open(inputFileName);
	outputFileName.append("_sorted.txt");
	long inputNum;
	char hashHolder;
	if (inputFile.is_open()) {
		inputFile >> hashHolder;
		while (inputFile >> inputNum) {
			 // adds each inputNum to your data structure
			vector.push_back(inputNum);
		}
		inputFile.close();
	}
	else {
		cout << "Unable to open " << inputFileName << endl;
		return -1;
	}
	//perform sort here
	const int vectorsize = vector.size();
	long arr[vectorsize];
	a = arr;

	arrSize = sizeof(arr) / sizeof(arr[0]);
	printf("Array has  %d  Elements. \n", arrSize);
	for (int i = 0; i < arrSize; i++)
	{
		arr[i] = vector.back();
		vector.pop_back();
	}
	thread first(threadFunction);
	thread second(threadFunction);
	thread third(threadFunction);
	thread fourth(threadFunction);
	first.join();
	second.join();
	third.join();
	fourth.join();
	mergeSort(arr, 0, arrSize - 1);
	int median = arr[arrSize / 2];
	printf("The median is %d\n", median);
	outputFile.open(outputFileName);
	int i = 0;
	while (arrSize > 0) {
		outputFile << arr[i] << endl;
		i++;
		arrSize = arrSize - 1;
	}
	outputFile.close();

	return 0;
}
