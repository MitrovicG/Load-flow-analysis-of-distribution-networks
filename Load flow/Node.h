#ifndef NODE_H
#define NODE_H

#include "Element.h"


class Node : public Element
{
	private:
		complex<float> V;
		complex<float> Si;
		complex<float> currentInj;
		complex<float> Yo;
		int parentBranchIndex;
		int childBranchIndex;

	public:
		//Constructor - default
		Node();
	
		//Constructor with parameters
		Node(complex<float>, complex<float>, complex<float>, complex<float>, int, int);
	

		//Set methods

		void SetVoltage(complex<float> value) 
		{
			V = value;
		}

		void SetComplexPower(float p, float q) 
		{
			Si.real(p);
			Si.imag(q);
		}

		void SetYoForNode(complex<float> value) 
		{
			Yo += 0.5f * value;
		}

		void SetInjectedCurrent(complex<float> value)
		{
			currentInj = value;
		}
		
		void SetInjectedCurrentToZero()
		{
			currentInj = 0.0f;
		}

		void SetParentBranchIndex(int value)
		{
			parentBranchIndex = value;
		}

		void SetChildBranchIndex(int value)
		{
			childBranchIndex = value;
		}

		//Get methode

		complex<float> GetVoltage() const
		{
			return V;
		}

		complex<float> GetComplexPower() const
		{
			return Si;
		}
		
		complex<float> GetYoForNode() const
		{
			return Yo;
		}

		complex<float> GetInjectedCurrent() const
		{
			return currentInj;
		}

		int GetParentBranchIndex() const
		{
			return parentBranchIndex;
		}

		int GetChildBranchIndex() const
		{
			return childBranchIndex;
		}
		
/**
 	Correct injected current 
	@param correction Value calculated in iterative procedure
	@param cosumption Nodal current injection
 */
	void CorrectInjectedCurrent(complex<float>correction, complex<float>consumption);
	
/**
 	Displays a information about node
 */
	void ShowElementInfo() const;
		
};

#endif