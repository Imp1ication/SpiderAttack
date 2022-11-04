#include <fstream>
#include <iostream>
#include "json.hpp"
#include <string>
#include <cmath>

using namespace std;
using json = nlohmann::json;


int main()
{
    srand( time(NULL) );

/* init game */
    string path_gene = "/home/implication/JavaWorkspace/SpiderAttack/bot/data.json";
    
    fstream geneFile(path_gene); 
    json jene;

    geneFile >> jene;
    geneFile.close();
    
    cout << jene << "\n\n";

    cout << jene["Atk"]["turn"] << " " << jene["Def"]["threat"]  << "\n\n";

    return 0;

}
