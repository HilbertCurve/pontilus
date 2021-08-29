/* date = August 22nd 2021 0:36 pm */

#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

namespace Application
{
    namespace Collections
    {
        struct LinkedListNode
        {
            int value;
            LinkedListNode* next;
        };
        
        struct LinkedList
        {
            int size;
            LinkedListNode* first;
        };
        
        
        LinkedList GenerateLinkedList(int size);
        LinkedListNode* FollowToEnd(LinkedListNode* node);
        void AddNodeToLinkedList(LinkedList ll, LinkedListNode* node);
        LinkedListNode* GetElementAt(LinkedList ll, int pos);
    }
}

#endif //_LINKED_LIST_H
