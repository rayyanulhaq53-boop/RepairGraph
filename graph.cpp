#include <iostream>
#include <string>
#include <list>
#include <queue> //imp for bfs traversal
#include <fstream> //save graph and load when start
using namespace std;

const int MAX_Nodes = 100; 

struct Node{
    int id;
    string name;
    string type; //Device, Symptom, Fault, Test, Part
};

struct Edge{
    int source;
    int destination;
    int weight;
    string relation;
};

struct FaultResult { //to store faults with there score calculted by weights
    int faultID;
    int score;
};

list<Edge> adj[MAX_Nodes];
int nodeCount = 0;
Node nodes[MAX_Nodes];

void addNode(int id, string name, string type){
    nodes[nodeCount].id = id;
    nodes[nodeCount].name = name;
    nodes[nodeCount].type = type;

    nodeCount++;
}
// adj[i] stores the neighbours of the node with id i+1 bcz nodes are starting from 1 and list index start from 0 as it is an array

void addEdge(int source, int destination, int weight, string relation)
{
    if (source < 1 || source > nodeCount || destination < 1 || destination > nodeCount){ //if source or destination does not exist
        cout << "Invalid node ID!" << endl;
        return;
    }
    Edge e;

    e.source = source;
    e.destination = destination;
    e.weight = weight;
    e.relation = relation; // tells why they are connected
    adj[source - 1].push_back(e); //we are doing -1 bcz node id starts from 1 and the array idex starts from 0 
}

int searchNode(string name)
{
    for (int i = 0; i < nodeCount; i++){
        if (nodes[i].name == name){
            return nodes[i].id;
        }
    }
    return -1;
}

void BFS(int startNode){
    if (startNode < 1 || startNode > nodeCount){
        cout << "Invalid starting node!" << endl;
        return;
    }

    bool visited[MAX_Nodes] = {false};
    queue<int> q;

    q.push(startNode);
    visited[startNode - 1] = true;

    while (!q.empty()){
        int current = q.front();
        q.pop();
        cout << nodes[current - 1].name << endl;

        for (list<Edge>::iterator it = adj[current - 1].begin(); it != adj[current - 1].end(); it++){
            int neighbour = it->destination;
            if (!visited[neighbour - 1]){
                visited[neighbour - 1] = true;
                q.push(neighbour);
            }
        }
    }
}


void DFS(int current, bool visited[]){
    visited[current - 1] = true;
    cout << nodes[current - 1].name << endl;
    for (list<Edge>::iterator it = adj[current - 1].begin(); it != adj[current - 1].end(); it++){
        int neighbour = it->destination;
        if (!visited[neighbour - 1]){
            DFS(neighbour, visited);
        }
    }
}

void bubbleSort(FaultResult results[], int resultCount){ //will be used to sort the faults in descending order
    for (int i = 0; i < resultCount - 1; i++){
        for (int j = 0; j < resultCount - i - 1; j++){
            if (results[j].score < results[j + 1].score){
                FaultResult temp = results[j];
                results[j] = results[j + 1];
                results[j + 1] = temp;
            }
        }
    }
}
void addFaultScore(FaultResult results[], int &resultCount, int faultID, int score);
int selectSymptom();

void collectFaultsBFS(int symptomID, FaultResult results[],int &resultCount){ //same as find possible faults the only difference is that it is to be reused for multiple symptoms and does not sort and display but collects and combines only
    //there is a shared array 
    bool visited[MAX_Nodes] = {false};
    queue<int> q;

    q.push(symptomID);
    visited[symptomID - 1] = true;

    while (!q.empty()){
        int current = q.front();
        q.pop();

        for (list<Edge>::iterator it = adj[current - 1].begin();it != adj[current - 1].end(); it++){
            int neighbour = it->destination;

            if (!visited[neighbour - 1]){
                if (it->relation == "Related to" ||
                    it->relation == "May indicate"){
                    visited[neighbour - 1] = true;

                    if (nodes[neighbour - 1].type == "Fault"){
                        addFaultScore(results, resultCount,
                                      neighbour, it->weight);
                    }

                    q.push(neighbour);
                }
            }
        }
    }
}

// follow only if relationship is "Related to" or "May indicate"
//we used bfs bcz it is not neccessary thatonly one fault it can be neeche also (agey bhi hoo sakte)
void findPossibleFaults(string symptomName)
{
    int symptomID = searchNode(symptomName);

    if (symptomID == -1)
    {
        cout << "Symptom not found!" << endl;
        return;
    }

    FaultResult results[MAX_Nodes];
    int resultCount = 0;

    collectFaultsBFS(symptomID, results, resultCount);//did this correction afterwards as code duplication was needed so i simply made a new function

    bubbleSort(results, resultCount);

    if (resultCount == 0)
    {
        cout << "No related faults found!" << endl;
        return;
    }

    for (int i = 0; i < resultCount; i++)
    {
        cout << i + 1 << ". "
             << nodes[results[i].faultID - 1].name
             << " - Score: " << results[i].score << endl;
    }
}

void addFaultScore(FaultResult results[], int &resultCount, int faultID, int score){ //this is to add score if multiple path lead to the same fault used in analyzeMultipleSymtoms 
    for (int i = 0; i < resultCount; i++){
        if (results[i].faultID == faultID){
            results[i].score += score;
            return;
        }
    }

    results[resultCount].faultID = faultID;
    results[resultCount].score = score;
    resultCount++;
}

void analyzeMultipleSymptoms(){
    FaultResult results[MAX_Nodes];
    int resultCount = 0;
    int selectedSymptoms[MAX_Nodes];
    int selectedCount = 0;
    int symptomCount;
    int availableSymptoms = 0;

    for (int i = 0; i < nodeCount; i++){
        if (nodes[i].type == "Symptom"){
            availableSymptoms++;
        }
    }

    cout << "\nHow many symptoms do you want to select? ";
    cin >> symptomCount;
    if (symptomCount <= 0 || symptomCount > availableSymptoms){
        cout << "Invalid number of symptoms!" << endl;
        return;
    }

    for (int i = 0; i < symptomCount; i++){
        cout << "\nSelect symptom " << i + 1 << ":\n";

        int symptomID = selectSymptom();

        if (symptomID == -1){
            cout << "Invalid selection. Try again." << endl;
            i--;
            continue;
        }

        bool duplicate = false;
        for (int j = 0; j < selectedCount; j++){
            if (selectedSymptoms[j] == symptomID){
                duplicate = true;
                break;
            }
        }

        if (duplicate){
            cout << "You have already selected this symptom!" << endl;
            i--;
            continue;
        }

        selectedSymptoms[selectedCount] = symptomID;
        selectedCount++;

        collectFaultsBFS(symptomID, results, resultCount);
    }

    bubbleSort(results, resultCount);

    if (resultCount == 0)
    {
        cout << "\nNo related faults found!" << endl;
        return;
    }

    cout << "\n========== RANKED POSSIBLE FAULTS ==========\n";

    for (int i = 0; i < resultCount; i++)
    {
        cout << i + 1 << ". "
             << nodes[results[i].faultID - 1].name
             << " - Score: " << results[i].score << endl;
    }
}

void recommendTests(int faultID){ //now after fault we need recommendation also this function will make that happen
    if (faultID < 1 || faultID > nodeCount){
        cout << "Invalid fault ID!" << endl;
        return;
    }
    cout << endl;
    cout << "Recommended Diagnostic Tests:\n";
    int count = 0;
    for (list<Edge>::iterator it = adj[faultID - 1].begin();it != adj[faultID - 1].end(); it++){
        int neighbour = it->destination;

        if (nodes[neighbour - 1].type == "Test"){
            count++;
            cout << count << ". " << nodes[neighbour - 1].name << endl;
        }
    }

    if (count == 0){
        cout << "No diagnostic tests found!" << endl;
    }
}

void recommendParts(int faultID){ //after fault surely there will be a edge telling a recommended part
    if (faultID < 1 || faultID > nodeCount){
        cout << "Invalid fault ID!" << endl;
        return;
    }

    cout << "\nRecommended Replacement Parts:\n";

    int count = 0;

    for (list<Edge>::iterator it = adj[faultID - 1].begin(); it != adj[faultID - 1].end(); it++){
        int neighbour = it->destination;

        if (nodes[neighbour - 1].type == "Part"){
            count++;

            cout << count << ". " << nodes[neighbour - 1].name << endl;
        }
    }

    if (count == 0){
        cout << "No compatible parts found!" << endl;
    }
}

void saveGraph(){
    ofstream file("graph.txt");

    if (!file){
        cout << "Error opening file!" << endl;
        return;
    }

    // Save number of nodes
    file << nodeCount << endl;

    // Save nodes
    for (int i = 0; i < nodeCount; i++){
        file << nodes[i].id << endl;
        file << nodes[i].name << endl;
        file << nodes[i].type << endl;
    }


    int edgeCount = 0; //count edges
    for (int i = 0; i < nodeCount; i++)
    {
        edgeCount += adj[i].size();
    }
    file << edgeCount << endl;

    // Save edges
    for (int i = 0; i < nodeCount; i++){
        for (list<Edge>::iterator it = adj[i].begin(); it != adj[i].end(); it++){
            file << it->source << endl;
            file << it->destination << endl;
            file << it->weight << endl;
            file << it->relation << endl;
        }
    }

    file.close();
    cout << "Graph saved successfully!" << endl;
}

void loadGraph(){
    ifstream file("graph.txt");
    if (!file){
        cout << "No saved graph found!" << endl;
        return;
    }

    // Clear existing graph
    nodeCount = 0;
    for (int i = 0; i < MAX_Nodes; i++){
        adj[i].clear();
    }

    // Read number of nodes
    int totalNodes; //first line stores number of nodes
    file >> totalNodes;
    file.ignore();

    if (totalNodes < 0 || totalNodes > MAX_Nodes){
        cout << "Invalid graph file!" << endl;
        file.close();
        return;
    }

    // Read nodes
    for (int i = 0; i < totalNodes; i++){
        file >> nodes[i].id;
        file.ignore();

        getline(file, nodes[i].name);
        getline(file, nodes[i].type);

        nodeCount++;
    }

    // Read number of edges
    int edgeCount;
    file >> edgeCount;
    file.ignore();
    if (edgeCount < 0){
        cout << "Invalid edge count!" << endl;
        file.close();
        nodeCount = 0;
        return;
    }

    // Read edges
    for (int i = 0; i < edgeCount; i++){
        Edge e;
        file >> e.source;
        file >> e.destination;
        file >> e.weight;
        file.ignore();
        getline(file, e.relation);
        if (!file || e.source < 1 || e.source > nodeCount || e.destination < 1 || e.destination > nodeCount){
            cout << "Invalid graph file!" << endl;
            file.close();
            nodeCount = 0;

            for (int j = 0; j < MAX_Nodes; j++)
                adj[j].clear();
            return;
        }

        adj[e.source - 1].push_back(e);
    }

    file.close();

    cout << "Graph loaded successfully!" << endl;
}

int selectSymptom(){
    int symptomIDs[MAX_Nodes];
    int count = 0;

    cout << endl;
    cout << "||||||||||||| SELECT SYMPTOM |||||||||||||||\n";

    // Display all available symptoms
    for (int i = 0; i < nodeCount; i++){
        if (nodes[i].type == "Symptom"){
            count++;
            symptomIDs[count - 1] = nodes[i].id;
            cout << count << ". " << nodes[i].name << endl;
        }
    }

    if (count == 0){
        cout << "No symptoms available!" << endl;
        return -1;
    }

    int choice;
    cout << "\nEnter your choice: ";
    cin >> choice;
    if (choice < 1 || choice > count)
    {
        cout << "Invalid selection!" << endl;
        return -1;
    }

    return symptomIDs[choice - 1]; //returns node id means index + 1 for nodes
}


void displayMenu(){
    cout << "\n||||||||||||||| REPAIRGRAPH |||||||||||||||\n";
    cout << "1. Display Graph\n";
    cout << "2. BFS Traversal\n";
    cout << "3. DFS Traversal\n";
    cout << "4. Find Possible Faults (Single Symptom)\n";
    cout << "5. Analyse Multiple Symptoms\n";
    cout << "6. Recommend Diagnostic Tests\n";
    cout << "7. Recommend Replacement Parts\n";
    cout << "8. Save Graph\n";
    cout << "9. Exit\n";

    cout << "\nEnter your choice: ";
}


int selectFault(){
    int faultIDs[MAX_Nodes];
    int count = 0;

    cout << "\n========== SELECT FAULT ==========\n";

    for (int i = 0; i < nodeCount; i++)
    {
        if (nodes[i].type == "Fault")
        {
            count++;

            faultIDs[count - 1] = nodes[i].id;

            cout << count << ". "
                 << nodes[i].name << endl;
        }
    }

    if (count == 0)
    {
        cout << "No faults available!" << endl;
        return -1;
    }

    int choice;

    cout << "\nEnter your choice: ";
    cin >> choice;

    if (choice < 1 || choice > count)
    {
        cout << "Invalid selection!" << endl;
        return -1;
    }

    return faultIDs[choice - 1];
}

void analyzeSingleSymptom()
{
    int symptomID = selectSymptom();

    if (symptomID == -1)
        return;

    FaultResult results[MAX_Nodes];
    int resultCount = 0;

    collectFaultsBFS(symptomID, results, resultCount);

    bubbleSort(results, resultCount);

    if (resultCount == 0)
    {
        cout << "No related faults found!" << endl;
        return;
    }

    cout << "\n|||||||||||| POSSIBLE FAULTS ||||||||||||\n";

    for (int i = 0; i < resultCount; i++)
    {
        cout << i + 1 << ". "
             << nodes[results[i].faultID - 1].name
             << " - Score: " << results[i].score << endl;
    }
}

void displayGraph(){
    cout << "\n========== REPAIRGRAPH ==========" << endl;

    for (int i = 0; i < nodeCount; i++){
        cout << nodes[i].id << ". " << nodes[i].name
             << " (" << nodes[i].type << ") -> ";

        for (list<Edge>::iterator it = adj[i].begin(); it != adj[i].end(); it++){
            cout << nodes[it->destination - 1].name
                 << " [" << it->relation << ", weight: " << it->weight << "] ";
        }

        cout << endl;
    }
}

void initializeGraph(){
    addNode(1, "Dell Inspiron 15", "Device");
    addNode(2, "Overheating", "Symptom");
    addNode(3, "Battery not charging", "Symptom");
    addNode(4, "Cooling Fan Malfunction", "Fault");
    addNode(5, "Battery Failure", "Fault");
    addNode(6, "Fan Speed Test", "Test");
    addNode(7, "Battery Health Test", "Test");
    addNode(8, "Replacement Cooling Fan", "Part");
    addNode(9, "Replacement Battery", "Part");

    addEdge(1, 2, 10, "Has symptom");
    addEdge(1, 3, 10, "Has symptom");
    addEdge(2, 4, 8, "May indicate");
    addEdge(3, 5, 9, "May indicate");
    addEdge(4, 6, 10, "Can be checked by");
    addEdge(4, 8, 10, "May require");
    addEdge(5, 7, 10, "Can be checked by");
    addEdge(5, 9, 10, "May require");
}

int main(){
    loadGraph();

    if (nodeCount == 0){
        initializeGraph();
        saveGraph();
    }

    int choice;

    do
    {
        displayMenu();
        cin >> choice;

        switch (choice)
        {
            case 1:
                displayGraph();
                break;

            case 2:
            {
                int startID;

                cout << "Enter starting node ID: ";
                cin >> startID;

                BFS(startID);
                break;
            }

            case 3:
            {
                int startID;

                cout << "Enter starting node ID: ";
                cin >> startID;

                if (startID >= 1 && startID <= nodeCount)
                {
                    bool visited[MAX_Nodes] = {false};
                    DFS(startID, visited);
                }
                else
                {
                    cout << "Invalid node ID!" << endl;
                }

                break;
            }

            case 4:
                analyzeSingleSymptom();
                break;

            case 5:
                analyzeMultipleSymptoms();
                break;

            case 6:
            {
                int faultID = selectFault();

                if (faultID != -1)
                    recommendTests(faultID);

                break;
            }

            case 7:
            {
                int faultID = selectFault();

                if (faultID != -1)
                    recommendParts(faultID);

                break;
            }

            case 8:
                saveGraph();
                break;

            case 9:
                saveGraph();
                cout << "Thank you for using RepairGraph!" << endl;
                break;

            default:
                cout << "Invalid menu choice!" << endl;
        }

    } while (choice != 9);

    return 0;
}