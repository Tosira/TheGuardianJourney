#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <unordered_map>
using namespace std; 


struct Guard {
    string name; 
    int powerLevel; 
    string mainMaster; 
    string village; 
    int op;     
    vector<Guard*> subGuardians;     
}; 

struct Village {    
    int score;    
    string name;
    string connectedVillage; 
    vector<Guard*> VillageGuardians; 
    vector<Village*> connectedVillages;                  
};

class Graph {
    private: 
        int numVertices; 
        vector<vector<int>> adjMatrix; 
    
    public: 
        Graph(int numVertex) : numVertices(numVertex) {
            adjMatrix.resize(numVertices, vector<int>(numVertices, 0)); 
        }

        void addEdge(int src, int dest) {
            if (src >= 0 && src < numVertices && dest >= 0 && dest < numVertices) adjMatrix[src][dest] = 1; 
        }
}; 

//Retorna guardia a partir de las varibles leidaas desde el archivo
Guard* createGuard(string name, string powerLevel, string mainMaster, string village, int op = 0)
{
    Guard* newGuard = new Guard; 
    newGuard->name = name;      
    newGuard->powerLevel = stoi(powerLevel); 
    newGuard->mainMaster = mainMaster; 
    newGuard->village = village; 
    newGuard->op = op;     
    return newGuard; 
}

//Retorna Village a partir de las varibles leidas desde el archivo
Village* createVillage(string name, string connectedVillage)
{
    Village* newVillage = new Village; 
    newVillage->name = name; 
    newVillage->score = 0;      
    newVillage->connectedVillage = connectedVillage;
    return newVillage; 
}

string convertToString(int intNum)
{
    stringstream ss;
    ss << intNum; 
    string stringNum = ss.str(); 
    return stringNum; 
}

//Imprime lista de guardias
void printListGuard(const vector<Guard*> &vecGuardians)
{
    cout << "Name, PowerLevel, MainMaster, Village, Option" <<endl; 
    for (const auto& guard : vecGuardians) 
        cout << guard->name << ", " << guard->powerLevel << ", " << guard->mainMaster << ", " << guard->village << ", " << guard->op << endl;    
} 

//Imprime lista de aldeas
void printListVillage(const vector<Village*> &vecVillages)
{
    for (const auto& v : vecVillages) cout << v->name << ", " << v->connectedVillage << endl; 
}

//Imprime todos los guardias de la lista vectorGuardians en un arbol con jerarquia
void printGuardianTree(Guard* masterOfTree) 
{
    if (masterOfTree == nullptr) return;     

    if (!masterOfTree->subGuardians.empty()){
        cout << masterOfTree->name << " -> "; 
        for (const auto& subGuard : masterOfTree->subGuardians) 
            cout << subGuard->name << " "; 
        cout << endl;
    }
        
    for (const auto& subGuard : masterOfTree->subGuardians) 
        printGuardianTree(subGuard);     
}

//Imprime los guradias separandolos por sus respectivas aldeas
void printVillageGuards(unordered_map<int, Village*> mapIntVillage)
{
    for (const auto& pair : mapIntVillage){
        cout << "Guaridas de aldea " << pair.second->name << endl; 
        for (const auto& guard : pair.second->VillageGuardians)
            cout << guard->name << ", " << guard->powerLevel << ", " << guard->village << endl; 
        cout << endl; 
    }
}

//Retorna true si "vecGuardians" contiene "guard", de lo contrario retorna false.  
bool containsGuard(const vector<Guard*> vecGuardians, const Guard* guard)
{
    for (const auto& g : vecGuardians)
        if(g == guard) return true; 

    return false;
}

//Retorna true si "vecVillages" contiene "village", de lo contrario retorna false.  
bool containsVillage(const vector<Village*> vecVillages, const Village* village) 
{    
    for (const auto& v : vecVillages) 
        if (v->name == village->name) return true;  //comparacion hecha en base al nombre     

    return false; 
}

//Retorna true si "nameVillages" contiene "name", de lo contrario retorna false
bool containsNameVillage(const vector<string> nameVillages, string name)
{
    for (const auto& n : nameVillages)
        if (n == name) return true; 

    return false; 
}

//Funcion encargada de organizar el vectorVillages, retornando un vector con todas las aldeas y sus adyacentes conectadas. 
vector<Village*> organizeVectorVillage(const vector<Village*> &vecVillages)
{    
    vector<Village*> organizedVillages;
    vector<string> nameVillage; 
    vector<string> nameConnectedVillages; 
    //El llenado de organizedVillages es con aldeas de nombres distintos porque estos representan a la unica aldea. 
    for (const auto& village : vecVillages){       
        if (!containsVillage(organizedVillages, village)) organizedVillages.push_back(village);
        nameVillage.push_back(village->name); 
    }
    
    for (const auto& village : vecVillages){
        if (!containsNameVillage(nameVillage, village->connectedVillage)){
            organizedVillages.push_back(createVillage(village->connectedVillage, ""));   //Grafo dirigido
            nameConnectedVillages.push_back(village->connectedVillage);
        }  
    }
    
    for (const auto& orVillage : organizedVillages)
    {
        for (const auto& village : vecVillages)
        {                        
            if (village->connectedVillage == orVillage->name){                
                for (const auto& searchOrVillage : organizedVillages){
                    if (searchOrVillage->name == village->name && containsNameVillage(nameConnectedVillages, village->connectedVillage)){                        
                        orVillage->connectedVillages.push_back(village); 
                        searchOrVillage->connectedVillages.push_back(orVillage);
                        break; 
                    } else if (searchOrVillage->name == village->name){
                        searchOrVillage->connectedVillages.push_back(orVillage); 
                        break; 
                    }

                }                
            }
        }
    }
    return organizedVillages; 
}

//Funcion en formTree. Retorna true si el nodo es ubicado en el orden jerarquico, de lo contrario retorna false.  
bool insertNodeInTree(Guard* guardToInsert, Guard* currentGuard)
{       
    if (currentGuard == nullptr) return false;     

    if (currentGuard->name == guardToInsert->mainMaster)
    {
        currentGuard->subGuardians.push_back(guardToInsert); 
        return true; 
    }

    if (!currentGuard->subGuardians.empty()){
        for (const auto& guard : currentGuard->subGuardians)        
            if (insertNodeInTree(guardToInsert, guard)) return true; 
    }
            
    return false; 
}

//Recorre el vector de guardias y los envia a insertNode para formar el arbol. 
void createTree(const vector<Guard*> &vecGuardians, Guard* masterOfTree) 
{   
    vector<Guard*> auxVecGuardians;         
    //Los bucles anidados buscan el descendiente correspondiente a los nodos ya ingresados. Por eso, insertNode retornara false cuando el guardia no encuentre a 
    //su maestro en los nodos que ya formen parte del arbol, asi se avanzara hasta que toque el guardia que encuentre su posicion en la jerarquia. Esto permite 
    //que el archivo Guardians.txt se ingrese en cualquier orden.       
    //se suma 1 a longitud de auxVecGuardians porque no contendra a masterOfTree(Stormheart, en este caso), ya que "insertNode" no retornara true para agregarlo.
    while ((auxVecGuardians.size()+1) != vecGuardians.size())   
    {         
        for (const auto& guard : vecGuardians){            
            if (!containsGuard(auxVecGuardians, guard))
                if (insertNodeInTree(guard, masterOfTree)) auxVecGuardians.push_back(guard);
        }        
    }    
}

//Funcion encargada de guardar en map las aldeas conectadas segun el archivo leido. 
void createGraph(const vector<Village*> &vecVillages, unordered_map<int, Village*> &mapIntVillage, Graph graph)
{    
    int cont = 0; 
    for (const auto& village : vecVillages){
        mapIntVillage[cont] = village; 
        cont++; 
    }
}

//Funcion que establece o relaciona a los guardias con sus respectivas aldeas a traves de VillageGuardians(vector de Village). 
void establishVillageGuards(unordered_map<int, Village*> &mapIntVillage, vector<Guard*> vecGuardians)
{
    for (const auto& pair : mapIntVillage){        
        for (const auto& guard : vecGuardians){        
            if (pair.second->name == guard->village) 
                pair.second->VillageGuardians.push_back(createGuard(guard->name, convertToString(guard->powerLevel), guard->mainMaster, guard->village));         
        }
    }
}

//Funcion que define el maestro de cada aldea comparando el poder de cada uno, quedando como maestro el mas fuerte.  
Guard* defineMainMaster(unordered_map<int, Village*> &mapIntVillage, string nameVillage)
{
    Guard* tempGuard = nullptr; 
    for (const auto& pair : mapIntVillage){
        if (nameVillage == pair.second->name){
            for (const auto& guard : pair.second->VillageGuardians){                 
                if (tempGuard == nullptr) tempGuard = guard; 

                if (tempGuard->powerLevel < guard->powerLevel) tempGuard = guard;                                 
            }              
            break; 
        }        
    }
    return tempGuard;
}

//Funcion que crea el arbol de guardias para cada aldea. 
void createGuardTreeForVillage(unordered_map<int, Village*> &mapIntVillage)
{   
    for (const auto& pair : mapIntVillage){
        //createTree(pair.second->VillageGuardians, defineMainMaster(mapIntVillage, pair.second->name));
    }
}

void createApprendice(unordered_map<int, Village*> mapIntVillage)
{
    string name; 
    int op = 0; 
    cout << "Ingrese nombre del guardia: ";
    cin >> name; 

    for (const auto& pair : mapIntVillage) cout << pair.second->name << "  opcion: " << pair.first << endl;  
    cout << "Seleccione la aldea a la que pertencera el guardia: ";
    cin >> op; 
}

int main ()
{
    vector<Guard*> vectorGuardians; 
    vector<Village*> vectorVillages;      
    unordered_map<int, Village*> mapIntVillage;     
    Guard* masterOfTree = nullptr;

    #pragma region        

        string filename = "Guardians.txt";
        ifstream file(filename); 
        if (file.is_open())
        {
            string line;                                   
            int op = 0; 

            while (getline(file, line))
            {
                string name, powerLevel, mainMaster, village;                 
                stringstream ss(line);                    
                getline(ss, name, ',');
                getline(ss, powerLevel, ',');               
                getline(ss, mainMaster, ',');               
                getline(ss, village, ',');                                      

                if (mainMaster == "")
                {                    
                    masterOfTree = createGuard(name, powerLevel, mainMaster, village, op); 
                    vectorGuardians.push_back(masterOfTree);
                    op++; 
                    continue; 
                } 
                vectorGuardians.push_back(createGuard(name, powerLevel, mainMaster, village, op));  //directo entregado a funcion formTree()                 
                op++; 
            }        
            
        }
        else 
        {
            cout << "Failed to open file: " << filename << endl; 
            return 1;  
        }
                
        string filename2 = "Villages.txt"; 
        ifstream file2(filename2);   
        if (file2.is_open())
        {
            string line;                                               
            int op = 0; 
            while (getline(file2, line))
            {                
                string name, connectedVillage;                  
                stringstream ss(line);                                    
                getline(ss, name, ','); 
                getline(ss, connectedVillage, ',');                  
                
                vectorVillages.push_back(createVillage(name,connectedVillage));
                op++;             
            }
        }
        else 
        {
            cout << "Failed to open file: " << filename << endl; 
            return 1;  
        }

    #pragma endregion

    createTree(vectorGuardians, masterOfTree); 
    vector<Village*> organizedVillages = organizeVectorVillage(vectorVillages); 
    Graph graph(organizedVillages.size());
    createGraph(organizedVillages, mapIntVillage, graph);
    establishVillageGuards(mapIntVillage, vectorGuardians); 

    int op = 1; 
    while(op!=0)
    {
        cout << "Ingrese <1> para imprimir arbol de los guradias, <2> para imprimir la lista de aldeas, o <3> para imprimir los guardias con sus respectivas aldeas: "; 
        cin >> op; 
        switch (op)
        {
        case 1: 
            printGuardianTree(masterOfTree);
            break;
        
        case 2: 
            printListVillage(organizedVillages); 
            break; 

        case 3: 
            printVillageGuards(mapIntVillage); 
            break; 
        
        default:
            break;
        }
        cout << "Ingrese <0> para salir del menu o cualquier otro numero para continuar: "; 
        cin >> op;         
        cout << endl; 
    }

    return 0; 
}