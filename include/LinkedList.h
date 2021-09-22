/* date = August 22nd 2021 0:36 pm */

#ifndef _PONTILUS_LINKED_LIST_H
#define _PONTILUS_LINKED_LIST_H

namespace Pontilus
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

#endif //_PONTILUS_LINKED_LIST_H
