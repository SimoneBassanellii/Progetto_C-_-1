#include <iostream>
#include <fstream> 
#include <string>
#include <list>
#include <cmath>
using namespace std;

string RicettaCompleta(string nome)
{
    string s = "", nomeTemp, ricetta;

    ifstream Ricetta("ricettario.csv");
    std::getline(Ricetta, s);
    while (!s.empty()) 
    {
        nomeTemp=s.substr(0, s.find(";")); //dividere la stringa per ottenere il nome della ricetta 
        if (nomeTemp == nome) 
        {
            ricetta = s;
            break;
        }
        std::getline(Ricetta, s);
    }
    return ricetta;
}



void ListaSpesa(string nome, float quantita)
{
    bool trovato=false;
    string s, nomeTemp="";
    int quantitaTemp;
    ifstream ListaRead("listaspesa.csv", ios::app);
    ofstream ListaWrite("listaspesaTemp.csv", ios::app);


    //ListaWrite << ingrediente << ":" << quantità;
    //cout << ceil(quantità / 500);

    //int ricavo = ceil(quantità / 500) * 500 - quantità;



    std::getline(ListaRead, s);


    //controllo se trovo l'ingrediente nel file
    while (!s.empty())
    {
		nomeTemp = s.substr(0, s.find(":"));
		quantitaTemp = stoi(s.substr(s.find(":") + 1, s.length()));
        if (nome == nomeTemp)
        {
			trovato = true;
			//scala quantità e lista della spesa
			quantita = quantita + quantitaTemp;
			ListaWrite << nome << ":" << quantita << endl;
		}
        else
        {
            ListaWrite << nomeTemp << ":" << quantitaTemp << endl;
        }

        
		//Fai il nuovo file copia e aggiungi
		std::getline(ListaRead, s);
	}
    /* {
        nomeTemp = s.substr(0, s.find(";"));
        quantitaTemp = stoi(s.substr(s.find(":") + 1, s.length()));

        if (nome == nomeTemp)
        {
            trovato = true;
            //scala quantità e lista della spesa
            quantita = quantita + quantitaTemp;

            ListaWrite << nome << ":" << quantita << endl;

        }

        //Fai il nuovo file copia e aggiungi
        std::getline(ListaRead, s);
    }*/

    if (!trovato)
    {
        //aggiungi ingrediente:0 e lista della spesa
        ListaWrite << nome << ":" << quantita << endl;
    }

    ListaRead.close();
    ListaWrite.close();

    std::remove("listaspesa.csv");
    std::rename("listaspesaTemp.csv", "listaspesa.csv");
    std::remove("listaspesaTemp.csv");

    
    
}


string Ingredienti(string ricetta) 
{
    string ingredienti;
    ingredienti = ricetta.substr(ricetta.find(";")+1, ricetta.length()); //ingredienti della ricetta, togli il nome e lascia solo gli ingredienti e procedimento
    ingredienti = ingredienti.substr(0, ingredienti.find(";")); //ingredienti della ricetta, togli il nome e lascia solo gli ingredienti
    return ingredienti;
}

void Magazzino() 
{
    ofstream Magazzino("magazzino.csv" /*ios::app*/);

    Magazzino << "farina:90\n";
    Magazzino << "zucchero:100\n";
}








void ControlloMagazzino(string ingredienti)
{
    int dim = 0;
    string nomi_e_ingre[100];
    while (ingredienti.find(",") != string::npos)
    {
        nomi_e_ingre[dim] = ingredienti.substr(0, ingredienti.find(","));
        dim++;
        ingredienti = ingredienti.erase(0, ingredienti.find(",") + 1);
        //cout << "\ningredienti rimanenti: " << ingredienti;
    }
    nomi_e_ingre[dim] = ingredienti;
    dim++;

    string nome, nomeTemp, s;
    int quantita, quantitaTemp;
    bool trovato = false;
    ifstream sr("magazzino.csv");

    for (int i = 0; i < dim; i++)
    {
        nome = nomi_e_ingre[i].substr(0, nomi_e_ingre[i].find(":"));
        quantita = stoi(nomi_e_ingre[i].substr(nomi_e_ingre[i].find(":") + 1, nomi_e_ingre[i].length()));


        ListaSpesa(nome, quantita); //non somma
    }

    sr.close();

    
}





int main()
{
    /*ofstream MyFile("filename.txt");
    MyFile << "test";
    MyFile.close();*/
    string ingredienti;
    int scelta=-1;
    Magazzino();
    cout << "Buongiorno, che dolce desidera ordinare?\n";
    cout << "1 - Sacher\n 2 - Creme Brulee\n 3 - Crostata\n";
    
    while (scelta != 0) 
    {
        cin >> scelta;
        switch (scelta)
        {
        case 1:
            ingredienti = Ingredienti(RicettaCompleta("sacher"));
            ControlloMagazzino(ingredienti);
            break;
        case 2:
            ingredienti = Ingredienti(RicettaCompleta("creme brulee"));
            ControlloMagazzino(ingredienti);
            break;
        case 3:
            ingredienti = Ingredienti(RicettaCompleta("crostata"));
            ControlloMagazzino(ingredienti);
            break;
        case 4:

            break;
        }
    }
    
    //rinominare il file vecchio
    std::remove("listaspesaVecchia.csv");
    std::rename("listaspesa.csv", "listaspesaVecchia.csv");
    std::remove("magazzinoTemp.csv");
}

