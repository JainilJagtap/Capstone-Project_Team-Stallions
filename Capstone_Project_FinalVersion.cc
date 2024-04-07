#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip> //used for unicode character and ascii art handling
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

// Creating a linked list
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

        // extracting the current computer date using ctime library
        time_t now = time(0);
        tm *ltm = localtime(&now);
        int curr_year = 1900 + ltm->tm_year;
        int curr_month = 1 + ltm->tm_mon;
        int curr_day = ltm->tm_mday;

        // checking if the days remaining for the task is less than the task already present in priority queue and arranging the tasks in the correct order
        int remaining_days = (y - curr_year) * 365 + (m - curr_month) * 30 + (dy - curr_day);

        if (head == nullptr || (((head->year - curr_year) * 365 + (head->month - curr_month) * 30 + (head->day - curr_day)) > remaining_days) || ((((head->year - curr_year) * 365 + (head->month - curr_month) * 30 + (head->day - curr_day))) == remaining_days && p > head->priority))
        {
            temp->next = head;
            head = temp;
        }
        else
        {
            Node *start = head;

            while (start->next != nullptr && ((((start->next->year - curr_year) * 365 + (start->next->month - curr_month) * 30 + (start->next->day - curr_day)) < remaining_days) || ((((start->next->year - curr_year) * 365 + (start->next->month - curr_month) * 30 + (start->next->day - curr_day)) == remaining_days) && start->next->priority > p)))
            {
                start = start->next;
            }
            temp->next = start->next;
            start->next = temp;
        }
    }

    // function to display task description of the approaching task
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

    // function to display the priority queue
    void printPQ()
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
    cout << "Enter the task description to add: ";
    IgnoreNewChar;
    getline(cin, newTask.description);

    cout << "Enter the Importance level of the task ranging in (0-9): "; // Importance level will be ranging in 0-9 only
    cin >> newTask.priority;
    IgnoreNewChar;
    cout << "Enter the deadline date of the task as (dd-mm-yyyy): ";
    cin >> newTask.date;
    IgnoreNewChar;

    if (fileOut.is_open())
    {
        fileOut << newTask.description << "," << newTask.priority << "," << newTask.date << endl; // we are using ',' symbol to separate the task description and priority in the file
        cout << "Task successfully added." << endl;
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
    yy = newTask.date.substr(6, 4);
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
            task.priority = s[sep] - '0'; // this will give the numerical priority stored in the file after the symbol ','

            string dd, mm, yy;
            dd = s.substr(sep + 2, 2);
            mm = s.substr(sep + 5, 2);
            yy = s.substr(sep + 8, 4);

            int ddd, mmm, yyy;
            ddd = stoi(dd);
            mmm = stoi(mm);
            yyy = stoi(yy);
            pq.push(task.description, task.priority, ddd, mmm, yyy);
        }
        fileIn.close();

        // cout << "Task successfully transferred from file to priority queue." << endl;
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
                    yy = s.substr(i + 7, 4);

                    int ddd, mmm, yyy;
                    ddd = stoi(dd);
                    mmm = stoi(mm);
                    yyy = stoi(yy);

                    time_t now = time(0);
                    tm *ltm = localtime(&now);
                    int curr_year = 1900 + ltm->tm_year;
                    int curr_month = 1 + ltm->tm_mon;
                    int curr_day = ltm->tm_mday;

                    int remaining_days = (yyy - curr_year) * 365 + (mmm - curr_month) * 30 + (ddd - curr_day);

                    cout << "***************************************************************************************" << endl;
                    cout << "Remainder!!!" << endl;
                    cout << "Days remaining for the most important task: " << remaining_days << endl;
                    cout << "Deadline for the same task is on: " << dd << "-" << mm << "-" << yy << endl;
                    cout << "***************************************************************************************" << endl;
                    break;
                }
            }
        }
    }
    else
    {
        cout << "Unable to open file for reading." << endl;
    }
}

// function to delete a given task from file and priority queue
void deletetask(const string &s, const string &fileName, PriorityQueue &pq)
{
    // deleting task from file
    ifstream fileIn(fileName);
    ofstream newFileOut("temp.txt"); // opening a temporary file to write
    if (fileIn.is_open() && newFileOut.is_open())
    {
        string x;
        int check = 0;
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
                // Skip writing this task to the temporary file
                check = 1;
                continue;
            }
            else
            {
                newFileOut << des << "," << prio << "," << dat << endl;
            }
        }
        fileIn.close();
        newFileOut.close();

        // Remove the temporary file and rename the new one
        remove(fileName.c_str());
        rename("temp.txt", fileName.c_str());

        if (check == 1)
            cout << "Task successfully deleted." << endl;
        else
            cout << "Task not found in the list" << endl;
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
            while (temp->next != nullptr && temp->next->data != s) // going to the node with the user inputted data in the linked list
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

// function to get remaining days of a task inputted by user
int getdeadline(string &a, PriorityQueue &pq, const string &fileName)
{
    string date;
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
                    yy = s.substr(i + 7, 4);

                    int ddd, mmm, yyy;
                    ddd = stoi(dd);
                    mmm = stoi(mm);
                    yyy = stoi(yy);

                    time_t now = time(0);
                    tm *ltm = localtime(&now);
                    int curr_year = 1900 + ltm->tm_year;
                    int curr_month = 1 + ltm->tm_mon;
                    int curr_day = ltm->tm_mday;

                    int remaining_days = (yyy - curr_year) * 365 + (mmm - curr_month) * 30 + (ddd - curr_day);
                    return remaining_days;
                    break;
                }
            }
        }
    }
    else
    {
        cout << "Unable to open file for reading." << endl;
    }
    return 0;
}

// function to mark a task as done
void markTask(string &fileName, PriorityQueue &pq)
{
    string completedTask;
    cout << "Enter the task description of the task to mark as done: ";
    IgnoreNewChar;
    getline(cin, completedTask);

    // adding the task to a new file Tasks_completed
    ofstream myFile("Tasks_completed.txt", ios::app);
    if (myFile.is_open())
    {
        myFile << completedTask << endl;
        myFile.close();
    }
    else
    {
        cout << "Unable to open file for writing completed tasks." << endl;
    }

    // deleting the task from original file
    deletetask(completedTask, fileName, pq);
}

// function to view all the completed tasks
void viewcompletedtask()
{
    cout << "Tasks that are completed are:" << endl;
    ifstream fileIn("Tasks_completed.txt");
    if (fileIn.is_open())
    {
        string s;
        while (getline(fileIn, s))
        {
            cout << s << endl;
        }
        fileIn.close();
    }
    else
    {
        cout << "Unable to open file for reading completed tasks." << endl;
    }
}

// function to automatically mark task as missing if they go past deadline
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

    cout << "  _____   ___    ___    ____ ______  ____    _  __   ____        ___    ___   ____      __   ____  _____ ______" << endl;
    cout << " / ___/  / _ |  / _ \\  / __//_  __/ / __ \\  / |/ /  / __/       / _ \\  / _ \\ / __ \\ __ / /  / __/ / ___//_  __/" << endl;
    cout << "/ /__   / __ | / ___/ _\\ \\   / /   / /_/ / /    /  / _/        / ___/ / , _// /_/ // // /  / _/  / /__   / /   " << endl;
    cout << "\\___/  /_/ |_|/_/    /___/  /_/    \\____/ /_/|_/  /___/       /_/    /_/|_| \\____/ \\___/  /___/  \\___/  /_/    " << endl;
    cout << "                                                                                                                " << endl;

    cout << " _      __   ____   __   _____  ____    __  ___   ____      ______  ____ " << endl;
    cout << "| | /| / /  / __/  / /  / ___/ / __ \\  /  |/  /  / __/     /_  __/ / __ \\" << endl;
    cout << "| |/ |/ /  / _/   / /__/ /__  / /_/ / / /|_/ /  / _/        / /   / /_/ /" << endl;
    cout << "|__/|__/  /___/  /____/\\___/  \\____/ /_/  /_/  /___/       /_/    \\____/" << endl;
    cout << " ______   ___    ____   __ __        ____  _____   __ __   ____   ___   __  __   __    ____   ___ " << endl;
    cout << "/_  __/  / _ |  / __/  / //_/       / __/ / ___/  / // /  / __/  / _ \\ / / / /  / /   / __/  / _ \\" << endl;
    cout << " / /    / __ | _\\ \\   / ,<         _\\ \\  / /__   / _  /  / _/   / // // /_/ /  / /__ / _/   / ,_/" << endl;
    cout << "/_/    /_/ |_|/___/  /_/|_|       /___/  \\___/  /_//_/  /___/  /____/ \\____/  /____//___/  /_/|_|" << endl;
    cout << "                                                                                                  " << endl;
    cout<<endl;
    cout<<endl;

    FileToQueue(pq, fileName); // loading all tasks from the file to priority queue at the beginning of program since there could be some left over tasks in file

    while (true)
    {
        // this would add missing to task if deadline has gone past everytime program is running
        missingTask(fileName, pq);
        // this would give the remainder in console everytime the program is running
        remainderfunc(pq, fileName);

        cout<<endl;
        cout << "╔══════════════════════════════════════════════════════════╗" << endl;
        cout << "║                    𝓣𝓐𝓢𝓚   𝓢𝓒𝓗𝓔𝓓𝓤𝓛𝓔𝓡                      ║" << endl;
        cout << "╠══════════════════════════════════════════════════════════╣" << endl;
        cout << "║     𝓟𝓵𝓮𝓪𝓼𝓮 𝓼𝓮𝓵𝓮𝓬𝓽 𝓪𝓷 𝓸𝓹𝓽𝓲𝓸𝓷 𝓯𝓻𝓸𝓶  𝓽𝓱𝓮 𝓵𝓲𝓼𝓽 𝓫𝓮𝓵𝓸𝔀:        ║" << endl;
        cout << "║                                                          ║" << endl;
        cout << "║   1. ➤ Add a task                                        ║" << endl;
        cout << "║   2. ➤ Delete a task                                     ║" << endl;
        cout << "║   3. ➤ Get Task with maximum importance level            ║" << endl;
        cout << "║   4. ➤ See deadline of a particular task                 ║" << endl;
        cout << "║   5. ➤ Mark a task as done                               ║" << endl;
        cout << "║   6. ➤ View completed tasks                              ║" << endl;
        cout << "║   7. ➤ Display all tasks in order                        ║" << endl;
        cout << "║   8. ➤ Exit the program                                  ║" << endl;
        cout << "╚══════════════════════════════════════════════════════════╝" << endl;

        int num;
        cin >> num;

        if (num == 8)
            break;

        if (num == 1)
        {
            addTask(fileName, pq);
        }
        if (num == 2)
        {
            string str;
            cout << "Enter task description of the task to be deleted: ";
            IgnoreNewChar;
            getline(cin, str);
            deletetask(str, fileName, pq);
        }
        if (num == 3)
        {
            cout<<"Task with maximum importance level and closest deadline is: "<<endl;
            cout << pq.peek() << endl;
        }
        if (num == 4)
        {
            string a;
            cout << "Enter the task description of the task whose deadline you want to view: " << endl;
            IgnoreNewChar;
            getline(cin, a);
            int remDays = getdeadline(a, pq, fileName);
            cout << "Remaining days for the given task is: " << remDays << endl;
        }
        if (num == 5)
        {
            markTask(fileName, pq);
        }
        if (num == 6)
        {
            viewcompletedtask();
        }
        if (num == 7)
        {
            cout << "Tasks that are left are in this order: " << endl;
            pq.printPQ();
        }
    }
    return 0;
}