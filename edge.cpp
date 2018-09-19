#include <cstdlib>
#include <cmath>

#include "edge.h"
#include "mesh.h"

// =================================================================
// CONSTRUCTOR
Edge::Edge(Vertex *a, Vertex *b, Mesh *m) 
{
  assert (a->getID() != b->getID());
  // Order the vertices so the smallest ID # is first.
  if (a->getID() < b->getID()) 
  {
    v1 = a;
    v2 = b;
  } 
  else 
  {
    v1 = b;
    v2 = a;
  }
  mesh = m;
  length = -1;
  legal = false;
  ReCalculateValue();
}

// =================================================================
// ACCESSOR
double Edge::getPriorityValue() const 
{
  if (mesh->getWhichCollapse() == "shortest") 
  {
    if (!legal) 
    {
      return 10000;
    }
    return length;
  } 
  else if (mesh->getWhichCollapse() == "color") 
  {
    // ASSIGNMENT 
    /* sort edges based on which would best keep overall appearance the same */
    /* smallest difference in average color should mean deletings edge won't be 
      noticed as much making it a better edge to collapse next. 
    */

    if (!legal) 
    {
      return 100000;
    }
    
    Color c1 = v1->getColor();
    Color c2 = v2->getColor();   
    Color temp = Average(c1, c2);
    double diff1 = std::abs(c1.r - temp.r) + std::abs(c1.g - temp.g) + std::abs(c1.b - temp.b);
    double diff2 = std::abs(c2.r - temp.r) + std::abs(c2.g - temp.g) + std::abs(c2.b - temp.b);
    return std::min(diff1, diff2);
  } 
  else 
  {
    std::cerr << "ERROR: Unknown choice of next edge to collapse" << std::endl;
    exit(1);
  }
}

void Edge::ReCalculateValue() 
{
  length = DistanceBetween(v1->getPoint(),v2->getPoint());
  legal = mesh->isLegalCollapse(this);
}


bool Edge::CheckValue() const 
{
  double length2 = DistanceBetween(v1->getPoint(),v2->getPoint());
  bool legal2 = mesh->isLegalCollapse((Edge*)this);
  bool error = false;
  if (legal != legal2) 
  {
    std::cout << "LEGALITY ERROR! " << *this << " " << legal << " should be " << legal2 << std::endl;
    error = true;
  }
  if (fabs(length-length2) > 0.0001) 
  {
    std::cout << "LENGTH ERROR!   " << *this << " " << length << " should be " << length2 << std::endl;
    error = true;
  }
  return error;
}

// Comparison functions necessary for placing edges in a heap
bool operator==(const Edge &a, const Edge& b) 
{
  assert (a.getV1()->getID() < a.getV2()->getID());
  assert (b.getV1()->getID() < b.getV2()->getID());
  return (a.getV1()->getID() == b.getV1()->getID() &&
          a.getV2()->getID() == b.getV2()->getID());
}
bool operator<(const Edge &a, const Edge& b) 
{
  assert (a.getV1()->getID() < a.getV2()->getID());
  assert (b.getV1()->getID() < b.getV2()->getID());
  return (a.getV1()->getID() < b.getV1()->getID() ||
          (a.getV1()->getID() == b.getV1()->getID() &&
           a.getV2()->getID() < b.getV2()->getID()));
}

// =================================================================
// stream printing functions for Edge and Edge*
std::ostream& operator<<(std::ostream &ostr, const Edge& e) 
{
  ostr << "EDGE " << e.getV1()->getID() << " " << e.getV2()->getID();
  if (e.isLegal()) 
  {
    ostr << "  len="  << DistanceBetween(e.getV1()->getPoint(),e.getV2()->getPoint());
  }
  return ostr;
}

std::ostream& operator<<(std::ostream &ostr, Edge* e) 
{
  ostr << *e; 
  return ostr;
}