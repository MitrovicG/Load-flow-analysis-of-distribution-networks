#include "Branch.h"

Branch::Branch() 
{
	node1 = 0;
	node2 = 0;
	R = 0.f;
	X = 0.f;
	G = 0.f;
	B = 0.f;
	l = 0.f;
	Z = 0.f;
	Y = 0.f;
	branchCurrent = 0.f;
	layerIndex = 0;
	branchStatus = ON;
}


void Branch::SetBranchStatus (int status)
{
	switch(status)
	{
		case(0) :
		branchStatus = OFF;
		break;

		case(1) :
		branchStatus = ON;
		break;

		case(-1) :
		branchStatus = BREAK_POINT;
		break;

		default:
		cout << "Wrong type of branch! " << endl;
		system ("Pause");
		break;
	}
}


void Branch::ShowElementInfo() const
{
	cout << endl;
	cout << "Informations of element: " << endl;
	cout << "........................." << endl;
	cout << "ID of element: " << GetID() << endl;
	cout << "Node 1 - Node 2 :"  << "   " << GetNode1() << " - " << GetNode2() << endl;
	cout << "Element name: " << GetName() << endl;
	cout << "R= " << Get_R() <<",  " << "X= " << Get_X() << ",  "<< "G= " << Get_G() << ",  " << "B= " << Get_B() << endl;
	cout << "Length [km]: " << Get_l() << endl;
	cout << "Complex Impedanse Z = " << Get_Z() << endl;
	cout << "Complex Admittance Y = " << Get_Y() << endl;
	cout << "Branch current : " << GetBranchCurrent() << endl;
	cout << "Branch is in layer : " << GetLayerIndex() << endl;
	cout << endl;
}