#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <cmath>
#include <stdlib.h>
#include <Windows.h>
#include <stringapiset.h>
#include <shellapi.h>
#include <winuser.h>
#include "Tchar.h"
#include <locale>
#include <codecvt>
using namespace std;

std::wstring ExePath() // Prendo la Path
{
    TCHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
    return std::wstring(buffer).substr(0, pos);
}

void AperturaHTML() 
{
    //setup converter
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;

    //use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
    std::string converted_str = converter.to_bytes(ExePath());

    // Calcolo la lungheza della stringa
    string spath = converted_str.substr(0, converted_str.length()-10) + "\\Progetto_C++_ 1\\home.html";
    int wideLen = MultiByteToWideChar(CP_UTF8, 0, spath.c_str(), -1, NULL, 0);

    // Alloco la memoria per la stringa
    wchar_t* wideStr = new wchar_t[wideLen];

    // Converto la stringa
    MultiByteToWideChar(CP_UTF8, 0, spath.c_str(), -1, wideStr, wideLen);

    // Uso la wide string
    LPCWSTR wpath = wideStr;

    // Apri il Browser
    ShellExecuteW(NULL, L"open", wpath, NULL, NULL, SW_SHOWNORMAL);
}

float QuantitaDefault(string tipo)
{
    float quantita;
    if (tipo == "g")
    {
        return quantita = 500;
    }
    else if (tipo == "ml")
    {
        return quantita = 1000;
    }
    else if (tipo == "u")
    {
        return quantita = 12;
    }
    return -1;
}

void SpesaQuantitastandard() 
{
    string s, nomeLista, tipoTemp;
    int quantitaLista;
    ifstream ListaRead("listaspesa.csv");
    ofstream ListaWrite("listaspesaTemp.csv", ios::app);
    ofstream CucinaWrite("cucina.csv");

    std::getline(ListaRead, s);

    while (!s.empty())
    {
        nomeLista = s.substr(0, s.find(":")); // estrai nome
        quantitaLista = stoi(s.substr(s.find(":") + 1, s.find("_"))); // estrai quantita
        tipoTemp = s.substr(s.find("_") + 1, s.length()); // estrai tipo

        CucinaWrite << nomeLista << ":" << quantitaLista << "_" << tipoTemp << endl;
        quantitaLista = ceil(quantitaLista / QuantitaDefault(tipoTemp)) * QuantitaDefault(tipoTemp); // arrotonda per eccesso alla quantità standard
        ListaWrite << nomeLista << ":" << quantitaLista << "_" << tipoTemp << endl; // scrivi nel file temporaneo

        std::getline(ListaRead, s);
    }

    ListaRead.close();
    ListaWrite.close();
    CucinaWrite.close();

    std::remove("listaspesa.csv");
    std::rename("listaspesaTemp.csv", "listaspesa.csv");
    std::remove("listaspesaTemp.csv");
}

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
    
    while (!s.empty()) //controllo se trovo l'ingrediente nel file
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

void ricettarioCompletoSito()
{
    ifstream MagazzinoRead("ricettario.csv");
    ofstream Magazzino("ricettarioSito.html");
    string s;

    Magazzino << "<link rel=\"stylesheet\" href=\"css1.css\">\n<div class=\"oggetto\" id = \"header\">";
    getline(MagazzinoRead, s);

    while (!s.empty())
    {
        Magazzino << s << "<br><br>\n";
        getline(MagazzinoRead, s);
    }

    Magazzino << "</div>";
    Magazzino.close();
}

void Magazzino()
{
    std::remove("magazzino.csv");
    srand(time(NULL));
    ifstream RicettarioRead("ricettario.csv");
    int dim;
    string s, m, nomeMagazzino, ingredienti;
    bool trovato = false;

    getline(RicettarioRead, s);

    while (!s.empty())
    {
        string nomeRicettario[100];
        trovato = false;
        dim = 0;
        ingredienti = s.substr(s.find(";") +1, s.length());
        ingredienti = ingredienti.substr(0, ingredienti.find(";"));

        while (ingredienti.find(",") != string::npos) //mettiamo gli ingredienti in un array
        {
            nomeRicettario[dim] = ingredienti.substr(0, ingredienti.find(","));
            dim++;
            ingredienti = ingredienti.erase(0, ingredienti.find(",") + 1);
        }
        nomeRicettario[dim] = ingredienti;
        dim++;

        for (int i = 0; i < dim; i++) //fai un FOR per ogni ingrediente della ricetta e controlla se è presente nel magazzino
        {
            trovato = false;
            ifstream MagazzinoRead("magazzino.csv");
            ofstream MagazzinoWrite("magazzinoTemp.csv");
            getline(MagazzinoRead, m);

            while (!m.empty())
            {
                nomeMagazzino = m.substr(0, m.find(":"));
                if (nomeMagazzino == nomeRicettario[i].substr(0, nomeRicettario[i].find(":")))
                {
                    trovato = true;
                }
                MagazzinoWrite << m << "\n";
                getline(MagazzinoRead, m);
            }
            string tipo = nomeRicettario[i].substr(nomeRicettario[i].find('_')+1, nomeRicettario[i].length());

            if (!trovato)
                MagazzinoWrite << nomeRicettario[i].substr(0, nomeRicettario[i].find(':')+1) << rand() % (int)QuantitaDefault(tipo) << "_" << tipo << "\n"; //trovare il tipo di ingrediente e da li fare il random

            MagazzinoWrite.close();
            MagazzinoRead.close();

            std::remove("magazzino.csv");
            std::rename("magazzinoTemp.csv", "magazzino.csv");
            std::remove("magazzinoTemp.csv");            
        }
        getline(RicettarioRead, s);
    }    
    RicettarioRead.close();   
}

void ricettarioSito()
{
    ofstream Magazzino("ordiniSito.html");
    Magazzino << "<link rel=\"stylesheet\" href=\"css1.css\">\n<div class=\"oggetto\" id = \"header\">";
    Magazzino.close();
}

void rcSitoricette(string ricetta)
{
    string nomeIngre, tipoIngre, nomeRicetta, Procedimento, quantita, ingredienti;
    int dim = 0;
    string nomi_e_ingre[100], tipo;
    ofstream Magazzino("ordiniSito.html", ios::app);

    nomeRicetta = ricetta.substr(0, ricetta.find(";")); // estrai nome
    ricetta = ricetta.substr(ricetta.find(";") + 1, ricetta.length()); //togli il nome e lascia solo gli ingredienti e procedimento
    ingredienti = ricetta.substr(0, ricetta.find(";")); // estrai ingredienti
    Procedimento = ricetta.substr(ricetta.find(";") + 1, ricetta.length()); //togli gli ingredienti e lascia solo il procedimento
    Magazzino << "<b>" << nomeRicetta << "</b>" << "<br><br>\n";

    while (ingredienti.find(",") != string::npos) //mettiamo gli ingredienti in un array
    {
        nomi_e_ingre[dim] = ingredienti.substr(0, ingredienti.find(","));
        dim++;
        ingredienti = ingredienti.erase(0, ingredienti.find(",") + 1);
    }
    nomi_e_ingre[dim] = ingredienti;
    dim++;   

    for (int i = 0; i < dim; i++) {
        nomeIngre = nomi_e_ingre[i].substr(0, nomi_e_ingre[i].find(":")); // estrai nome
		quantita = nomi_e_ingre[i].substr(nomi_e_ingre[i].find(":") + 1, nomi_e_ingre[i].length()); //estrai quantita e tipo
		Magazzino << " - " << nomeIngre << ":" << quantita << "<br>\n";
    }

    Magazzino << Procedimento << "<br><br><br><br>\n";
    Magazzino.close();
}

void ricettarioSitoEnd()
{
    ofstream Magazzino("ordiniSito.html", ios::app);
    Magazzino << "</div>";
    Magazzino.close();
}

void magazzinoSito()
{
    ifstream MagazzinoRead("magazzino.csv");
    ofstream Magazzino("magazzinoSito.html" /*ios::app*/);
    string s;

    Magazzino << "<link rel=\"stylesheet\" href=\"css1.css\">\n<div class=\"oggetto\" id = \"header\">";

    getline(MagazzinoRead, s);

    while (!s.empty())
    {
		Magazzino << s << "<br>\n";
		getline(MagazzinoRead, s);
	}
    
    Magazzino << "</div>";
    Magazzino.close();
}

void listaSpesaSito()
{
    ifstream MagazzinoRead("listaspesa.csv");
    ofstream Magazzino("listaspesaSito.html" /*ios::app*/);
    string s;

    Magazzino << "<link rel=\"stylesheet\" href=\"css1.css\">\n<div class=\"oggetto\" id = \"header\">";

    getline(MagazzinoRead, s);

    while (!s.empty())
    {
        Magazzino << s << "<br>\n";
        getline(MagazzinoRead, s);
    }

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
                if (quantitaMagazzino < quantitaLista) //scrivo nella lista
                {

                    quantitaLista = quantitaLista - quantitaMagazzino;
                    quantitaMagazzino = 0;
                    ListaWrite << nomeLista << ":" << quantitaLista << "_" << tipo << endl;
                }
                else //non scrivo nella lista
                {
                    quantitaMagazzino = quantitaMagazzino - quantitaLista;
                }
            }        
            MagazzinoWrite << nomeMagazzino << ":" << quantitaMagazzino << "_" << tipo << endl; //scrivo nel magazzino
            std::getline(MagazzinoRead, m);
        }

        if (trovato == false)
        {
            nomeMagazzino = nomeLista;
            quantitaMagazzino = 0;
            MagazzinoWrite << nomeMagazzino << ":" << quantitaMagazzino << "_" << tipoTemp << endl; //scrivo nel magazzino
            ListaWrite << nomeLista << ":" << quantitaLista << "_" << tipoTemp << endl; //riscrivo nella lista
        }

        MagazzinoRead.close();
        MagazzinoWrite.close();
        std::remove("magazzino.csv");
        std::rename("magazzinoTemp.csv", "magazzino.csv");
        std::remove("magazzinoTemp.csv");
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
    sr.close();
}

void Compra()
{
    string s, m, p,  nomeLista, nomeMagazzino, tipo, tipoMagazzino, nomeCucina;
    int quantitaLista, quantitaMagazzino, quantitaCucina;
    float quantita;
    int ricavo;
    ifstream Leggiamo("listaspesa.csv");
    std::getline(Leggiamo, s);

    while (!s.empty())
    {
        nomeLista = s.substr(0, s.find(":")); // estrai nome
        ricavo = stoi(s.substr(s.find(":") + 1, s.find("_"))); // estrai quantita
        tipo = s.substr(s.find("_") + 1, s.length());

        //scala la quantita utilizzata
        ifstream CucinaRead("cucina.csv");
        std::getline(CucinaRead, p);

        while (!p.empty())
        {
            nomeCucina = p.substr(0, p.find(":")); // estrai nome

            if (nomeCucina == nomeLista) 
            {
                quantitaCucina = stoi(p.substr(p.find(":") + 1, p.find("_"))); // estrai quantita
                ricavo = ricavo - quantitaCucina;
            }            
            std::getline(CucinaRead, p);
        }

        CucinaRead.close();

        ofstream MagazzinoWrite("magazzinoTemp.csv", ios::app);
        ifstream MagazzinoRead("magazzino.csv");

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
    string ingredienti, ricetta, s;
    string scelta = "";
    int scelta2 = -1;
    int i = 0;

    ifstream Ricette("ricettario.csv");

    ricettarioCompletoSito();
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
        
        if (ricetta != "NoN")
        {
            ingredienti = Ingredienti(ricetta);
            ControlloMagazzino(ingredienti);
            rcSitoricette(ricetta);
        }
        else
        {
            cout << "Ricetta non trovata\n";
        }
        cin >> scelta;
    }
    
    //rinominare il file vecchio
    ControlloIngredientiPresenti();
    SpesaQuantitastandard();
    Compra();
    Aggiungi();
    
    //qui aggiungi
    ricettarioSitoEnd();

    //crei il file di lista spesa per il sito html
    listaSpesaSito();
    magazzinoSito();

    std::remove("listaspesaVecchia.csv");
    std::rename("listaspesa.csv", "listaspesaVecchia.csv");
    std::remove("magazzinoTemp.csv");

    AperturaHTML();
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
        ricettarioCompletoSito();
    }
}

int main()
{
    ofstream listaSpesaSito("listaspesaSito.html");
    listaSpesaSito.close();
    
    Magazzino();
    magazzinoSito();
    ricettarioSito();

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