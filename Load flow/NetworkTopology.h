#ifndef NETWORKTOPOLOGY_H
#define NETWORKTOPOLOGY_H

#include "Branch.h"
#include "Node.h"
#include <list>
#include <tuple>

float const S_NOM = 3150000.f;   //nominal power [MVA]
float const V_NOM = 20000.f;     //nominal voltage [kV]

class NetworkTopology
{
	private:
		int numberOfNodes;
		int numberOfBranches;
		int sourceNodeIndex;
		int numberOfLayers;

		Branch *branches;
		Node *nodes;
		int *nodesBP;

	public:	
		//Constructor - default
		NetworkTopology()
		{
			numberOfNodes = 0;
			numberOfBranches = 0;
			sourceNodeIndex = 0;
			numberOfLayers = 0;
			branches = 0;
			nodes = NULL;
		}

		//Constructor with parameters
		NetworkTopology (int nn, int nb, int sn, int numLayers)
		{
			numberOfNodes = nn;
			numberOfBranches = nb;
			sourceNodeIndex = sn;
			numberOfLayers = numLayers;
			nodes = NULL;
			branches = NULL;
		}

		//Set methods
		void SetNumberOfNodes (int number)
		{
			numberOfNodes = number;
		}

		void SetNumberOfBranches (int number)
		{
			numberOfBranches = number;
		}

		void SetSupplyNodeIndex(int index)
		{
			sourceNodeIndex = index;
		}

		void SetNumberOfLayers (int number)
		{
			numberOfLayers = number;
		}

		//Get methods
		int GetNumberOfNodes () const
		{
			return numberOfNodes;
		}

		int GetNumberOfBranches () const
		{
			return numberOfBranches;
		}

		int GetSourceNodeIndex () const
		{
			return sourceNodeIndex;
		}

		int GetNumberOfLayers () const
		{
			return numberOfLayers;
		}


		
		//Friend functions 
		/**
			Load data from input files
		*/
		friend void LoadNumberOfElements(NetworkTopology &);
		friend void LoadNodes(NetworkTopology &);
		friend void LoadBranches(NetworkTopology &);

		/**
			Calculate inverse matrix
			param@ src Invertible matrix
			param@ n Matrix dimension
		*/
		friend complex<float>** GetMatrixInverse(complex<float>** src, int n);

		/**
			Topology analysis, converting weakly meashed network to radial
			@param net Instance of class Network
		*/
		Branch* TopologyAnalysis(NetworkTopology &net);

		/**
			Finds the unique path connecting the two side nodes of the breakpoint, 
			which has been divided into a positive path and a negative path 
			@param net Instance of class Network
		*/
		tuple<Branch **, int *, int**> FindPathsForZmatrix(NetworkTopology &net);

		/**
			Calculates elements of sensitivity matrix
			@param net Instance of class Network
			@return Calcualted sensitivity matrix
		*/
		complex<float>** CalculateSensitivityMatrix(NetworkTopology &net);

		/**
			Finds loop breakpoints (LBP) and collect branches to array
			@param net Instance of class Network
			@return breakpoint branch vector
		*/
		Branch* FindBranchesWithLoops(NetworkTopology &net);

		/**
		Set Yo for each nodes
		*/
		void SetYoForNodes(NetworkTopology &net);

		/**
		Calculate the breakpoint voltge mismatches by taking the voltage 
		difference (magnitude) between the two side nodes
		@param net Instance of class Network
		@param branchesLBP Loop breakpoint vector
		@return Vector with calculated breakpoint voltage mismatches
		*/
		complex<float>* Calculate_dV_vector(NetworkTopology & net, Branch *branchesLBP);

		/**
		Calculates breakpoint injection corrections
		@param SM Sensitivity matrix
		@param dV Breakpoint voltage vector
		@param J Brakpoint correction current vector
		*/
		void Calculate_dJ_vector(complex<float>**SM, complex<float>*dV,complex<float>*J);

		/**
		Calculates branch currents
		@param net Instance of class Network
		@param branchesArray Vector of branches, sorted by layers
		*/
		void CalculateCurrentInBranches(NetworkTopology &net, Branch* branchesArray);

		/**
		Calculates nodal voltages
		@param net Instance of class Network
		@param branchesArray Vector of branches sorted by layers
		@return maximal breakpoint voltage mismatch
		*/
		float CalculateVoltages(NetworkTopology &net, Branch* branchesArray);

		/**
		Calculates load flows 
		@param net Instance of class Network
		@param branchesArray Vector of branches sorted by layers
		@param dV Breakpoint voltage vector
		@param correction Brakpoint correction current vector
		@param index of iteration
		*/
		float CalculateLoadFlows(NetworkTopology &net, Branch* branchesArray,float dV, complex<float>* correction, int counter);

		/**
		Repeates iterative procedure and call function CalculateLoadFlows for each iteration 
		until the breakpoint voltage mismatches the preset error tolerance.
		@param net Instance of class Network
		@param branchesArray Vector of branches sorted by layers
		*/
		void LoadFlowAnalysis(NetworkTopology &net, Branch*branchesArray);


};


#endif