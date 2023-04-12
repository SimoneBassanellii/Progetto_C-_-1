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
    getline(Ricetta, s);
    while (!s.empty()) 
    {
        nomeTemp=s.substr(0, s.find(";")); //dividere la stringa per ottenere il nome della ricetta 
        if (nomeTemp == nome) 
        {
            ricetta = s;
            break;
        }
        getline(Ricetta, s);
    }
    return ricetta;
}

int ListaSpesa(string ingrediente, float quantità)
{
    fstream Lista("listaspesa.csv", ios::app);

    Lista << ingrediente << ":" << quantità;
    //cout << ceil(quantità / 500);

    int ricavo = ceil(quantità / 500) * 500 - quantità;
    Lista.close();
    return ricavo;
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





/*string* ControlloIngredienti(string ingredienti)
{
    int dim = 1;
    string* nomi_e_ingre = new string[100];

    while (ingredienti.find(",") != string::npos)
    {
        nomi_e_ingre[dim] = ingredienti.substr(0, ingredienti.find(","));
        dim++;
        ingredienti = ingredienti.erase(0, ingredienti.find(",") + 1);
        //cout << "\ningredienti rimanenti: " << ingredienti;
    }
    nomi_e_ingre[dim] = ingredienti;
    dim++;
    nomi_e_ingre[0] = dim;
    //prima pos array è per la lunghezza
    return nomi_e_ingre;
}*/



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
    ofstream sw("magazzinoTemp.csv", ios::app);

    for (int i = 0; i < dim; i++)
    {
        nome = nomi_e_ingre[i].substr(0, nomi_e_ingre[i].find(":"));
        quantita = stoi(nomi_e_ingre[i].substr(nomi_e_ingre[i].find(":") + 1, nomi_e_ingre[i].length()));


        getline(sr, s);


        //controllo se trovo l'ingrediente nel file
        while (!nomeTemp.empty())
        {
            nomeTemp = s.substr(0, s.find(";"));
            quantitaTemp = stoi(s.substr(s.find(":") + 1, s.length()));

            if (nome == nomeTemp)
            {
                trovato = true;
                //scala quantità e lista della spesa
                quantitaTemp = quantitaTemp - quantita;

                if (quantitaTemp < 0)
                {
                    quantitaTemp = ListaSpesa(nome, quantitaTemp) + quantitaTemp;
                }

                sw << nome << ":" << quantitaTemp << endl;

            }

            //Fai il nuovo file copia e aggiungi
            getline(sr, s);
        }


        if (!trovato)
        {
            //aggiungi ingrediente:0 e lista della spesa
            sw << nome << ":" << ListaSpesa(nome, quantita) << endl;
        }

    }

    sr.close();
    sw.close();

    remove("magazzino.csv");

    if (rename("magazzinoTemp.csv", "magazzino.csv") != 0)
        perror("Error renaming file");
    else
        cout << "File renamed successfully";
}


/*
void ControlloMagazzinoNO(string ingredienti)
{
    string* nandi= new string[100];

    string nomi_e_ingre[100] = c_str(sandi);

    nomi_e_ingre = ControlloIngredienti(ingredienti);

    int dim = stoi(nomi_e_ingre[0]);

    //abbiamo gli ingredienti salvati su um array

    string nome, nomeTemp, s;
    int quantita, quantitaTemp;
    bool trovato = false;
    ifstream sr("magazzino.csv");
    ofstream sw("magazzinoTemp.csv", ios::app);

    //salto pos 0 perche è dove è memorizzatála dimensione
    for (int i = 1; i < dim; i++)
    {
        nome = nomi_e_ingre[i].substr(0, nomi_e_ingre[i].find(":"));
        quantita = stoi(nomi_e_ingre[i].substr(nomi_e_ingre[i].find(":") + 1, nomi_e_ingre[i].length()));


        getline(sr, s);


        //controllo se trovo l'ingrediente nel file
        while (!nomeTemp.empty())
        {
            nomeTemp = s.substr(0, s.find(";"));
            quantitaTemp = stoi(s.substr(s.find(":") + 1, s.length()));

            if (nome == nomeTemp)
            {
                trovato = true;
                //scala quantità e lista della spesa
                quantitaTemp = quantitaTemp - quantita;

                if (quantitaTemp < 0)
                {
                    quantitaTemp = ListaSpesa(nome, quantitaTemp) + quantitaTemp;
                }

                sw << nome << ":" << quantitaTemp << endl;

            }

            //Fai il nuovo file copia e aggiungi
            getline(sr, s);
        }


        if (!trovato)
        {
            //aggiungi ingrediente:0 e lista della spesa
            sw << nome << ":" << ListaSpesa(nome, quantita) << endl;
        }

    }

    sr.close();
    sw.close();

    remove("magazzino.csv");

    if (rename("magazzinoTemp.csv", "magazzino.csv") != 0)
        perror("Error renaming file");
    else
        cout << "File renamed successfully";
    delete[](nomi_e_ingre);
}*/


int main()
{
    /*ofstream MyFile("filename.txt");
    MyFile << "test";
    MyFile.close();*/
    string ingredienti;
    int scelta;
    Magazzino();
    cout << "Buongiorno, che dolce desidera ordinare?\n";
    cout << "1 - Sacher\n 2 - Creme Brulee";
    cin >> scelta;
    
    switch (scelta)
    {
    case 1:
        ingredienti = Ingredienti(RicettaCompleta("sacher"));
        ControlloMagazzino(ingredienti);
        break;
    case 2:

        break;
    case 3:

        break;
    case 4:

        break;
    }


    
    std::remove("listaspesa.csv");
    std::remove("magazzinoTemp.csv");
}

