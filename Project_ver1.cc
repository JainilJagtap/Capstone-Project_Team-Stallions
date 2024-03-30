#include <iostream>
#include <fstream>
#include <string>
#define IgnoreNewChar cin.ignore                  //for ignoring newline character
using namespace std;


//Class for defining task
class Task
{  
public:
    string description;
    int priority;
};

//creating a linked list 
class Node
{
public:
    string data;
    int priority;
    Node *next;

    Node(string d, int p) : data(d), priority(p), next(nullptr) {}
};

//using the linked list created, creating a class priority queue
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


//function to add a task to the file
void addTask(const string& fileName) {
    ofstream fileOut(fileName, ios::app); 
    Task newTask;
    cout << "Enter the task description: ";
    IgnoreNewChar; 
    getline(cin, newTask.description);

    cout << "Enter the priority: ";
    cin >> newTask.priority;
    IgnoreNewChar;
    
    if(fileOut.is_open()){
        fileOut << newTask.description << "|" << newTask.priority << endl;               //we are using '|' symbol to separate the task description and priority in the file
        cout << "Task successfully added to the file." << endl;
        fileOut.close();
    }
    else{
        cout << "Unable to open file to write task." << endl;
    }
}


//function to load any left tasks from the file to priority queue
void FileToQueue(PriorityQueue& pq, const string& fileName){
    ifstream fileIn(fileName);
    if(fileIn.is_open()){
        string s;
        while(getline(fileIn, s)){
            Task task;
            int sep;
            for(sep = 0; sep < s.size(); sep++){
                if(s[sep] == '|') break;
                else task.description += s[sep];
            }
            task.priority = s[sep+1]-'0';                 //this will give the numerical priority stored in the file after the symbol '|'
            pq.push(task.description, task.priority);
        }
        fileIn.close(); 
        cout << "Task successfully transferred from file to priority queue." << endl;
    }
    else{
        cout << "Unable to open file for file to queue transfer." << endl;
    }
}


int main(){
    string fileName = "Task.txt";
    PriorityQueue pq;
    
    FileToQueue(pq,fileName);         //loading all tasks from the file to priority queue at the beginning of program since there could be some left over tasks in file
    
    while(true){
        cout << "To add a task press 2" << endl;
        cout << "To mark a task done press 3" << endl;
        cout << "To get deadline of task with maximum priority press 4" << endl;
        cout << "To see the deadline of all pending task press 5" << endl;
        cout << "To exit the program press 6" << endl;

        int num;
        cin >> num;

        if(num == 6) break;

        if(num == 2){
            addTask(fileName);
            FileToQueue(pq, fileName);
        }

        if(num==3)
        {
            cout<<pq.peek()<<endl;
        }
    }
    return 0;
}
