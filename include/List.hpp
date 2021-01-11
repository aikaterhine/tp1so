#ifndef LIST_HPP
#define LIST_HPP

#include <string>
#include <iostream>

using namespace std;

  class List {

    private:
      struct Node;

    public:
      List();
      struct Node *insertInEmpty(struct Node*, int);
      struct Node *insertAtBegin(struct Node*, int);
      struct Node *insertAtEnd(struct Node*, int);
      struct Node *insertAfter(struct Node*, int, int);
      void traverseList(struct Node*);
      void deleteNode(Node**, int);
  };

#endif // LIST_HPP
