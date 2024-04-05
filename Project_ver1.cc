#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#define IgnoreNewChar cin.ignore()                  //for ignoring newline character
using namespace std;


//Class for defining task
class Task
{  
public:
    string description;
    int priority;
    string date;
};

//creating a linked list 
class Node
{
public: 
    string data;
    int priority;
    int day,month,year;
    Node *next;

    Node(string d, int p,int dy,int m,int y) : data(d), priority(p),day(dy), month(m), year(y), next(nullptr) {}
};

//using the linked list created, creating a class priority queue
class PriorityQueue
{
private:
    Node *head;

public:
    PriorityQueue() : head(nullptr) {}

    void push(string d, int p, int dy, int m, int y)
    {
        Node *temp = new Node(d, p, dy, m, y);
        time_t now = time(0);
        tm *ltm = localtime(&now);
        int curr_year = 1900 + ltm->tm_year;
        int curr_month = 1 + ltm->tm_mon;
        int curr_day = ltm->tm_mday;

        int remaining_days=(y-curr_year)*365+(m-curr_month)*30+(dy-curr_day);

        if (head == nullptr || (((head->year-curr_year)*365+(head->month-curr_month)*30+(head->day-curr_day))>remaining_days) || ((((head->year-curr_year)*365+(head->month-curr_month)*30+(head->day-curr_day)))==remaining_days && p>head->priority))
        {
            temp->next = head;
            head = temp;
        }
        else
        {
            Node *start = head;
            
            // int deadline = (y-curr_year)+(m-curr_month)+(dy-curr_day);
            while (start->next != nullptr && ((((start->next->year-curr_year)*365+(start->next->month-curr_month)*30+(start->next->day-curr_day))<remaining_days) || ((((start->next->year-curr_year)*365+(start->next->month-curr_month)*30+(start->next->day-curr_day))==remaining_days) && start->next->priority>p)))
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
    cout<<"Enter the deadline date as dd mm yy: ";
    cin>>newTask.date;
    IgnoreNewChar;

    if(fileOut.is_open()){
        fileOut << newTask.description << "," << newTask.priority << "," <<newTask.date<< endl;               //we are using '|' symbol to separate the task description and priority in the file
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
                if(s[sep] == ',') break;
                else task.description += s[sep];
            }
            sep++;
            task.priority = s[sep]-'0';                 //this will give the numerical priority stored in the file after the symbol '|'
            
            string dd,mm,yy;
            dd=s.substr(sep+2,2);
            mm=s.substr(sep+5,2);
            yy=s.substr(sep+8,2);

            int ddd,mmm,yyy;
            ddd=stoi(dd);
            mmm=stoi(mm);
            yyy=stoi(yy);
            pq.push(task.description, task.priority,ddd,mmm,yyy);
        }
        fileIn.close(); 
        cout << "Task successfully transferred from file to priority queue." << endl;
    }
    else{
        cout << "Unable to open file for file to queue transfer." << endl;
    }
}


int main(){
    string fileName = "Task_List.txt";
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
