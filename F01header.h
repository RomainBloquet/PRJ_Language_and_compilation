#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#include <vector>

using namespace std;

void Menu();
vector<char> Lecture(string);
void LectureNonRec(string, vector<char>);
vector<char> ElimRecur(vector<vector<string> >,vector<char>,vector<char>);
void Premier(vector<vector<string> >,vector<char>,vector<string>);
void Suivant (vector<vector<string> > , vector<char> , vector<string> , vector<vector<string> > );
void Table (vector<vector<string> > , vector<char> , vector<string> , vector<vector<string> > , vector<vector<string> >);

#endif // HEADER_H_INCLUDED
