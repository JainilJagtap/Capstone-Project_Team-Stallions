#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#define IgnoreNewChar cin.ignore() // for ignoring newline character
using namespace std;

// Class for defining task
class Task
{
public:
    string description;
    int priority;
    string date;
};

// creating a linked list
class Node
{
public:
    string data;
    int priority;
    int day, month, year;
    Node *next;

    Node(string d, int p, int dy, int m, int y) : data(d), priority(p), day(dy), month(m), year(y), next(nullptr) {}
};

// using the linked list created, creating a class priority queue
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

        int remaining_days = (y - curr_year) * 365 + (m - curr_month) * 30 + (dy - curr_day);

        if (head == nullptr || (((head->year - curr_year) * 365 + (head->month - curr_month) * 30 + (head->day - curr_day)) > remaining_days) || ((((head->year - curr_year) * 365 + (head->month - curr_month) * 30 + (head->day - curr_day))) == remaining_days && p > head->priority))
        {
            temp->next = head;
            head = temp;
        }
        else
        {
            Node *start = head;

            // int deadline = (y-curr_year)+(m-curr_month)+(dy-curr_day);
            while (start->next != nullptr && ((((start->next->year - curr_year) * 365 + (start->next->month - curr_month) * 30 + (start->next->day - curr_day)) < remaining_days) || ((((start->next->year - curr_year) * 365 + (start->next->month - curr_month) * 30 + (start->next->day - curr_day)) == remaining_days) && start->next->priority > p)))
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
        return "Priority queue is empty";
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

    Node *getHead()
    {
        return head;
    }

    void printPQ() // displaying the priority queue
    {
        Node *temp = head;
        if (temp == NULL)
        {
            cout << "No tasks in the Task manager" << endl;
        }
        else
        {
            while (temp)
            {
                cout << temp->data << endl;
                temp = temp->next;
            }
        }
    }

    ~PriorityQueue()
    {
        while (!isEmpty())
            pop();
    }
};

// function to add a task to the file and priority queue
void addTask(const string &fileName, PriorityQueue &pq)
{
    // adding task to the file

    ofstream fileOut(fileName, ios::app);
    Task newTask;
    cout << "Enter the task description: ";
    IgnoreNewChar;
    getline(cin, newTask.description);

    cout << "Enter the priority: ";
    cin >> newTask.priority;
    IgnoreNewChar;
    cout << "Enter the deadline date as dd mm yy: ";
    cin >> newTask.date;
    IgnoreNewChar;

    if (fileOut.is_open())
    {
        fileOut << newTask.description << "," << newTask.priority << "," << newTask.date << endl; // we are using ',' symbol to separate the task description and priority in the file
        cout << "Task successfully added to the file." << endl;
        fileOut.close();
    }
    else
    {
        cout << "Unable to open file to write task." << endl;
    }

    // adding task to the priority queue
    int pday, pmonth, pyear;
    string dd, mm, yy;
    dd = newTask.date.substr(0, 2);
    mm = newTask.date.substr(3, 2);
    yy = newTask.date.substr(6, 2);
    pday = stoi(dd);
    pmonth = stoi(mm);
    pyear = stoi(yy);
    pq.push(newTask.description, newTask.priority, pday, pmonth, pyear);
}

// function to load any left tasks from the file to priority queue
void FileToQueue(PriorityQueue &pq, const string &fileName)
{
    ifstream fileIn(fileName);
    if (fileIn.is_open())
    {
        string s;
        while (getline(fileIn, s))
        {
            Task task;
            int sep;
            for (sep = 0; sep < s.size(); sep++)
            {
                if (s[sep] == ',')
                    break;
                else
                    task.description += s[sep];
            }
            sep++;
            task.priority = s[sep] - '0'; // this will give the numerical priority stored in the file after the symbol '|'

            string dd, mm, yy;
            dd = s.substr(sep + 2, 2);
            mm = s.substr(sep + 5, 2);
            yy = s.substr(sep + 8, 2);

            int ddd, mmm, yyy;
            ddd = stoi(dd);
            mmm = stoi(mm);
            yyy = stoi(yy);
            pq.push(task.description, task.priority, ddd, mmm, yyy);
        }
        fileIn.close();
        cout << "Task successfully transferred from file to priority queue." << endl;
    }
    else
    {
        cout << "Unable to open file for file to queue transfer." << endl;
    }
}

// function which implements remainder functionality, it is called automatically everytime the program runs
void remainderfunc(PriorityQueue &pq, const string &fileName)
{
    string a;
    string date;
    a = pq.peek();
    ifstream fileIn(fileName);

    if (fileIn.is_open())
    {
        string s;
        while (getline(fileIn, s))
        {
            string x;
            int j = 0;
            while (s[j] != ',')
            {
                x += s[j];
                j++;
            }
            if (x == a)
            {
                int i = 0;
                while (s[i] != ',')
                {
                    i++;
                }
                i++;
                while (i < s.size() && s[i] != ',')
                {
                    i++;
                }
                if (i < s.size())
                {
                    string dd, mm, yy;
                    dd = s.substr(i + 1, 2);
                    mm = s.substr(i + 4, 2);
                    yy = s.substr(i + 7, 2);

                    int ddd, mmm, yyy;
                    ddd = stoi(dd);
                    mmm = stoi(mm);
                    yyy = stoi(yy);

                    time_t now = time(0);
                    tm *ltm = localtime(&now);
                    int curr_year = ltm->tm_year - 100;
                    int curr_month = 1 + ltm->tm_mon;
                    int curr_day = ltm->tm_mday;

                    int remaining_days = (yyy - curr_year) * 365 + (mmm - curr_month) * 30 + (ddd - curr_day);
                    cout << "Days remaining for most important task: " << remaining_days << endl;
                    break;
                }
            }
        }
    }
    else
    {
        cout << "Unable to open file for reading." << endl; // Debugging output
    }
}

// function to delete a given task from file and priority queue
void deletetask(const string &s, const string &fileName, PriorityQueue &pq)
{
    // deleting task from file
    ifstream fileIn(fileName);
    ofstream newFileOut("temp.txt");
    if (fileIn.is_open() && newFileOut.is_open())
    {
        string x;
        while (getline(fileIn, x))
        {
            string des;
            int i;
            for (i = 0; i < x.size(); i++)
            {
                if (x[i] == ',')
                    break;
                else
                {
                    des += x[i];
                }
            }
            int prio = x[i + 1] - '0';
            i = i + 3;
            string dat;
            for (int j = i; j < x.size(); j++)
            {
                dat += x[j];
            }

            if (des == s)
            {
                // Skip writing this task to the new file
                continue;
            }
            else
            {
                newFileOut << des << "," << prio << "," << dat << endl;
            }
        }
        fileIn.close();
        newFileOut.close();

        // Remove the old file and rename the new one
        remove(fileName.c_str());
        rename("temp.txt", fileName.c_str());
        cout << "Task successfully deleted from the file." << endl;
    }
    else
    {
        cout << "Unable to open file for task deletion." << endl;
    }

    // deleting task from priority-queue
    Node *head = pq.getHead();
    if (head == nullptr)
    {
        cout << "Priority queue is empty. Can't delete the task." << endl;
    }
    else
    {
        if (head->data == s)
        {
            pq.pop(); // Removing the head of the priority queue
        }
        else
        {
            Node *temp = head;
            while (temp->next != nullptr && temp->next->data != s)     //going to the node with the user inputted data in the linked list
            {
                temp = temp->next;
            }
            if (temp->next != nullptr)                       //delete this node
            {
                Node *tempNext = temp->next;
                temp->next = tempNext->next;
                delete tempNext;
            }
            else
            {
                cout << "Task not found in the priority queue." << endl;
            }
        }
    }
}

// function to get deadline of a task inputted by user
void getdeadline(PriorityQueue &pq, const string &fileName)
{
    string a;
    string date;
    cout << "Enter the task name" << endl;
    cin >> a;
    IgnoreNewChar;

    ifstream fileIn(fileName);

    if (fileIn.is_open())
    {
        string s;
        while (getline(fileIn, s))
        {
            string x;
            int j = 0;
            while (s[j] != ',')
            {
                x += s[j];
                j++;
            }
            if (x == a)
            {
                int i = 0;
                while (s[i] != ',')
                {
                    i++;
                }
                i++;
                while (i < s.size() && s[i] != ',')
                {
                    i++;
                }
                if (i < s.size())
                {
                    string dd, mm, yy;
                    dd = s.substr(i + 1, 2);
                    mm = s.substr(i + 4, 2);
                    yy = s.substr(i + 7, 2);

                    int ddd, mmm, yyy;
                    ddd = stoi(dd);
                    mmm = stoi(mm);
                    yyy = stoi(yy);

                    time_t now = time(0);
                    tm *ltm = localtime(&now);
                    int curr_year = ltm->tm_year - 100;
                    int curr_month = 1 + ltm->tm_mon;
                    int curr_day = ltm->tm_mday;

                    int remaining_days = (yyy - curr_year) * 365 + (mmm - curr_month) * 30 + (ddd - curr_day);
                    cout << "Days remaining for the task: " << remaining_days << endl;
                    break;
                }
            }
        }
    }
    else
    {
        cout << "Unable to open file for reading." << endl; // Debugging output
    }
}


// function to add missing tasks
void missingTask(string &fileName, PriorityQueue &pq)
{
    ifstream fileIn(fileName);
    ofstream fileOut("temp1.txt");
    if (fileIn.is_open())
    {
        string s;
        while (getline(fileIn, s))
        {
            string des;
            int i;
            for (i = 0; i < s.size(); i++)
            {
                if (s[i] == ',')
                    break;
                else
                {
                    des += s[i];
                }
            }
            int prio = s[i + 1] - '0';
            i = i + 3;
            string dat;
            int j;
            for (j = i; j < s.size(); j++)
            {
                if (s[j] == ',')
                    break;
                dat += s[j];
            }

            // this will check if the task has gone past deadline and add missing to it in file
            if (getdeadline(des, pq, fileName) < 0)
            {
                if (s[j + 1] != ',')
                    fileOut << des << "," << prio << "," << dat << ","
                            << "missing" << endl;
            }
            else
            {
                fileOut << des << "," << prio << "," << dat << endl;
            }

            // now once task is marked missing in file it should be deleted from priority queue
            if (getdeadline(des, pq, fileName) < 0)
            {
                Node *head = pq.getHead();
                if (head == nullptr)
                {
                    cout << "No tasks left in the Task Manager." << endl;
                }
                else
                {
                    if (head->data == des)
                    {
                        pq.pop(); // Removing the head of the priority queue
                    }
                    else
                    {
                        Node *temp = head;
                        while (temp->next != nullptr && temp->next->data != des) // going to the node with the user inputted data in the linked list
                        {
                            temp = temp->next;
                        }
                        if (temp->next != nullptr) // delete this node
                        {
                            Node *tempNext = temp->next;
                            temp->next = tempNext->next;
                            delete tempNext;
                        }
                        else
                        {
                            cout << "Task not found in the priority queue." << endl;
                        }
                    }
                }
            }
        }
        fileIn.close();
        fileOut.close();
        // removing the original file and renaming the temporary file to original file
        remove(fileName.c_str());
        rename("temp1.txt", fileName.c_str());
    }
    else
    {
        cout << "Unable to open file for reading." << endl;
    }
}


int main()
{
    string fileName = "Task_List.txt";
    PriorityQueue pq;

    FileToQueue(pq, fileName); // loading all tasks from the file to priority queue at the beginning of program since there could be some left over tasks in file

    while (true)
    {
        // this would give the remainder in console everytime the program is running
        remainderfunc(pq, fileName);

        cout << "To add a task press 2" << endl;
        cout << "To delete a task press 3" << endl;
        cout << "To get deadline of task with maximum priority press 4" << endl;
        cout << "To see the deadline of a particular task press 5" << endl;
        cout << "To exit the program press 6" << endl;

        int num;
        cin >> num;

        if (num == 6)
            break;

        if (num == 2)
        {
            addTask(fileName, pq);
        }
        if (num == 3)
        {
            string str;
            cin >> str;
            deletetask(str, fileName, pq);
        }
        if (num == 4)
        {
            cout << pq.peek() << endl;
        }
        if (num == 5)
        {
            getdeadline(pq, fileName);
        }
    }
    return 0;
}
