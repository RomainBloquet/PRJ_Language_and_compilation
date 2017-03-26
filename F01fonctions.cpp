#include <iostream>
#include <fstream>
#include "F01header.h"
#include <windows.h>
#include <vector>

void Menu()
{
    string numFichier;
    string nomFichier;
    string nomNonRec;

    system("mode con COLS=100");
    cout << "\t\t\t\t\tAnalyseur predictif\n" << endl;

    cout<<"Saisissez le numero du fichier contenant la grammaire a utiliser (un chiffre de 1 a 9)."<<endl;
    cin>>numFichier;

    nomFichier = "F01grammaire"+numFichier+".txt";
    vector<char> terminauxNR;
    terminauxNR = Lecture(nomFichier);

    LectureNonRec("F01nonRecur.txt", terminauxNR);
}

vector<char> Lecture(string nomFichier)
{
    string ligne;
    vector<vector<string> > grammaire;
    vector<char> nonTerminaux;
    vector<char> terminaux;
    bool nonTerm;
    int cpt=0;
    int cpt2=0;
    bool stop=false;

    ifstream lecture(nomFichier.c_str());
    if(lecture)
    {
        cout<<"\nSelection du fichier "<<nomFichier<<".";

        while(getline(lecture, ligne)) //Premiere lecture pour stocker les nonTerminaux et la première colonne du tableau de la grammaire
        {
            string tempString;
            tempString += ligne[0];
            grammaire.push_back(vector<string>(1,tempString));
            nonTerminaux.push_back(ligne[0]);
        }
        lecture.close();

        ifstream lecture(nomFichier.c_str());
        while(getline(lecture, ligne)) //Deuxieme lecture pour stocker les terminaux et remplir le reste du tableau de la grammaire
        {
            string tempString;

            int i=2;
            for(i; i<ligne.size(); i++) //On parcourt les caracteres de la ligne
            {
                if(ligne[i]!='|') //Si le caractere actuel nest pas un "ou", on ajoute le caractere à un string
                {
                    tempString+=ligne[i];
                }
                if((ligne[i+1]=='|') || ((i+1)==ligne.size())) //Si le caractere suivant est un "ou" ou si on est à un caractere de la fin de ligne, on ajoute le string dans le tableau de la grammaire
                {
                    grammaire[cpt].push_back(tempString);
                    tempString.clear();
                }
                nonTerm=0;
                for(int j=0; j<nonTerminaux.size(); j++) //On parcourt la liste des états non terminaux
                {
                    if((ligne[i]==nonTerminaux[j])||(ligne[i]=='|')||(ligne[i]=='#')) //Si le caractere actuel est un état non terminal, un "ou", ou le mot vide, ce n'est pas un terminale
                    {
                        nonTerm=1;
                    }
                }
                if(nonTerm==0)
                {
                    if(cpt2==0)
                    {
                        terminaux.push_back(ligne[i]);
                        cpt2++;
                    }
                    else
                    {
                        for(int k=0; k<terminaux.size(); k++)
                        {
                            if(terminaux[k]==ligne[i])
                            {
                                stop=true;
                            }
                        }
                        if (stop == false)
                        {
                            terminaux.push_back(ligne[i]);
                        }
                    }
                    stop = false;
                }
            }
            cpt++;
        }
        lecture.close();

        for(int i=0; i<grammaire.size(); i++) //On affiche la grammaire
        {
            cout<<endl;
            for(int j=0; j<grammaire[i].size(); j++)
            {
                if (j == 0)
                {
                    cout<<endl << grammaire[i][j];
                }
                if (j == 1)
                {
                    cout<<" => "<<grammaire[i][j];
                }
                if (j > 1)
                {
                    cout<<endl<<"     "<<grammaire[i][j];
                }
            }
        }
    }
    else
    {
        system("cls");
        cerr<<"Impossible d'ouvrir le fichier !"<<endl;
        Menu();
    }
    return ElimRecur(grammaire,terminaux,nonTerminaux);
}

vector<char> ElimRecur(vector<vector<string> > grammaire, vector<char> terminaux, vector<char> nonTerminaux)
{
    vector<vector<string> > copie = grammaire;
    ofstream ecriture("F01nonRecur.txt");
    bool recursif ;
    bool stop = false;
    int cpt = 0;

    if(ecriture)
    {
        for(int i = 0; i< grammaire.size(); i++)
        {
            recursif = false;
            for(int j = 1; j < grammaire[i].size(); j++) // Test s'il y a récursivité sur la ligne
            {
                if(grammaire[i][0][0] == grammaire[i][j][0])
                {
                    recursif = true;
                }
            }
            if(recursif == false) // S'il n'y a pas de recursivite sur la ligne, on la recopie telle quelle
            {
                ecriture << grammaire[i][0] << "=";
                for(int j = 1; j < grammaire[i].size(); j++)
                {
                    if (j!=grammaire[i].size()-1)
                    {
                        ecriture << grammaire[i][j]<< "|";
                    }
                    else
                    {
                        ecriture << grammaire[i][j];
                    }

                }
                cpt ++;
            }
            else // Si il y a recursivité sur la ligne
            {
                copie = grammaire;

                for (int k = 1; k < grammaire[i].size(); k++)// On regarde dans les cases de la ligne
                {
                    if(copie[i][0][0] != copie[i][k][0] && !copie[i][k].empty()) // Si le premier Char de la case est différent de celui de celle de comparaison alors
                    {
                        if (stop == false && !copie[i][k].empty() && copie[i][k][0]!='#') // On ecrit la grammaire dans le fichier
                        {
                            ecriture << grammaire[i][0] << "=" << copie[i][k][0] << grammaire[i][0] << "`";
                            copie[i][k].erase();
                            stop = true;
                        }
                        if (stop == false && !copie[i][k].empty() && copie[i][k][0]=='#' ) // On ecrit la grammaire dans le fichier
                        {
                            ecriture << grammaire[i][0] << "=" << grammaire[i][0] << "`";
                            copie[i][k].erase();
                            stop = true;
                        }
                        if (stop == true && !copie[i][k].empty() && copie[i][k][0]!='#')
                        {
                            ecriture << "|" << copie[i][k][0] << grammaire[i][0] << "`";
                            copie[i][k].erase();
                        }
                        if (stop == true && !copie[i][k].empty()  && copie[i][k][0]=='#' )
                        {
                            ecriture << "|" << grammaire[i][0] << "`";
                            copie[i][k].erase();
                        }
                        if (stop == true && !copie[i][k].empty() && k== grammaire[i].size()-1 && copie[i][k][0]!='#')
                        {
                            ecriture << "|" << copie[i][k][0] << grammaire[i][0] << "`"<<endl;
                            copie[i][k].erase();
                        }
                        if (stop == true && !copie[i][k].empty() && k== grammaire[i].size()-1 && copie[i][k][0]=='#' )
                        {
                            ecriture << "|" << grammaire[i][0] << "`"<<endl;
                            copie[i][k].erase();
                        }

                    }

                }
                int l = 1;
                while (l < grammaire[i].size())// On regarde dans les cases de la ligne
                {
                    if(copie[i][0][0] == copie[i][l][0] && !copie[i][l].empty()  ) // Si le premier Char de la case est différent de celui de celle de comparaison alors
                    {
                        string a;
                        if (l ==1)
                        {
                            ecriture <<endl << copie[i][0][0]<< "`=";
                        }
                        if ( l != grammaire[i].size() -1  )
                        {
                            a.empty();
                            for (int m = 1; m < grammaire[i][l].size(); m++)
                            {
                                a+=grammaire[i][l][m];
                            }
                            ecriture << a<< copie[i][0][0] << "`|"  ;
                        }
                    }
                    l++;
                }
                ecriture << '#';
            }
            stop = false;
            ecriture << endl;
        }

    }
    else
    {
        system("cls");
        cerr<<"Impossible d'ouvrir le fichier !"<<endl;
        Menu();
    }
    return terminaux;
}

void LectureNonRec(string nomNonRecur, vector<char> terminauxNR)
{
    string ligneNR;
    vector<vector<string> > grammaireNR;
    vector<string> nonTerminauxNR;
    bool nonTerm;
    int cpt=0;

    cout<<"\n\nElimination de la recursivite a gauche.";

    ifstream lecture(nomNonRecur.c_str());

    if(lecture)
    {
        while(getline(lecture, ligneNR)) //Premiere lecture pour stocker les nonTerminaux et les lignes de la matrice
        {
            string tempString;
            tempString += ligneNR[0];
            if (ligneNR[1]!= '=')
            {
                tempString += ligneNR[1];
            }
            grammaireNR.push_back(vector<string>(1,tempString));
            nonTerminauxNR.push_back(tempString);
        }
        lecture.close();

        ifstream lecture(nomNonRecur.c_str());
        while(getline(lecture, ligneNR)) //Deuxieme lecture pour stocker les terminaux et creer les colonnes de la matrice
        {
            char tempChar;
            string tempString;
            int i=2;

            for(i; i<ligneNR.size(); i++)
            {
                tempChar = ligneNR[i];
                if((tempChar!='|')&&(tempChar!='='))
                {
                    tempString+=tempChar;
                }
                if(((ligneNR[i+1])=='|')||((i+1)==ligneNR.size()))
                {
                    grammaireNR[cpt].push_back(tempString);
                    tempString.clear();
                }
            }
            cpt++;
        }
        lecture.close();

        for(int i=0; i<grammaireNR.size(); i++)
        {
            cout<<endl;
            for(int j=0; j<grammaireNR[i].size(); j++)
            {
                if (j == 0)
                {
                    cout<<endl << grammaireNR[i][j];
                }
                if (j == 1)
                {
                    cout<<" => "<<grammaireNR[i][j];
                }
                if (j > 1)
                {
                    cout<<endl<<"      "<<grammaireNR[i][j];
                }
            }
        }
    }
    else
    {
        system("cls");
        cerr<<"Impossible d'ouvrir le fichier !"<<endl;
        Menu();
    }
    Premier(grammaireNR,terminauxNR,nonTerminauxNR);
}

void Premier (vector<vector<string> > grammaireNR, vector<char> terminauxNR, vector<string> nonTerminauxNR)
{
    cout<<endl;
    vector<vector<string> > premier;
    vector<vector<string> > CopieGrammaire=grammaireNR;
    string terminal;
    bool doublon;
    int indiceChar;
    bool continuer;

    cout<<"\nPremiers";

    for(int i=0; i<grammaireNR.size(); i++)
    {
        premier.push_back(vector<string>(1,grammaireNR[i][0]));
    }

    for(int x=0; x<grammaireNR.size(); x++)
    {
        for(int i=0; i<grammaireNR.size(); i++) // Parcours de la grammaire
        {
            for(int j=1; j<grammaireNR[i].size(); j++)
            {
                indiceChar=0;
                continuer=true;
                if(grammaireNR[i][j][0]=='#') // Si c'est un #, on le met dans les premiers
                {
                    doublon = false;
                    for (int t=1; t < premier[i].size(); t++)
                    {
                        if (premier[i][t]=="#")
                        {
                            doublon = true;
                        }
                    }
                    if (doublon == false)
                    {
                        premier[i].push_back("#");
                    }
                }

                while(indiceChar<grammaireNR[i][j].size() && continuer==true) // On parcours chaque caractère de la case jusqu'au premier caractère non nullable
                {
                    continuer=false;
                    for(int k=0; k<terminauxNR.size(); k++) // Parcours des terminaux
                    {
                        if(grammaireNR[i][j][indiceChar]==terminauxNR[k]) // Si le char actuel de la case de la grammaire est un terminal, on le met dans les premiers
                        {
                            doublon = false;
                            for (int t=0; t < premier[i].size(); t++)
                            {
                                if (premier[i][t][0]==terminauxNR[k])
                                {
                                    doublon = true;
                                }
                            }
                            if (doublon == false)
                            {
                                terminal.erase();
                                terminal += terminauxNR[k];
                                premier[i].push_back(terminal);
                            }
                        }
                    }

                    for(int k=0; k<nonTerminauxNR.size(); k++) // Parcours des non terminaux
                    {
                        if((grammaireNR[i][j][indiceChar]==nonTerminauxNR[k][0] && nonTerminauxNR[k].size()==1) || (grammaireNR[i][j][indiceChar]==nonTerminauxNR[k][0] && grammaireNR[i][j][indiceChar+1]=='`' && nonTerminauxNR[k].size()==2)) // Si le caractère actuel de la case est un non terminal
                        {
                            int l=0;
                            while(premier[l][0]!=nonTerminauxNR[k]) // Pour se placer à la bonne ligne dans les premiers
                            {
                                l++;
                            }
                            for(int m=1; m<premier[l].size(); m++) // Parcours des premiers de la ligne actuelle
                            {
                                doublon=false;
                                for(int n=1; n < premier[i].size(); n++)
                                {
                                    if(premier[i][n] ==  premier[l][m])
                                    {
                                        doublon = true;
                                    }
                                }
                                if ((doublon == false && premier[l][m]!="#") || (doublon == false && grammaireNR[i][j][indiceChar+1]=='`'))
                                {
                                    premier[i].push_back(premier[l][m]);
                                }
                            }

                            int o=0;
                            while(grammaireNR[o][0]!=nonTerminauxNR[k])
                            {
                                o++;
                            }
                            for(int p=1; p<grammaireNR[o].size(); p++)
                            {

                                if(grammaireNR[o][p][0]=='#')
                                {
                                    continuer=true;
                                }
                            }

                        }
                    }
                    if(continuer==true)
                    {
                        indiceChar++;
                    }
                }
            }
        }
    }

    for(int i=0; i<premier.size(); i++)
    {
        cout<<endl;
        for(int j=0; j<premier[i].size(); j++)
        {
            if (j == 0)
            {
                cout<<endl <<"P("<<premier[i][j]<<")";
            }
            if (j == 1)
            {
                cout<<" = {"<<premier[i][j];
            }
            if (j > 1)
            {
                cout<<";"<<premier[i][j];
            }
        }
        cout<<"}";
    }
    cout << endl << endl;
    Suivant (grammaireNR, terminauxNR, nonTerminauxNR, premier);
}

void Suivant (vector<vector<string> > grammaireNR, vector<char> terminauxNR, vector<string> nonTerminauxNR, vector<vector<string> > premier)
{
    vector <vector<string > > suivant;
    vector<vector<string > > comparateur; // grammaire avec chaque caractère séparé l'un de l'autre
    string tmp;
    string nt;

    cout<<"Suivants";

    for (int i = 0; i < grammaireNR.size(); i++) // On cré une copie de la grammaire avec chaque caractère séparé
    {
        suivant.push_back(vector<string>(1,grammaireNR[i][0]));
        comparateur.push_back(vector<string>(1,grammaireNR[i][0]));

        for (int j = 1; j < grammaireNR[i].size(); j++)
        {
            for (int l = 0; l < grammaireNR[i][j].size(); l++)
            {
                tmp = grammaireNR[i][j][l];

                if (grammaireNR[i][j][l+1] == '`')
                {
                    tmp += '`';
                    l++;
                }
                comparateur[i].push_back(tmp);
            }
        }
    }

    suivant[0].push_back("$"); // $ dans l'axiome

    for (int i = 0; i < grammaireNR.size(); i++) // On cherche les caractères qui suivent chaque NT
    {
        for (int j = 1; j < grammaireNR[i].size(); j++)
        {
            for(int m=0; m<grammaireNR[i][j].size()-1; m++)// Pour chaque caractère de la case
            {
                for(int n=0; n<nonTerminauxNR.size(); n++)
                {
                    if(grammaireNR[i][j][m]==nonTerminauxNR[n][0] && grammaireNR[i][j][m+1]!='`') //Si c'est un NT
                    {
                        for (int a = 0; a < suivant.size(); a++) // Pour le premier caractère de chaque ligne de suivant
                        {
                            if(suivant[a][0] == nonTerminauxNR[n])
                            {
                                string s;
                                if(grammaireNR[i][j][grammaireNR[i][j].size()-1]=='`')
                                {
                                    s += grammaireNR[i][j][m+1];
                                    s += '`';
                                }
                                else
                                {
                                    s += grammaireNR[i][j][m+1];
                                }

                                bool doublon = false;
                                for(int t=1; t<suivant[a].size(); t++)
                                {
                                    if(suivant[a][t]==s)
                                    {
                                        doublon  = true;
                                    }
                                }
                                if(doublon==false)
                                {
                                    suivant[a].push_back(s);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    int size;

    for(int o=0; o<suivant.size(); o++) // REGLE 2
    {
        for(int p=1; p<suivant[o].size(); p++)
        {
            for(int q=0; q<premier.size(); q++)
            {
                for(int r=0; r<premier[q].size(); r++)
                {
                    if(suivant[o][p]==premier[q][0])
                    {
                        for(int t=1; t<premier[q].size(); t++)
                        {
                            if(premier[q][t]=="#")
                            {
                                size=premier[q].size()-2;
                            }
                            else
                            {
                                size=premier[q].size()-1;
                            }
                        }
                        int cpt2=1;
                        while(cpt2<size)
                        {
                            suivant[o].push_back("");
                            cpt2++;
                        }
                        suivant[o][p].erase();
                        int cpt=1;
                        int cpt3=suivant[o].size()-size;
                        while(cpt<premier[q].size())
                        {
                            if(premier[q][cpt]!="#")
                            {
                                suivant[o][cpt3]=premier[q][cpt];
                            }
                            cpt3++;
                            cpt++;
                        }
                    }
                }
            }
        }
    }

    for(int i=0; i<grammaireNR.size(); i++) //REGLE 3
    {
        for(int j=1; j<grammaireNR[i].size(); j++)
        {
            for(int k=0; k<grammaireNR[i][j].size(); k++)
            {
                if(k==grammaireNR[i][j].size()-1 && grammaireNR[i][j][k]!='`')
                {
                    for(int o=0; o<suivant.size(); o++)
                    {
                        if(suivant[o][0][0] == grammaireNR[i][j][k])
                        {
                            int cpt=1;
                            do
                            {
                                if(suivant[i].size()==1)
                                {
                                    suivant[o].push_back(suivant[i][cpt]);
                                }
                                else
                                {
                                    bool doublon = false;
                                    for(int t=1; t<suivant[o].size(); t++)
                                    {
                                        if(suivant[o][t]==suivant[i][cpt])
                                        {
                                            doublon  = true;
                                        }
                                    }
                                    if(doublon==false)
                                    {
                                        suivant[o].push_back(suivant[i][cpt]);
                                    }
                                }
                                cpt++;
                            }
                            while(cpt<suivant[i].size());
                        }
                    }
                }

                if(k==grammaireNR[i][j].size()-2 && grammaireNR[i][j][k+1]=='`' && grammaireNR[i][0][0]+grammaireNR[i][0][1]!=grammaireNR[i][j][k]+grammaireNR[i][j][k+1])
                {
                    for(int o=0; o<suivant.size(); o++)
                    {
                        if(suivant[o][0][0] == grammaireNR[i][j][k] && suivant[o][0].size()==2)
                        {
                            int cpt=1;
                            do
                            {
                                if(suivant[i].size()==1)
                                {
                                    suivant[o].push_back(suivant[i][cpt]);
                                }
                                else
                                {
                                    bool doublon = false;
                                    for(int t=1; t<suivant[o].size(); t++)
                                    {
                                        if(suivant[o][t]==suivant[i][cpt])
                                        {
                                            doublon  = true;
                                        }
                                    }
                                    if(doublon==false)
                                    {
                                        suivant[o].push_back(suivant[i][cpt]);
                                    }
                                }
                                cpt++;
                            }
                            while(cpt<suivant[i].size());
                        }
                    }
                }

                if(j==comparateur[i].size()-2)
                {
                    for(int z=0; z<nonTerminauxNR.size(); z++)
                    {
                        for(int w=0; w<nonTerminauxNR.size(); w++)
                        {
                            if(comparateur[i][j+1]==nonTerminauxNR[z] && comparateur[i][j]==nonTerminauxNR[w])
                            {
                                for(int y=0; y<premier[z].size(); y++)
                                {
                                    if(premier[z][y]=="#")
                                    {
                                        int cpt=0;

                                        while(comparateur[i][j]!=suivant[cpt][0])
                                        {
                                            cpt++;
                                        }

                                        int cpt2=1;
                                        do
                                        {
                                            bool doublon = false;
                                            for(int t=1; t<suivant[cpt].size(); t++)
                                            {
                                                if(suivant[cpt][t]==suivant[i][cpt2])
                                                {
                                                    doublon  = true;
                                                }
                                            }
                                            if(doublon==false)
                                            {
                                                suivant[cpt].push_back(suivant[i][cpt2]);
                                            }
                                            cpt2++;
                                        }
                                        while(cpt2!=suivant[i].size());
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for(int o=0; o<suivant.size(); o++)
    {
        cout<<endl;
        for(int p=0; p<suivant[o].size(); p++)
        {
            if (p == 0)
            {
                cout<<endl <<"S("<<suivant[o][p]<<")";
            }
            if (p == 1)
            {
                if(p==suivant[o].size()-1)
                {
                    cout<<" = {"<<suivant[o][p];
                }
                if(suivant[o][p][0]!='\0' && p!=suivant[o].size()-1)
                {
                    cout<<" = {"<<suivant[o][p]<<";";
                }
                if(suivant[o][p][0]=='\0' && p!=suivant[o].size()-1)
                {
                    cout<<" = {"<<suivant[o][p];
                }
            }
            if(p > 1 && suivant[o][p][0]!='\0')
            {
                if(p==suivant[o].size()-1)
                {
                    cout<<suivant[o][p];
                }
                else
                {
                    cout<<suivant[o][p]<<";";
                }

            }


        }
        cout<<"}";
    }
    Table(grammaireNR,terminauxNR,nonTerminauxNR,premier,suivant);
}

void Table (vector<vector<string> > grammaireNR, vector<char> terminauxNR, vector<string> nonTerminauxNR, vector<vector<string> > premier, vector<vector<string> > suivant)
{
    cout<<"\n\nTable d'analyse\n";
    terminauxNR.push_back('$');
    string table[nonTerminauxNR.size()+1][terminauxNR.size()+1];

    for(int i=0; i<=nonTerminauxNR.size(); i++)
    {
        for(int j=0; j<=terminauxNR.size(); j++)
        {
            table[0][0]="  ";
            if(i==0 && j!=0)
            {
                table[i][j]=terminauxNR[j-1];
            }
            if(j==0 && i!=0)
            {
                table[i][j]=nonTerminauxNR[i-1];
            }
        }
    }

    for(int k=0; k<grammaireNR.size(); k++)
    {
        for(int l=1; l<grammaireNR[k].size(); l++)
        {
            bool nTerm=false;
            bool term=false;

            for(int a=0; a<nonTerminauxNR.size(); a++)
            {
                if(nonTerminauxNR[a][0]==grammaireNR[k][l][0])
                {
                    nTerm=true;
                }
            }
            for(int a=0; a<terminauxNR.size(); a++)
            {
                if(terminauxNR[a]==grammaireNR[k][l][0])
                {
                    term=true;
                }
            }
            if(grammaireNR[k][l]=="#")
            {
                for(int a=1; a<suivant[k].size(); a++)
                {
                    int cpt=1;
                    while(suivant[k][a]!=table[0][cpt])
                    {
                        cpt++;
                    }
                    table[k+1][cpt]=grammaireNR[k][0]+"->#";
                }
            }
            if(term==true)
            {
                int cpt=1;
                while(grammaireNR[k][l][0]!=table[0][cpt][0])
                {
                    cpt++;
                }
                table[k+1][cpt]=grammaireNR[k][0]+"->"+grammaireNR[k][l];
            }
            if(nTerm==true)
            {
                int cpt=0;
                if(grammaireNR[k][l].size()==2 && grammaireNR[k][l][grammaireNR[k][l].size()-1]=='`')
                {
                    while(premier[cpt][0]!=grammaireNR[k][l]) // On se place à la bonne ligne dans les premiers
                    {
                        cpt++;
                    }
                }
                else
                {
                    while(premier[cpt][0][0]!=grammaireNR[k][l][0]) // On se place à la bonne ligne dans les premiers
                    {
                        cpt++;
                    }
                }
                for(int m=1; m<premier[cpt].size(); m++) // On parcourt les premiers du premier caractère de la partie droite
                {
                    if(premier[cpt][m]!="#")
                    {
                        int cpt2=1;
                        while(premier[cpt][m]!=table[0][cpt2])
                        {
                            cpt2++;
                        }
                        table[k+1][cpt2]=grammaireNR[k][0]+"->"+grammaireNR[k][l];
                    }
                }
            }
        }
    }

    for(int i=0; i<=nonTerminauxNR.size(); i++)
    {
        cout<<endl;
        for(int j=0; j<=terminauxNR.size(); j++)
        {
            if(i==0 && j!=0)
            {
                cout<<table[i][j]<<"            ";
            }
            if(j==0)
            {
                if(table[i][0].size()==2)
                {
                    cout<<table[i][j]<<" ";
                }
                if(table[i][0].size()==1)
                {
                    cout<<table[i][j]<<"  ";
                }
                cout<<"           ";
            }
            if(i!=0 && j!=0)
            {
                int cpt=0;
                cout<<table[i][j]<<"   ";
                if((10-table[i][j].size()!=0)&& !table[i][j].empty())
                {
                    while(cpt!= 10-table[i][j].size())
                    {
                        cpt++;
                        cout<<" ";
                    }
                }
            }
            if(table[i][j].empty())
            {
                cout<<"          ";
            }
        }
    }
    cout<<endl;
}
