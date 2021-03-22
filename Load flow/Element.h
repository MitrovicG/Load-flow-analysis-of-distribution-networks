#ifndef ELEMENT_H
#define ELEMENT_H

#include <iostream>
#include <fstream>
#include <string>
#include <complex>

using namespace std;


class Element 
{
	protected:
		int elementIndex;
		int elementID;
		string elementName;

	public:

		//Constructor - default
		Element();
	
		//Constructor with parameters
		Element(int index, int id, string name);


		//Set methods
		void SetIndex (int value)
		{
			elementIndex = value;
		}

		void SetID (int value)
		{
			elementID = value;
		}

		void SetName(string name)
		{
			elementName = name;
		}

		//Get methods
		int GetIndex() const
		{
			return elementIndex;
		}

		int GetID() const
		{
			return elementID;
		}

		string GetName() const 
		{
			return elementName;
		}

/**
Displays a information about element
*/
		virtual void ShowElementInfo() const = 0;

};

#endif