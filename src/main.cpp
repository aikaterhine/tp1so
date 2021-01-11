#include "../include/main.hpp"
#include "../src/List.cpp"

int main (int argc, char * argv []){
  struct Node *last = NULL;

  last = insertInEmpty(last, 30);
  last = insertAtBegin(last, 20);
  last = insertAtBegin(last, 10);
  last = insertAtEnd(last, 40);
  last = insertAtEnd(last, 60);
  last = insertAfter(last, 50,40 );
  cout<<"The circular linked list created is as follows:"<<endl;
  traverseList(last);
  deleteNode(&last,10);
}
