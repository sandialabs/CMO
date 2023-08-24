#include <cstdio>
#include <iostream>
#include <set>

#ifdef USEGRAPHICS
#ifndef _graphics
#include "graphics.h"
#endif 
#endif


#ifndef CONTACTMAP_H
#define CONTACTMAP_H





using namespace std;



/* Here we define the class Contact. A contact map is going to be a collection
   of Contact instances 
*/

class Contact {
private:
  unsigned   _Left, _Right;
public:
  Contact() { _Left = _Right = 0; };
  Contact(unsigned left, unsigned right) {
    if (left < right) {
      _Left  = left;
      _Right = right;
    } else {
      _Left  = right;
      _Right = left;
    }
  };
  Contact(Contact const &C) {
    _Left = C._Left;
    _Right = C._Right;
  };
  ~Contact() {};

  Contact &operator=(Contact const &C) {
    _Left = C._Left;
    _Right = C._Right;
    return(*this);
  };

  bool     operator<(Contact const &C) const {
    return((_Left < C._Left) ||
           ((_Left == C._Left) && 
            (_Right < C._Right)));
  };

  bool     operator==(Contact const &C) const {
    return((_Left == C._Left) && (_Right == C._Right));
  };

  unsigned  left(void)  const { return(_Left); };
  unsigned  right(void) const { return(_Right); };
};


typedef set<Contact>::iterator ContactMapIterator;




/* Here we define the class ContactMap. An instance of a contact map will hold
a collection (set) of contact (the class is defined above), and a threshodl */

class ContactMap {
private:

  set<Contact>        _Contacts;
  double              _Threshold;
  char*               _theProteinName;
  int                 _numberOfResidues;
  int                 _numberOfContacts;

public:
  ContactMap();
  ContactMap(char *);
  ContactMap(double t);
  ~ContactMap();


  void ContactMap::doRead(char * FILENAME);

  char *   generateDescription(void) 
  {
   static char s[170];
   sprintf(s,"Contact Map with %d contacts at threshold %f",
            _Contacts.size(), _Threshold);
   return (s);
  };


#ifdef _graphics
  void      draw(NKGraphicPanel aGP, int startX, int startY,bool up);
#endif

  void      addContact(unsigned a, unsigned b) {
    _Contacts.insert(Contact(a,b));
  };

  void      delContact(unsigned a, unsigned b) {
    _Contacts.erase(Contact(a,b));
  };

  bool      isContact(unsigned a, unsigned b) {
    if (_Contacts.find(Contact(a,b)) != _Contacts.end())
      return(true);
    else
      return(false);
  };
 
  double    getThreshold(void) {
    return(_Threshold);
  };
  void      setThreshold(double t) {
    _Threshold = t;

  };

  void setProteinName(char * aP)
  {
   _theProteinName = aP;
  }

  char * getProteinName()
  {
   return _theProteinName;
  }


  unsigned  getNumberOfContacts(void) {
    return(_Contacts.size());
  };



  unsigned  getNumberOfResidues(void) {
    return(_numberOfResidues);
  };

  void setNumberOfResidues(unsigned num) {
    _numberOfResidues = num;
  };

  ContactMapIterator  begin(void) {
    return(_Contacts.begin());
  };

  ContactMapIterator  end(void) {
    return(_Contacts.end());
  };
};

#endif  //  CONTACTMAP_H



