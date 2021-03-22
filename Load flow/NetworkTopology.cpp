#include "NetworkTopology.h"

int numberOfBP = 0; //breakpoint counter 

/*******************************************************   LOAD DATA  *************************************************************************/

void LoadNumberOfElements (NetworkTopology &net)
{
	int numNodes;		//numberOfNodes
	int numBranches;	//numberOfBranches
	int supplyNode;		//Index of supply node
	
	string temp;

	fstream file; 
	file.open("Network Topology Data.txt");

	if(file.is_open())
	{
		file >>  temp >> temp >> temp >> numNodes ;
		file >>  temp >> temp >> temp >> numBranches ;
		file >>  temp >> temp >> temp >> supplyNode ;

		net.SetNumberOfNodes(numNodes);
		net.SetNumberOfBranches(numBranches);
		net.SetSupplyNodeIndex(supplyNode);

		cout << "Number of nodes: " << net.GetNumberOfNodes() << endl;
		cout << "Number of branches: " << net.GetNumberOfBranches() << endl;
		cout << "Supply Node: " << net.GetSourceNodeIndex() << endl;

			file.close();
	}
	else
	{
		cout <<  "An error has occured while opening file Network Topology Data2.txt!";
	}
}

void LoadNodes(NetworkTopology &net)
{
	int index, id;
	string name;
	float p, q;
	Node node;


	string lineTemp;

	fstream file;

	file.open("Nodes.txt");

	if(file.is_open())
	{
		for (int i=0; i<2; i++)
		{
			getline(file,lineTemp);
		}

		net.nodes = new Node[net.GetNumberOfNodes()];

		for(int i=0; i < net.numberOfNodes; i++)
		{
			file >> index >> id >> name >> p >> q ;
			
			node.SetIndex(index);
			node.SetID(id);
			node.SetName(name);
			node.SetComplexPower(p,q);
			net.nodes[i] = node;
		}
			file.close();

	}
	else 
	{
		cout <<  "An error has occured while opening file Nodes.txt!" << endl;
	}
}

void LoadBranches(NetworkTopology &net)
{
	int index, id;
	string name;
	int node1, node2;
	float r,x,g,b,l;
	Branch branch;

	complex<float> Zb = pow(V_NOM,2) / S_NOM;
	complex<float> Yb = 1.f/Zb;
	
	string lineTemp;

	fstream file;

	file.open("Branches.txt");

	if(file.is_open())
	{
		for (int i=0; i<2; i++)
		{
			getline(file,lineTemp);
		}

		net.branches = new Branch[net.GetNumberOfBranches()];

		for(int i=0; i< net.numberOfBranches; i++)
		{
			file >> index >> id >> name >> node1 >> node2;
			file >> r >> x >> g >> b >> l;
			
			complex<float> z_temp(r*l,x*l);
			complex<float> y_temp(g*l,b*l);

			complex<float>z = z_temp/ Zb;
			complex<float>y = y_temp/ Yb;

			branch.SetIndex(index);
			branch.SetID(id);
			branch.SetName(name);
			branch.SetNode1(node1);
			branch.SetNode2(node2);
			branch.Set_R(r*l);
			branch.Set_X(x*l);
			branch.Set_G(g*l);
			branch.Set_B(b*l);
			branch.Set_l(l);
			branch.Set_Z(real(z),imag(z));
			branch.Set_Y(real(y),imag(y));

			net.branches[i] = branch;

		}
			file.close();

	}
	else
	{
		cout << " An error has occured while opening file Network Branches.txt! " << endl;
	}


}

/**************************************************************************************************************************************************/
/************************************************************   TOPOLOGY ANALYSIS   ***************************************************************/
/**************************************************************************************************************************************************/


Branch* NetworkTopology:: TopologyAnalysis(NetworkTopology &dm)
{
	int *visitedNodes = new int[dm.numberOfNodes];
	int *visitedBranches = new int[dm.numberOfBranches];

	for(int i=0; i < dm.numberOfNodes; i++)
	{
		visitedNodes[i] = -1;
	}

	for(int i=0; i < dm.numberOfBranches; i++)
	{
		visitedBranches[i] = -1;
	}

	int index = -1;
	int layerIndex = 1;
	bool condition = true;     
	int tempValueOfNode = 0;
	int loopsCounter = 0;

	Branch *branchesInLayers = new Branch[dm.numberOfBranches - loopsCounter];

	visitedNodes[dm.sourceNodeIndex] = 1;

	while(condition)   
	{
		condition = false;

	//Sorting by layers
	for(int i=0; i < dm.numberOfBranches; i++)
	{
		if (visitedNodes[dm.branches[i].GetNode1()] == -1 && visitedNodes[dm.branches[i].GetNode2()] == layerIndex)
		{
			index++;
			visitedNodes[dm.branches[i].GetNode1()] = layerIndex + 1;
			visitedBranches[i] = layerIndex;
			dm.branches[i].SetLayerIndex(layerIndex);
			 
			tempValueOfNode = dm.branches[i].GetNode1();
			dm.branches[i].SetNode1(dm.branches[i].GetNode2());
			dm.branches[i].SetNode2(tempValueOfNode);

			branchesInLayers[index] = dm.branches[i];
			dm.nodes[dm.branches[i].GetNode2()].SetParentBranchIndex(dm.branches[i].GetIndex());
			dm.nodes[dm.branches[i].GetNode1()].SetChildBranchIndex(dm.branches[i].GetIndex());

			condition = true;
		}
		else if (visitedNodes[dm.branches[i].GetNode1()] == layerIndex && visitedNodes[dm.branches[i].GetNode2()] == -1)
		{
			index++;
			visitedNodes[dm.branches[i].GetNode2()] = layerIndex + 1;
			visitedBranches[i] = layerIndex;
			dm.branches[i].SetLayerIndex(layerIndex);

			dm.nodes[dm.branches[i].GetNode2()].SetParentBranchIndex(dm.branches[i].GetIndex());
			dm.nodes[dm.branches[i].GetNode1()].SetChildBranchIndex(dm.branches[i].GetIndex());

			branchesInLayers[index] = dm.branches[i];
			condition = true;
		}
		//loop brakpoints
		else if (visitedNodes[dm.branches[i].GetNode1()] != -1 && visitedNodes[dm.branches[i].GetNode2()] != -1 && visitedBranches[i] == -1)
		{
			loopsCounter++;
			visitedBranches[i] = -2;

			dm.branches[i].SetBranchStatus(-1);
			
			//important for function FindPathsForSM, defining positive and negative part of path
			dm.nodes[dm.branches[i].GetNode1()].SetInjectedCurrent(1.0f);
			dm.nodes[dm.branches[i].GetNode2()].SetInjectedCurrent(-1.0f);
			
			numberOfBP++;

			condition = true;
		}
	}
		layerIndex++;
	}

	Branch *tempBranches = new Branch[dm.numberOfBranches];
	for (int i=0; i< dm.numberOfBranches; i++)
	{
		tempBranches[i] = dm.branches[i] ;
		dm.branches[i] = branchesInLayers[i];
	}


	cout << endl;
	cout << "Branches by layers: " << endl;
	cout << "...................." << endl;

	int layer = 1;
	for(int i=0; i< dm.numberOfBranches; i++)
	{
		if(dm.branches[i].GetLayerIndex() == 0)
		{
			continue;
		}

		if(dm.branches[i].GetLayerIndex() == layer)
		{
			cout << dm.branches[i].GetIndex() << "  " ;
		}
		else 
		{
			cout << endl;
			cout << dm.branches[i].GetIndex() << "  ";
			layer++;
		}
	}

	cout << endl;

	dm.SetNumberOfLayers(dm.branches[dm.numberOfBranches-1].GetLayerIndex());

	for (int i=0; i< dm.numberOfBranches; i++)
	{
		dm.branches[i] = tempBranches[i];
	}
	
	delete[] visitedBranches;
	delete[] visitedNodes;
	return branchesInLayers;
}


/**************************************************************************************************************************************************/
/**************************************************************************************************************************************************/
/**********************************************    CALCULATION Of SENSITIVITY MATRIX     **********************************************************/
/**************************************************************************************************************************************************/

tuple<Branch**,int*, int**>  NetworkTopology::FindPathsForZmatrix(NetworkTopology &net)
{
	Branch *branchesLBP = new Branch[numberOfBP];

	Branch **pathsArray;
	pathsArray = new Branch* [numberOfBP];

	int **pathOrientation;
	pathOrientation = new int* [numberOfBP];

	int * sizeOfArrays = new int [numberOfBP];


	int iterator = 0;

	//Finding LBP
	for(int i=0; i < net.numberOfBranches ; i++)
	{
		if(net.branches[i].GetBranchStatus() == BREAK_POINT)
		{
			branchesLBP[iterator] = branches[i];
			iterator++;
		}
	}

	//temporary list for branches
	list<Branch> branchBPlist;
	//temporary list for branch orientations (values: 1- positive, -1 -negative )
	list<int> branchOrientation;

	for(int i = 0; i < numberOfBP ; i++)
	{
		branchBPlist.clear();
		branchOrientation.clear();
		//inital value of pathLength is 2 (starting branches are included for each node of breakpoint)  
		int pathLength = 2;

		Branch tempBranch1 = branches[net.nodes[branchesLBP[i].GetNode1()].GetParentBranchIndex()-1];
		Branch tempBranch2 = branches[net.nodes[branchesLBP[i].GetNode2()].GetParentBranchIndex()-1];
		branchBPlist.push_back(tempBranch1);
		branchBPlist.push_back(tempBranch2);

		//1 or -1
		int path1 = 0;
		int path2 = 0;

		//Orientation is positive if injected current is 1A
		if(net.nodes[tempBranch1.GetNode2()].GetInjectedCurrent() == 1.0f)
		{
			branchOrientation.push_back(1);
			path1 = 1;

		}
		else 
		{
			branchOrientation.push_back(-1);
			path1 = -1;
		}

		if(net.nodes[tempBranch2.GetNode2()].GetInjectedCurrent() == 1.0f)
		{
			branchOrientation.push_back(1);
			path2 = 1;
			net.nodes[tempBranch2.GetNode1()].SetVoltage(1.0f);
			net.nodes[tempBranch1.GetNode2()].SetVoltage(1.0f);

		}
		else 
		{
			branchOrientation.push_back(-1);
			path2 = -1;
			net.nodes[tempBranch2.GetNode1()].SetVoltage(1.0f);
			net.nodes[tempBranch1.GetNode2()].SetVoltage(1.0f);

		}

		
		do
		{
			if(tempBranch1.GetNode1() != net.sourceNodeIndex)
			{
				tempBranch1 = branches[net.nodes[tempBranch1.GetNode1()].GetParentBranchIndex()-1];
				branchBPlist.push_back(tempBranch1);
				pathLength ++;
				branchOrientation.push_back(path1);
			}
		} while (tempBranch1.GetNode1() != net.sourceNodeIndex);

		do
		{
			if(tempBranch2.GetNode1() != net.sourceNodeIndex)
			{
				tempBranch2 = branches[net.nodes[tempBranch2.GetNode1()].GetParentBranchIndex()-1];
				branchBPlist.push_back(tempBranch2);
				pathLength ++;
				branchOrientation.push_back(path2);

			}
		} while (tempBranch2.GetNode1() != net.sourceNodeIndex);
		

		 pathsArray[i] = new Branch[pathLength];
		 pathOrientation[i] = new int[pathLength];
		 int index = 0;
		 for(list<Branch>::iterator it = branchBPlist.begin(); it != branchBPlist.end(); it ++)
		 {
			 pathsArray[i][index] = *it;
			 index++;
		 }
		 sizeOfArrays[i] = index;

		 index = 0;
		  for(list<int>::iterator it = branchOrientation.begin(); it != branchOrientation.end(); it ++)
		 {
			 pathOrientation[i][index] = *it;
			 index++;
		  }

	}

	return make_tuple(pathsArray, sizeOfArrays, pathOrientation);
}


/**************************************************************************************************************************************************/

complex<float>** NetworkTopology::CalculateSensitivityMatrix(NetworkTopology &net)
{
	int *sizeOfArrays = new int[numberOfBP];
	Branch **pathsOfBranches = new Branch*[numberOfBP];
	int **pathOrientation = new int*[numberOfBP];

	tie(pathsOfBranches, sizeOfArrays, pathOrientation) = net.FindPathsForZmatrix(net);

	complex<float> **SM;  //sensitivity matrix
	SM = new complex<float>* [numberOfBP];

	for(int i = 0; i < numberOfBP; i++)
	{
		SM[i] = new complex<float> [numberOfBP];
	}

	for(int i = 0; i < numberOfBP; i++)
	{
		for(int j = 0; j< numberOfBP; j++)
		{
			SM[i][j] = 0.0f;
		}
	}

	//Print Z of branch
	for (int i =0; i < net.numberOfBranches; i++)
	{
		cout << "Branch " << i+1 <<", Z = " << net.branches[i].Get_Z() << endl;
	}
	cout << endl;


	//****************  Calculating diagonal elements  ****************//
	complex<float> Z_path = 0.0f;

	for(int i=0; i < numberOfBP; i++)
	{
		Z_path = 0.0f;

		for(int j =0; j < sizeOfArrays[i]; j++)
		{
			Z_path += pathsOfBranches[i][j].Get_Z();
		}

		SM[i][i] = Z_path;
	}

	//**************** Calculating non-diagonal elements  ****************//

	complex<float> Z_common = 0.0f;

	for(int i = 0, j = i+1; i < numberOfBP; i++)		
	{
		for(int j = i+1; j < numberOfBP  ; j++)
		{
			Z_common = 0.0f;

			for(int p1 = 0; p1 < sizeOfArrays[i]; p1++)
			{
				for(int p2 = 0 ; p2 < sizeOfArrays[j]; p2++)
				{
					if(i != j)
					{
						if(pathsOfBranches[i][p1].GetIndex() == pathsOfBranches[j][p2].GetIndex())
						{
							if(pathOrientation[i][p1] == pathOrientation[j][p2])
							{
								Z_common += pathsOfBranches[i][p1].Get_Z();
							}
							else
							{
								Z_common -= pathsOfBranches[i][p1].Get_Z();
							}

						}
					}
					else continue;
				}
			}
			SM[i][j] = Z_common;
			SM[j][i] = Z_common;
		}
	}

	//Print Sensitivity matrix
	cout << "Sensitivity matrix: " << endl;
	cout << endl;
	for(int i = 0; i <numberOfBP; i++)
	{
		for(int j =0; j< numberOfBP; j++)
		{
			cout << SM[i][j] << "   " ;
		}
			cout << endl;
	}

	//Print paths
	cout << endl;
	for(int i=0; i < numberOfBP; i++)
	{
		cout << "PATH " << i + 1 << " - branches: " << endl;
		cout << "-------------------" << endl;
		for(int j =0; j < sizeOfArrays[i]; j++)
		{
			cout << pathsOfBranches[i][j].GetIndex() << endl;
		}
		cout << endl;
	}

	return SM;
}


/**************************************************************************************************************************************************/
/**************************************************************************************************************************************************/

void NetworkTopology:: SetYoForNodes(NetworkTopology &net)
{
	for(int i = 0; i < net.numberOfBranches ; i++)
	{
		if(net.branches[i].GetBranchStatus() == BREAK_POINT)
		{
			continue;
		}
		else
		{
			net.nodes[net.branches[i].GetNode1()].SetYoForNode(net.branches[i].Get_Y());
			net.nodes[net.branches[i].GetNode2()].SetYoForNode(net.branches[i].Get_Y());
		}
	}

}


/**************************************************************************************************************************************************/
/**************************************************************************************************************************************************/

void NetworkTopology::CalculateCurrentInBranches(NetworkTopology &net, Branch* branchesArray)
{

	for(int i = 0; i < net.numberOfBranches ; i++)
	{
		net.branches[i].SetBranchCurrent(0.0f);
	}

	for(int i=net.numberOfBranches - numberOfBP; i>0 ; i--)
	{
		if(branchesArray[i-1].GetBranchStatus() == ON)
		{
			if(branchesArray[i-1].GetLayerIndex() == net.numberOfLayers || net.nodes[branchesArray[i-1].GetNode2()].GetChildBranchIndex() == -1)
			{
				net.branches[(branchesArray[i-1].GetIndex())-1].CalculateBranchCurrent(net.nodes[branchesArray[i-1].GetNode2()].GetInjectedCurrent());
				if(net.branches[net.nodes[branchesArray[i-1].GetNode1()].GetParentBranchIndex()-1].GetBranchCurrent() == 0.0f)
				{
					net.branches[net.nodes[branchesArray[i-1].GetNode1()].GetParentBranchIndex()-1].CalculateBranchCurrent(net.branches[(branchesArray[i-1].GetIndex())-1].GetBranchCurrent() + net.nodes[branchesArray[i-1].GetNode1()].GetInjectedCurrent());
				}
				else 
				{
					net.branches[net.nodes[branchesArray[i-1].GetNode1()].GetParentBranchIndex()-1].CalculateBranchCurrent(net.branches[(branchesArray[i-1].GetIndex())-1].GetBranchCurrent());
				}
				
			}
			else 
			{
				if(net.branches[branchesArray[i-1].GetIndex()-1].GetLayerIndex() != 1)
				{
					if(net.branches[net.nodes[branchesArray[i-1].GetNode1()].GetParentBranchIndex()-1].GetBranchCurrent() == 0.0f)
					{
						net.branches[net.nodes[branchesArray[i-1].GetNode1()].GetParentBranchIndex()-1].CalculateBranchCurrent(net.branches[(branchesArray[i-1].GetIndex())-1].GetBranchCurrent() + net.nodes[branchesArray[i-1].GetNode1()].GetInjectedCurrent());
					}
					else 
					{
						net.branches[net.nodes[branchesArray[i-1].GetNode1()].GetParentBranchIndex()-1].CalculateBranchCurrent(net.branches[(branchesArray[i-1].GetIndex())-1].GetBranchCurrent());
					}
				
				}
				else
				{
					break;
				} 

			}

		}
			else continue;
	}

	//Write currents 
		cout << endl;
		cout << "Current in branch: " << endl;
		cout << "-------------------" << endl;
		cout << endl;

		for(int i = 0; i< numberOfBranches; i++)
		{
			cout << "Branch " << i+1 << " :  " << net.branches[i].GetBranchCurrent() << endl;
		}

		cout << endl;
}

/**************************************************************************************************************************************************/

float NetworkTopology::CalculateVoltages(NetworkTopology &net, Branch* branchesArray)
{

	complex<float> oldVoltage = 0;
	complex<float> newVoltage = 0;
	complex<float> dVoltage = 0;
	float dV = 0;
	float dVtemp = 0;

		for(int i=0; i<net.numberOfBranches - numberOfBP ; i++)
		{
			int branchIndex = branchesArray[i].GetIndex()-1;
			oldVoltage = net.nodes[branchesArray[i].GetNode2()].GetVoltage();
			newVoltage = net.nodes[branchesArray[i].GetNode1()].GetVoltage() - net.branches[branchIndex].Get_Z() * net.branches[branchIndex].GetBranchCurrent() ;
			dVoltage = oldVoltage - newVoltage;

			net.nodes[branchesArray[i].GetNode2()].SetVoltage(newVoltage);

			dVtemp = sqrt(pow(real(dVoltage),2) + pow(imag(dVoltage),2)); 

			if(dVtemp > dV)
			{
				dV = dVtemp;
				cout << dV << endl;
			}
		}

		cout << endl;
		cout << "Nodal votages: " << endl;
		cout << "-----------------" << endl;
		cout << endl;

		for(int i = 0; i<net.numberOfNodes; i++)
		{
			cout << "Node " << net.nodes[i].GetIndex() << " :  " << net.nodes[i].GetVoltage() ;
			(nodesBP[i] == 1) ? cout << " * " << endl : cout << endl;
		}

		cout << endl;

	return dV;
}


/**************************************************************************************************************************************************/

Branch* NetworkTopology::FindBranchesWithLoops(NetworkTopology& net)
{
	Branch *branchesWithLoops = new Branch[numberOfBP];
	nodesBP = new int [net.numberOfNodes];

	for (int i = 0; i< net.numberOfNodes; i++)
	{
		nodesBP[i] = 0;
	}

	int index = 0;

	for(int i = 0; i < net.numberOfBranches; i++)
	{
		if(net.branches[i].GetBranchStatus() == BREAK_POINT)
		{
			branchesWithLoops[index] = net.branches[i];
			nodesBP[branches[i].GetNode1()] = 1;
			nodesBP[branches[i].GetNode2()] = 1;

			index++;
		}
	}

	return branchesWithLoops;
}


/**************************************************************************************************************************************************/

complex<float>* NetworkTopology::Calculate_dV_vector(NetworkTopology &net, Branch* branchesBP)
{
	complex<float>* dV_vect = new complex<float> [numberOfBP];

	for(int i = 0; i < numberOfBP; i++)
	{
		dV_vect[i] = net.nodes[branchesBP[i].GetNode1()].GetVoltage() - net.nodes[branchesBP[i].GetNode2()].GetVoltage();
	}

	return dV_vect;
}


/**************************************************************************************************************************************************/

void NetworkTopology::Calculate_dJ_vector(complex<float>**SM, complex<float>*V,complex<float>*dJ)
{

	  for (int i=0;i<numberOfBP;i++)
	  {
        for (int j=0;j<numberOfBP;j++)
		{
            dJ[i] += SM[i][j] * V[j];
        }
    }

}


/**************************************************************************************************************************************************/
/**************************************************************************************************************************************************/
/********************************************************     LOAD FLOW     ***********************************************************************/
/**************************************************************************************************************************************************/
/**************************************************************************************************************************************************/


float NetworkTopology::CalculateLoadFlows(NetworkTopology &net, Branch* branchesArray,float dV, complex<float>* correction, int counter)
{

		//********************	step (1) : Calculation of injected currents ********************//

		cout << "Injected currents: " << endl;
		cout << "------------------" << endl;
		cout << endl;

	
		for (int i = 0; i<net.numberOfNodes; i++)
		{
			if(counter == 1)
			{
				net.nodes[i].SetInjectedCurrent(conj(net.nodes[i].GetComplexPower() / net.nodes[i].GetVoltage()) + net.nodes[i].GetYoForNode() * net.nodes[i].GetVoltage());
			}
			else if (counter != 1)
			{
				if(nodesBP[i] !=1) 
				{
					net.nodes[i].SetInjectedCurrent(conj(net.nodes[i].GetComplexPower() / net.nodes[i].GetVoltage()) + net.nodes[i].GetYoForNode() * net.nodes[i].GetVoltage());
				}
				else
				{
					net.nodes[i].CorrectInjectedCurrent(correction[i],(conj(net.nodes[i].GetComplexPower() / net.nodes[i].GetVoltage()) + net.nodes[i].GetYoForNode() * net.nodes[i].GetVoltage()));
				}
			}
		}

		//Write injected currents 
		for(int i = 0; i < net.numberOfNodes; i++)
		{
				cout << "Node " << net.nodes[i].GetIndex() << ": " << net.nodes[i].GetInjectedCurrent() << endl;
		}

		//********************	step (2) : Calculation of branch currents - "Backward Sweep" ********************//

		net.CalculateCurrentInBranches(net,branchesArray);

		//******************** step (3) : Voltage calculation - "Forward Sweep" ********************//

		dV = net.CalculateVoltages(net,branchesArray);
		
		return dV;
}


/**************************************************************************************************************************************************/
/**************************************************************************************************************************************************/

void NetworkTopology:: LoadFlowAnalysis(NetworkTopology &net, Branch* branchesArray)
{
	
	//Setovanje otocnih admitansi za sve cvorove
	net.SetYoForNodes(net);

	float dV = 100;	     
	int counter = 0;     //iteration counter

	//setting voltages before first iteration
	for (int i=0; i<net.numberOfNodes; i++) 
	{
		net.nodes[i].SetVoltage(1.0f);
	}

	//Creation of sensitivity matrix
	complex<float> **SM = net.CalculateSensitivityMatrix(net);

	Branch *branchesLBP = net.FindBranchesWithLoops(net);

	complex<float> **SM_inv = new complex<float>* [numberOfBP];	
	SM_inv = GetMatrixInverse(SM,numberOfBP);

	cout << "Inverse SM" << endl;
	cout << "-----------------" << endl; cout << endl;

	for(int i=0; i < numberOfBP; i++)
	{
		for(int j=0; j < numberOfBP; j++)
		{
			cout << SM_inv[i][j] << "   ";
		}
			cout << endl;
		}
		cout << endl;


	complex<float> *dJ = new complex<float> [numberOfBP];

		for(int i=0; i < numberOfBP; i++)
		{
				dJ[i] = 0;
		}

	complex<float> *corrections = new complex<float> [net.numberOfNodes];
		
		for(int i = 0; i < net.numberOfNodes; i++)
		{
			corrections[i] = 0.0f;
		}

	//-----------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------ ITERATIVE PROCEDURE --------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------------//


	cout << endl;
	cout << "*************************************** LOAD FLOWS ***************************************" << endl;
	cout << endl;

	//setting injected current 
	for(int i =0; i < numberOfBP; i++)
	{
		net.nodes[branchesLBP[i].GetNode1()].SetInjectedCurrentToZero();
		net.nodes[branchesLBP[i].GetNode2()].SetInjectedCurrentToZero();
	}
	
	do 
	{
		counter++;
		cout << "ITERATION : " << counter << endl;
		cout <<"**************" << endl; cout << endl;
		
		dV = net.CalculateLoadFlows(net,branchesArray, dV, corrections,counter);
	//}
		//for weakly-meshed network
		if(numberOfBP > 0)
		{
			cout << "-------------------------------- Corrections -------------------------------- " << endl;
			cout << endl;

			complex<float> *dV_vector = net.Calculate_dV_vector(net,branchesLBP);
			
			cout << endl;
			cout << "dV vector: " << endl;
			cout << "-----------" << endl; cout << endl;

			for(int i=0; i < numberOfBP; i++)
			{
				cout << dV_vector[i] << endl;
			}
			cout << endl;

			net.Calculate_dJ_vector(SM_inv,dV_vector,dJ);

			cout << endl;
			cout << "dJ vector: " << endl;
			cout << "-----------" << endl; cout << endl;

			for(int i=0; i < numberOfBP; i++)
			{
				cout << dJ[i] << endl;
			}

			for(int i = 0; i < net.numberOfNodes; i++)
			{
				corrections[i] = 0.0f;
			}

			for(int i = 0; i < numberOfBP; i++)
			{
				corrections[branchesLBP[i].GetNode1()] += dJ[i];
				corrections[branchesLBP[i].GetNode2()] -= dJ[i];
			}

			
			cout << endl;
			cout << "----------------------------------------------------------------------------------" << endl;
			cout << endl;
		}
	} while(abs(dV) > 0.00001 );
}


/**************************************************************************************************************************************************/
