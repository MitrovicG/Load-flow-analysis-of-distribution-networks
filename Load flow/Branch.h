#ifndef BRANCH_H
#define BRANCH_H

#include "Element.h"

enum BranchStatus
{
	OFF = 0,
	ON = 1,
	BREAK_POINT = -1
};


class Branch : public Element
{

	private:
		int node1, node2;
		float R, X, G, B, l;	
		complex<float> Z, Y;
		complex<float> branchCurrent;
		int layerIndex;
		BranchStatus branchStatus;

	public:
		Branch();


		//Set methods

		void SetNode1(int value)
		{
			node1 = value;
		}

		void SetNode2(int value)
		{
			node2 = value;
		}

		void Set_R(float value)
		{
			R = value;
		}

		void Set_X(float value)
		{
			X = value;
		}

		void Set_G(float value)
		{
			G = value;
		}

		void Set_B(float value)
		{
			B = value;
		}

		void Set_l(float value)
		{
			l = value;
		}

		void Set_Z(float r, float x)
		{
			Z.real(r);
			Z.imag(x);
		}

		void Set_Y(float g, float b)
		{
			Y.real(g);
			Y.imag(b);
		}

		void SetBranchCurrent(complex<float> value)
		{
			branchCurrent = value;
		}

		void CalculateBranchCurrent(complex<float> value)
		{
			branchCurrent += value;
		}

		void SetLayerIndex(int value)
		{
			layerIndex = value;
		}

		void SetBranchStatus (int status);


		//Get methods

		int GetNode1() const
		{
			return node1;
		}

		int GetNode2() const
		{
			return node2;
		}

		float Get_R() const
		{
			return R;
		}

		float Get_X() const
		{
			return X;
		}

		float Get_G() const
		{
			return G;
		}

		float Get_B() const
		{
			return B;
		}

		float Get_l() const
		{
			return l;
		}

		complex<float> Get_Z() const 
		{
			return Z;
		}

		complex<float> Get_Y() const 
		{
			return Y;
		}

		complex<float> GetBranchCurrent() const
		{
			return branchCurrent;
		}

		int GetLayerIndex() const
		{
			return layerIndex;
		}

		BranchStatus GetBranchStatus() const
		{
			return branchStatus;
		}

/**
 	Displays a information about branch
*/
		void ShowElementInfo() const;
	

};

#endif