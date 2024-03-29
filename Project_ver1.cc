#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#define IgnoreNewChar cin.ignore()                           //this will ignore any newline character
using namespace std;

//Class for defining task
class Task
{  
    public:
    string description;
    int priority;

};
class Node
{
public:
    string data;
    int priority;
    Node *next;

    Node(string d, int p) : data(d), priority(p), next(nullptr) {}
};

class PriorityQueue
{
private:
    Node *head;

public:
    PriorityQueue() : head(nullptr) {}

    void push(string d, int p)
    {
        Node *temp = new Node(d, p);
        if (head == nullptr || head->priority < p)
        {
            temp->next = head;
            head = temp;
        }
        else
        {
            Node *start = head;
            while (start->next != nullptr && start->next->priority > p)
            {
                start = start->next;
            }
            temp->next = start->next;
            start->next = temp;
        }
    }

    string peek()
    {
        if (head != nullptr)
            return head->data;
        return "";
    }

    void pop()
    {
        if (head == nullptr)
            return;
        Node *temp = head;
        head = head->next;
        delete temp;
    }

    bool isEmpty()
    {
        return head == nullptr;
    }

    ~PriorityQueue()
    {
        while (!isEmpty())
            pop();
    }
};

void addTask(string& filename){

    ofstream fileOut(filename);               //opening the task file to add tasks
    Task newTask;
    cout<<"Enter the task description: ";
    getline(cin,newTask.description);
    cout<<"Enter the priority: ";
    cin>>newTask.priority;
    IgnoreNewChar;

    //adding task to the file
    if(fileOut.is_open()){
        fileOut<<newTask.description<<"|"<<newTask.priority<<endl;                  //here '|' separates the description and priority in file
        fileOut.close();
        cout<<"Task successfully added to the file."<<endl;
    }
    else{
        cout<<"Unable to open file to write task."<<endl;
    }

}

//this function loads all the tasks left in the file to the priority queue
void FileToQueue(Priority_Queue* pq,string& filename){
    ifstream fileIn(filename);
    if(fileIn.is_open()){
        string s;
        while(fileIn.eof()==0){
            getline(fileIn,s);
            Task task;
            int sep;
            for(sep=0;sep<s.size();sep++){
                if(s[sep]=='|') break;
                else{
                    task.description+=s[sep];
                }
            }
            task.priority=s[sep+1];
            //pushing task from file to priority queue
            push(pq,task.description,task.priority);        //for now this push function is of priority queue which is to be reimplemented in the class
        }
        fileIn.close();
        cout<<"Task successfully transferred from file to priority queue."<<endl;
    }
    else{
        cout<<"Unable to open file for file to queue transfer."<<endl;
    }
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
            Priority_Queue *pq;
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
