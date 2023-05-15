#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <cmath>
#include <stdlib.h>
using namespace std;

string RicettaCompleta(string nome)
{
    string s = "", nomeTemp, ricetta = "NoN";

    ifstream Ricetta("ricettario.csv");
    std::getline(Ricetta, s);
    while (!s.empty())
    {
        nomeTemp = s.substr(0, s.find(";")); //dividere la stringa per ottenere il nome della ricetta
        if (nomeTemp == nome)
        {
            ricetta = s;
            break;
        }
        std::getline(Ricetta, s);
    }
    return ricetta;
}



void ListaSpesa(string nome, float quantita, string tipoIngrediente)
{
    bool trovato = false;
    string s, nomeTemp = "", tipo;
    int quantitaTemp;
    ifstream ListaRead("listaspesa.csv", ios::app);
    ofstream ListaWrite("listaspesaTemp.csv", ios::app);



    std::getline(ListaRead, s);


    //controllo se trovo l'ingrediente nel file
    while (!s.empty())
    {
        nomeTemp = s.substr(0, s.find(":"));
        quantitaTemp = stoi(s.substr(s.find(":") + 1, s.find("_")));
        tipo = s.substr(s.find("_") + 1, s.length());

        if (nome == nomeTemp)
        {
            trovato = true;
            //scala quantità e lista della spesa
            quantita = quantita + quantitaTemp;
            ListaWrite << nome << ":" << quantita << "_" << tipoIngrediente << endl;
        }
        else
        {
            ListaWrite << nomeTemp << ":" << quantitaTemp << "_" << tipo << endl;
        }


        //Fai il nuovo file copia e aggiungi
        std::getline(ListaRead, s);
    }


    if (!trovato)
    {
        //aggiungi ingrediente:0 e lista della spesa
        ListaWrite << nome << ":" << quantita << "_" << tipoIngrediente << endl;
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
    ingredienti = ricetta.substr(ricetta.find(";") + 1, ricetta.length()); //ingredienti della ricetta, togli il nome e lascia solo gli ingredienti e procedimento
    ingredienti = ingredienti.substr(0, ingredienti.find(";")); //ingredienti della ricetta, togli il nome e lascia solo gli ingredienti
    return ingredienti;
}

void Magazzino()
{
    ofstream Magazzino("magazzino.csv" /*ios::app*/);

    srand(time(NULL));


    Magazzino << "farina:" << rand() % 1000 << "_g\n";
    Magazzino << "zucchero:" << rand() % 1000 << "_g\n";
    Magazzino << "uova:" << rand() % 50 << "_u\n";
    Magazzino << "latte:" << rand() % 2000 << "_ml\n";

    Magazzino.close();
}



void ricettarioSito()
{
    ofstream Magazzino("ricettarioSito.html" /*ios::app*/);

    Magazzino << "<div id = \"header\">";

    Magazzino.close();
}


void rcSitoricette(string ricetta)
{
    ofstream Magazzino("ricettarioSito.html", ios::app);

    Magazzino << ricetta << endl;

    Magazzino.close();
}


void ricettarioSitoEnd()
{
    ofstream Magazzino("ricettarioSito.html", ios::app);

    Magazzino << "</div>";

    Magazzino.close();
}



void ControlloIngredientiPresenti()
{
    string s, nomeMagazzino, nomeLista, m, tipo, tipoTemp;
    int quantitaMagazzino, quantitaLista;
    bool trovato = false;
    ifstream ListaRead("listaspesa.csv");

    ofstream ListaWrite("listaspesaTemp.csv", ios::app);

    std::getline(ListaRead, s);

    while (!s.empty())
    {
        nomeLista = s.substr(0, s.find(":")); // estrai nome
        quantitaLista = stoi(s.substr(s.find(":") + 1, s.find("_"))); // estrai quantita
        tipoTemp = s.substr(s.find("_") + 1, s.length()); // estrai tipo


        ofstream MagazzinoWrite("magazzinoTemp.csv", ios::app);
        ifstream MagazzinoRead("magazzino.csv");
        std::getline(MagazzinoRead, m);
        trovato = false;
        while (!m.empty())
        {
            nomeMagazzino = m.substr(0, m.find(":")); // estrai nome
            quantitaMagazzino = stoi(m.substr(m.find(":") + 1, m.find("_"))); //estrai quantita
            tipo = m.substr(m.find("_") + 1, m.length()); // estrai tipo

            if (nomeLista == nomeMagazzino)
            {
                trovato = true;
                if (quantitaMagazzino < quantitaLista)
                {

                    quantitaLista = quantitaLista - quantitaMagazzino;
                    quantitaMagazzino = 0;
                    //scivo nella lista
                    ListaWrite << nomeLista << ":" << quantitaLista << "_" << tipo << endl;
                }
                else
                {
                    quantitaMagazzino = quantitaMagazzino - quantitaLista;
                    //non scrivo nella lista
                }
            }
            //scrivo nel magazzino
            MagazzinoWrite << nomeMagazzino << ":" << quantitaMagazzino << "_" << tipo << endl;
            //sovrascrivo magazzino


            std::getline(MagazzinoRead, m);

        }

        if (trovato == false)
        {
            nomeMagazzino = nomeLista;
            quantitaMagazzino = 0;
            //scrivo nel magazzino
            MagazzinoWrite << nomeMagazzino << ":" << quantitaMagazzino << "_" << tipoTemp << endl;
            //riscrivo nella lista
            ListaWrite << nomeLista << ":" << quantitaLista << "_" << tipoTemp << endl;
        }

        MagazzinoRead.close();
        MagazzinoWrite.close();
        std::remove("magazzino.csv");
        std::rename("magazzinoTemp.csv", "magazzino.csv");
        std::remove("magazzinoTemp.csv");


        //sovrascrivo lista


        std::getline(ListaRead, s);
    }

    ListaRead.close();
    ListaWrite.close();

    std::remove("listaspesa.csv");
    std::rename("listaspesaTemp.csv", "listaspesa.csv");
    std::remove("listaspesaTemp.csv");


}



void ControlloMagazzino(string ingredienti)
{
    int dim = 0;
    string nomi_e_ingre[100], tipo;

    //mettiamo gli ingredienti in un array
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
        quantita = stoi(nomi_e_ingre[i].substr(nomi_e_ingre[i].find(":") + 1, nomi_e_ingre[i].find("_")));
        tipo = nomi_e_ingre[i].substr(nomi_e_ingre[i].find("_") + 1, nomi_e_ingre[i].length());


        ListaSpesa(nome, quantita, tipo); //non somma
    }

    //Controllare se nella lista della spesa c'è già l'ingrediente e se non c'è aggiungerlo dalla lista della spesa


    sr.close();


}


float QuantitaDefault(string tipo)
{
    float quantita;
    if (tipo == "g")
    {
        quantita = 500;
    }
    else if (tipo == "ml")
    {
        quantita = 1000;
    }
    else if (tipo == "u")
    {
        quantita = 12;
    }
    return quantita;
}

void Compra()
{
    string s, m, nomeLista, nomeMagazzino, tipo, tipoMagazzino;
    int quantitaLista, quantitaMagazzino;
    float quantita;
    int ricavo;
    ifstream Leggiamo("listaspesa.csv");


    std::getline(Leggiamo, s);

    while (!s.empty())
    {
        nomeLista = s.substr(0, s.find(":")); // estrai nome
        quantitaLista = stoi(s.substr(s.find(":") + 1, s.find("_"))); // estrai quantita
        tipo = s.substr(s.find("_") + 1, s.length());

        quantita = quantitaLista;

        ofstream MagazzinoWrite("magazzinoTemp.csv", ios::app);
        ifstream MagazzinoRead("magazzino.csv");

        ricavo = ceil(quantita / QuantitaDefault(tipo)) * QuantitaDefault(tipo) - quantita;
        std::getline(MagazzinoRead, m);
        while (!m.empty())
        {
            nomeMagazzino = m.substr(0, m.find(":")); // estrai nome
            quantitaMagazzino = stoi(m.substr(m.find(":") + 1, m.length())); //estrai quantita
            tipoMagazzino = m.substr(m.find("_") + 1, m.length());
            if (nomeLista == nomeMagazzino)
            {
                quantitaMagazzino = quantitaMagazzino + ricavo;
            }

            MagazzinoWrite << nomeMagazzino << ":" << quantitaMagazzino << "_" << tipoMagazzino << endl;

            std::getline(MagazzinoRead, m);
        }

        MagazzinoRead.close();
        MagazzinoWrite.close();
        std::remove("magazzino.csv");
        std::rename("magazzinoTemp.csv", "magazzino.csv");
        std::remove("magazzinoTemp.csv");


        std::getline(Leggiamo, s);
    }
}


void Aggiungi()
{
    string s, m, nomeLista, nomeMagazzino, tipo;
    int quantitaLista, quantitaMagazzino;

    ofstream MagazzinoWrite("magazzinoTemp.csv", ios::app);
    ifstream MagazzinoRead("magazzino.csv");


    std::getline(MagazzinoRead, m);
    while (!m.empty())
    {
        nomeMagazzino = m.substr(0, m.find(":")); // estrai nome
        quantitaMagazzino = stoi(m.substr(m.find(":") + 1, m.find("_"))); //estrai quantita
        tipo = m.substr(m.find("_") + 1, m.length());

        if (quantitaMagazzino == 0)
        {
            quantitaMagazzino = QuantitaDefault(tipo);
        }

        MagazzinoWrite << nomeMagazzino << ":" << quantitaMagazzino << "_" << tipo << endl;

        std::getline(MagazzinoRead, m);
    }

    MagazzinoRead.close();
    MagazzinoWrite.close();
    std::remove("magazzino.csv");
    std::rename("magazzinoTemp.csv", "magazzino.csv");
    std::remove("magazzinoTemp.csv");
}



void Ordine()
{
    /*ofstream MyFile("filename.txt");
    MyFile << "test";
    MyFile.close();*/
    string ingredienti, ricetta, s;
    string scelta = "";
    int scelta2 = -1;
    int i = 0;



    ifstream Ricette("ricettario.csv");

    ricettarioSito();
    cout << "Buongiorno, che dolce desidera ordinare?\nScrivi il nome come indicato (Case Sensitive)\n" << endl;

    std::getline(Ricette, s);
    while (!s.empty())
    {
        i++;
        cout << "- " << s.substr(0, s.find(";")) << endl;
        std::getline(Ricette, s);
    }

    cout << "\nPremi 0 per Uscire\n" << endl;
    Ricette.close();


    cin >> scelta;
    while (scelta != "0")
    {
        ricetta = RicettaCompleta(scelta);
        
        rcSitoricette(ricetta);

        if (ricetta != "NoN")
        {
            ingredienti = Ingredienti(ricetta);
            ControlloMagazzino(ingredienti);

        }
        else
        {
            cout << "Ricetta non trovata\n";
        }

        cin >> scelta;
    }

    //rinominare il file vecchio
    ControlloIngredientiPresenti();
    Compra();
    Aggiungi();
    //qui aggiungi
    ricettarioSitoEnd();
    std::remove("listaspesaVecchia.csv");
    std::rename("listaspesa.csv", "listaspesaVecchia.csv");
    std::remove("magazzinoTemp.csv");
}

void AggiungiRicetta()
{
    string nome, ingredienti, preparazione, s;
    cout << "Inserisci il nome della ricetta" << endl;
    cin >> nome;
    cout << "Inserisci gli ingredienti (attenzione alla formattazione Sintassi = ingrediente1:quantita_tipo,ingrediente2:quantita_tipo...)" << endl;
    cin >> ingredienti;
    cout << "Inserisci la preparazione (attenzione alla formattazione Sintassi = Frase 1. Frase 2. ...)" << endl;
    cin >> preparazione;
    ofstream Ricette("ricettario.csv", ios::app);
    Ricette << nome << ";" << ingredienti << ";" << preparazione << endl;
    Ricette.close();
}

void EliminaRicetta()
{
    string nome, s;
    cout << "Inserisci il nome della ricetta da eliminare" << endl;
    cin >> nome;
    ifstream Ricette("ricettario.csv");
    ofstream RicetteTemp("ricettarioTemp.csv");
    std::getline(Ricette, s);
    while (!s.empty())
    {
        if (s.substr(0, s.find(";")) != nome)
        {
            RicetteTemp << s << endl;
        }
        std::getline(Ricette, s);
    }
    Ricette.close();
    RicetteTemp.close();
    std::remove("ricettario.csv");
    std::rename("ricettarioTemp.csv", "ricettario.csv");
    std::remove("ricettarioTemp.csv");
}

void ModificaRicetta() {
    string nome, ingredienti, preparazione, s;
    cout << "Inserisci il nome della ricetta da modificare" << endl;
    cin >> nome;
    cout << "Inserisci gli ingredienti (attenzione alla formattazione Sintassi = ingrediente1:quantita_tipo,ingrediente2:quantita_tipo...)" << endl;
    cin >> ingredienti;
    cout << "Inserisci la preparazione (attenzione alla formattazione Sintassi = Frase 1. Frase 2. ...)" << endl;
    cin >> preparazione;
    ifstream Ricette("ricettario.csv");
    ofstream RicetteTemp("ricettarioTemp.csv");
    std::getline(Ricette, s);
    while (!s.empty())
    {
        if (s.substr(0, s.find(";")) != nome)
        {

            RicetteTemp << s << endl;
        }
        else
        {
            RicetteTemp << nome << ";" << ingredienti << ";" << preparazione << endl;
        }
        std::getline(Ricette, s);
    }
    Ricette.close();
    RicetteTemp.close();
    std::remove("ricettario.csv");
    std::rename("ricettarioTemp.csv", "ricettario.csv");
    std::remove("ricettarioTemp.csv");

}

void VisualizzaRicette()
{
    string s;
    ifstream Ricette("ricettario.csv");
    std::getline(Ricette, s);
    while (!s.empty())
    {
        cout << s << endl;
        std::getline(Ricette, s);
    }
    Ricette.close();
}

void CRUD()
{
    int scelta = -1;


    while (scelta != 0)
    {
        system("CLS");
        cout << "Che operazione vuoi eseguire?" << endl;
        cout << "1) Aggiungi una ricetta" << endl;
        cout << "2) Modifica una ricetta" << endl;
        cout << "3) Elimina una ricetta" << endl;
        cout << "4) Visualizza ricette" << endl;
        cout << "0) Uscita" << endl;

        cin >> scelta;
        system("CLS");

        switch (scelta)
        {
        case 1:
            AggiungiRicetta();
            break;
        case 2:
            ModificaRicetta();
            break;
        case 3:
            EliminaRicetta();
            break;
        case 4:
            VisualizzaRicette();
            break;
        case 0:
            break;
        }
    }


}

int main()
{
    int scelta;
    cout << "Benvenuto nel programma di gestione del Dolce Amore" << endl;
    cout << "Che operazione vuoi eseguire?" << endl;
    cout << "1) Ordina un dolce" << endl;
    cout << "2) CRUD del ricettario" << endl;

    cin >> scelta;
    system("CLS");

    switch (scelta)
    {
    case 1:
        Ordine();
        break;
    case 2:
        CRUD();
        break;
    }
    return 0;
}