// Load flow.cpp : Defines the entry point for the console application.
//

#include "NetworkTopology.h"
#include <iostream>

int main()
{

	NetworkTopology network;

	//*********** LOAD DATA ***********/
	LoadNumberOfElements(network);
	LoadNodes(network);
	LoadBranches(network);


	Branch *branches = network.TopologyAnalysis(network);
	cout << endl;
	cout << "***********************************" << endl;
	cout << endl;

	network.LoadFlowAnalysis(network, branches);
	

	
	cout << endl;
	system("Pause");
	return 0;
}


