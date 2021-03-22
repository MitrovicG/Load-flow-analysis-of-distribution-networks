#include "Element.h"

Element::Element()
{
	elementIndex = 0;
	elementID = 0;
	elementName = "element";
}


Element::Element(int index, int id, string name)
{
	elementIndex = index;
	elementID = id;
	elementName = name;
}
