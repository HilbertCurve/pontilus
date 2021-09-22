#include "LinkedList.h"

namespace Pontilus
{
    namespace Collections
    {
        static LinkedListNode *GenerateLinkedListNode(int size)
        {
            LinkedListNode *ll = new LinkedListNode();
            ll->value = 0;

            if (size > 0)
            {
                size--;
                ll->next = GenerateLinkedListNode(size);
            }

            return ll;
        }

        LinkedList GenerateLinkedList(int length)
        {
            LinkedList ll = {length, GenerateLinkedListNode(length)};

            return ll;
        }

        LinkedListNode *FollowToEnd(LinkedListNode *node)
        {
            LinkedListNode *ptr = node;

            while (ptr->next != nullptr)
            {
                ptr = ptr->next;
            }

            return ptr;
        }

        void AddNodeToLinkedList(LinkedList ll, LinkedListNode *node)
        {
            LinkedListNode *last = FollowToEnd(ll.first);

            last->next = node;
        }

        LinkedListNode *GetElementAt(LinkedList ll, int pos)
        {
            int location = 0;

            LinkedListNode *ptr = ll.first;

            while (pos > location)
            {
                ptr = ptr->next;
                location++;
            }

            return ptr;
        }
    }
}
