#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

class task
{
};

struct Node
{
    int data;
    int priority;
    Node *next;
};

Node *newNode(int d, int p)
{
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->data = d;
    temp->priority = p;
    temp->next = nullptr;
    return temp;
}

int peek(Node *head)
{
    return head->data;
}

void pop(Node *&head)
{
    Node *temp = head;
    head = head->next;
    free(temp);
}

void push(Node *&head, int d, int p)
{
    Node *start = head;

    Node *temp = newNode(d, p);
    if (head->priority < p)
    {
        temp->next = head;
        head = temp;
    }
    else
    {
        while (start->next != nullptr && start->next->priority > p)
        {
            start = start->next;
        }

        temp->next = start->next;
        start->next = temp;
    }
}

bool isEmpty(Node *head)
{
    return head == nullptr;
}

int main()
{
    while (1)
    {
        cout << "To add a task press 2" << endl;
        cout << "To mark a task done press 3" << endl;
        cout << "To get deadline of task with maximum priority press 4" << endl;
        cout << "To see the deadline of all pending task press 5" << endl;
        cout << "To exit the program press 6" << endl;

        int num;
        cin >> num;

        if (num == 6)
            break;

        if (num == 2)
        {
            Node *pq;
            pq = newNode(4, 5);
            push(pq, 5, 3);
            push(pq, 4, 3);
            push(pq, 4, 5);
            push(pq, 7, 7);

            while(!isEmpty(pq))
            {
                cout<<peek(pq)<<" ";
                pop(pq);
            }
        }
    }
    return 0;
}
