#include "Node.h"

Node::Node()
{	
	V = 0;
	Si = 0;
	Yo = 0;
	currentInj = 0;
	parentBranchIndex = -1;
	childBranchIndex = -1;
}

Node::Node(complex<float> v, complex<float> s, complex<float> yo, complex<float> cInj, int parentBranch, int childBranch)
{
	V = v;
	Si = s;
	Yo = yo;
	currentInj = cInj;
	parentBranchIndex = parentBranch;
	childBranchIndex = childBranch;
}

void Node::CorrectInjectedCurrent(complex<float> correction, complex<float> consumption)
{
	currentInj = 0.0f;
	currentInj = correction + consumption;
}

void Node::ShowElementInfo() const
{
	cout << endl;
	cout << "Informations of element: " << endl;
	cout << "........................." << endl;
	cout << "ID of element: " << GetID() << endl;
	cout << "Element name: " << GetName() << endl;
	cout << "Complex power [S] = " << GetComplexPower() << endl;
	cout << "Voltage [V] = " << GetVoltage() << endl;
	cout << "Yo : " << GetYoForNode() << endl;
	cout << "Injected current : " << GetInjectedCurrent() << endl;
	cout << "Fedding branch: " << GetParentBranchIndex() << endl;
	cout << endl;
}