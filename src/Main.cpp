#include "iostream"
#include <cstdio>
#include "string"
#include <sstream>
#include "fstream"
#include <iterator>
#include <bits/stdc++.h>

using namespace std;

class Solitaire {
    public:
        // Array size
        static const int DECK_SIZE = 52;
        static const int REM_DECK_SIZE = 28;
        static const int STOCK_SIZE = 24;
        static const int ROW = 20;
        static const int COLUMN = 7;

        string heart[15];
        string diamond[15];
        string spades[15];
        string clubs[15];

        string deck[DECK_SIZE];
        string rem_deck[REM_DECK_SIZE];
        string stock[STOCK_SIZE];


        string table[ROW][COLUMN];
        string visible[50];


        string stockArr[3]={"0","0","0"};
        string stockRem[24];
        int stockRemCapacity = 24;
        int stockCommand = 0;

        // check that if stock empty
        string empty = "@@@";



        template <size_t rows, size_t cols, size_t count, size_t a, size_t h, size_t s, size_t c, size_t d>
        void printTable(const char* outputFile, string (&table)[rows][cols], string(&visible)[count], string(&stockArr)[a], string(&heart)[h], string(&spades)[s],
                        string(&clubs)[c], string(&diamond)[d], string command, string empty)
        {
            FILE *write;
            write = fopen(outputFile,"a");
            string stock[3];
            for(int i=0; i<3; i++){
                if(stockArr[i]!="0"){
                    stock[i]=stockArr[i];
                }
                else{
                    stock[i]="___";
                }
            }
            int dd, ss, hh, cc;
            for(dd =0; dd<d-1; dd++){
                if(diamond[dd].empty()){
                    break;
                }
            }
            for(hh =0; hh<h-1; hh++){
                if(heart[hh].empty()){
                    break;
                }
            }
            for(cc =0; cc<c-1; cc++){
                if(clubs[cc].empty()){
                    break;
                }
            }
            for(ss =0; ss<s-1; ss++){
                if(spades[ss].empty()){
                    break;
                }
            }
            string heartS;
            string clubS;
            string diamondS;
            string spadeS;

            if(dd == 0) {
                diamondS = "___";
            }
            else{
                diamondS=diamond[dd-1];
            }
            if(cc == 0) {
                clubS = "___";
            }
            else{
                clubS=clubs[cc-1];
            }
            if(ss == 0) {
                spadeS = "___";
            }
            else{
                spadeS= spades[ss-1];
            }
            if(hh == 0) {
                heartS = "___";
            }
            else{
                heartS= heart[hh-1];
            }

//            string at ="@@@";
//            if( empty ){
//                at = "___";
//            }
            fprintf(write,"\n%s %s %s %s         %s %s %s %s \n\n",empty.c_str(), stock[0].c_str(),stock[1].c_str(),stock[2].c_str(),heartS.c_str(),diamondS.c_str(),spadeS.c_str(),clubS.c_str());
            int cardNum=0;
            for(int i=0; i<20; i++){
                for(int j=0; j<7; j++){
                    if(table[i][j]!= "0" && !table[i][j].empty()){
                        cardNum++;
                        break;
                    }

                }
            }


            for(int i=0; i<cardNum; i++){
                for(int j=0; j<7; j++){
                    string card=table[i][j];
                    if(card=="0"){
                        fprintf(write,"      ");
                    }
                    else {
                        bool found= false;
                        for(int x=0; x<count; x++ ){
                            if(visible[x].empty() == 0 && visible[x] !="0") {
                                string vis = visible[x];
                                if (vis == card) {
                                    found=true;
                                    fprintf(write,"%s   ", card.c_str());
                                    break;
                                }
                            }
                        }
                        if(!found && card.empty() == 0){
                            fprintf(write,"@@@   ");
                        }
                    }
                }
                fprintf(write,"\n");
            }
            fprintf(write,"\n%s\n\n****************************************\n", command.c_str());
            fclose(write);
        }
        template <size_t N>
        void splitString(string (&arr)[N], const string& str){
            stringstream ssin(str);
            int i=0;
            while (ssin.good() && i < N){
                ssin >> arr[i];
                ++i;
            }
        }

        int fileReader(const string& deckFile, const string& commandF, const char* outputFile){
            int count = 0;             // Loop counter variable

            ifstream inputFile;        // Input file stream object
            // Open the file.
            inputFile.open(deckFile.c_str());

            // Read the cards from the file into the deck array
            while (DECK_SIZE > count && inputFile >> deck[count]){
                count++;
            }
            // Close the file.
            inputFile.close();

            // fills first 24 cards into the stock array
            for (count = 0; count < 24; count++){
                stock[23-count] = deck[count];
            }

            // fills last 28 cards into the rem deck
            for (count = 0; count < 28; count++){
                rem_deck[27-count] = deck[24+count];
            }


            for(int i=0; i<ROW; i++){
                for(int j=0; j<COLUMN; j++){
                    if(i>6){
                        table[i][j]="0";
                    }
                    else{
                        if(j>=i){
                            int index = (7 * i) + j-((i * (i+1)) / 2);
                            table[i][j]=rem_deck[index];

                        }
                        else {
                            table[i][j] = "0";
                        }
                    }
                }
            }

            for(int i=0; i<50; i++){
                if(i>6){
                    visible[i]="0";
                }
                else{
                    visible[i]=table[i][i];
                }
            }


            // Create a text string, which is used to output the text file
            string command;

            // Read from the text file
            ifstream commandFile(commandF.c_str());


            // Use a while loop together with the getline() function to read the file line by line
            while (getline (commandFile, command)) {
                string commandArr[5];
                splitString(commandArr, command);
                printTable(outputFile,table,visible,stockArr, heart,spades,clubs,diamond, command , empty);


                //move to foundation waste command
                if(commandArr[2]=="foundation" && commandArr[3]=="waste"){
                    int k;
                    for(k=2; k>-1; k--) {
                        if(stockArr[k] != "0"){
                            break;
                        }
                    }
                    string card= stockArr[k];

                    // declaring character array
                    char cardArr[card.length() + 1];

                    // copying the contents of the
                    // string to char array
                    strcpy(cardArr, card.c_str());
                    int n;
                    switch (cardArr[0]) {
                        case 'D' : //2
                            for(n=0; n<16; n++){
                                if(diamond[n].empty()){
                                    break;
                                }

                            }
                            diamond[n]=stockArr[k];
                            stockArr[k]="0";
                            break;

                        case 'S':
                            for(n=0; n<15; n++){
                                if(spades[n].empty()){
                                    break;
                                }
                            }
                            spades[n]=stockArr[k];
                            stockArr[k]="0";
                            break;
                        case 'C'://4
                            for(n=0; n<15; n++){
                                if(clubs[n].empty()){
                                    break;
                                }
                            }
                            clubs[n]=stockArr[k];
                            stockArr[k]="0";
                            break;
                        case 'H':
                            for(n=0; n<15; n++){
                                if(heart[n].empty()){
                                    break;
                                }
                            }
                            heart[n]=stockArr[k];
                            stockArr[k]="0";
                            break;
                    }

                }

                    //move to foundation pile pile_num command
                else if(commandArr[2]=="foundation"&&commandArr[3]=="pile"){
                    int i;
                    int pileNum= atoi(commandArr[4].c_str());
                    for(i=0; i<15; i++){
                        if(table[i][pileNum].empty() || table[i][pileNum]=="0"){
                            break;
                        }

                    }
                    string card=table[i-1][pileNum];

                    // declaring character array
                    char cardArr[card.length() + 1];

                    // copying the contents of the
                    // string to char array
                    strcpy(cardArr, card.c_str());
                    int n;
                    switch (cardArr[0]) {
                        case 'D' : //2
                            for(n=0; n<16; n++){
                                if(diamond[n].empty()){
                                    break;
                                }

                            }
                            diamond[n]=table[i-1][pileNum];
                            table[i-1][pileNum]="0";
                            break;

                        case 'S':
                            for(n=0; n<15; n++){
                                if(spades[n].empty()){
                                    break;
                                }
                            }
                            spades[n]=table[i-1][pileNum];
                            table[i-1][pileNum]="0";
                            break;
                        case 'C'://4
                            for(n=0; n<15; n++){
                                if(clubs[n].empty()){
                                    break;
                                }
                            }
                            clubs[n]=table[i-1][pileNum];
                            table[i-1][pileNum]="0";
                            break;
                        case 'H':
                            for(n=0; n<15; n++){
                                if(heart[n].empty()){
                                    break;
                                }
                            }
                            heart[n]=table[i-1][pileNum];
                            table[i-1][pileNum]="0";
                            break;


                    }


                }

                    // move [ pile <source_pile_num> <source_pile_card_index> ] <destination_pile_num> command
                else if(commandArr[0]=="move"&& commandArr[1]=="pile"){
                    int i;
                    int pileNum= atoi(commandArr[2].c_str());
                    int num= atoi(commandArr[3].c_str());
                    int destination= atoi(commandArr[4].c_str());
                    for(i=0; i<15; i++){
                        if(table[i][pileNum].empty() || table[i][pileNum]=="0"){
                            break;
                        }

                    }
                    int k;
                    for(k=0; k<15; k++){
                        if(table[k][destination].empty() || table[k][destination]=="0"){
                            break;
                        }

                    }
                    for(int j=num; j>=0; j--){
                        if(i>=num) {
                            table[k+num-j][destination] = table[i - j-1][pileNum];
                            table[i-j-1][pileNum] = "0";

                        }
                        else {
                            cout<<"error"<<endl;
                            //error
                        }
                    }
                }

                //open from stock command
                else if(commandArr[1]=="from"){

                    int j;
                    for(j=0; j<24; j++){
                        if(stockRem[j].empty() || stockRem[j] == "0")
                            break;
                    }
                    if(stockArr[2] != "0"){  // A A A
                        for(int k=0; k<3; k++) {
                            stockRem[j + k] = stockArr[k];
                        }
                        j=j+3;
                    }
                    else if(stockArr[1] != "0"){ // A A _
                        for(int k=0; k<2; k++) {
                            stockRem[j + k] = stockArr[k];
                        }
                        j = j+2;
                    }
                    else if (stockArr[0] != "0"){ // A _ _
                        stockRem[j] = stockArr[0];
                        j= j+1;
                    }

                    if(stockCommand> stockRemCapacity-1){
                        empty = "@@@";
                        stockCommand =0;
                        stockRemCapacity = j;



                        for(int k=0; k<3; k++) {
                            stockArr[k] = "0";

                        }
                        for(int i=0; i<24; i++){
                            if(i<stockRemCapacity) {
                                stock[i] = stockRem[i];
                                stockRem[i] = "0";
                            }
                            else{
                                stock[i] = "0";
                            }
                        }

                    }

                    else if(stockRemCapacity>stockCommand-1 && stockRemCapacity<stockCommand+5){
                        empty = "___";
                        for(int k=0; k<3; k++){
                            stockArr[k]= stock[stockCommand+k];
                        }
                        stockCommand = stockCommand + 3;
                    }
                    else if(stockRemCapacity>stockCommand-1 && stockRemCapacity<stockCommand+3){
                        empty = "___";
                        int dif= stockRemCapacity-stockCommand;
                        for(int k=0; k<dif; k++){
                            stockArr[k]= stock[stockCommand+k];
                        }
                        stockCommand = stockCommand + 3;

                    }
                    else if(stockRemCapacity>stockCommand-1 ){
                        empty = "@@@";
                        for(int k=0; k<3; k++){
                            stockArr[k]= stock[stockCommand+k];
                        }
                        stockCommand = stockCommand + 3;

                    }
                    cout<<empty<<"  "<<stockRemCapacity<<" "<<stockCommand<<endl;



                }

                //open 5
                else if(commandArr[0]=="open"){
                    int i, j;
                    int pileNum= atoi(commandArr[1].c_str());
                    for(i=0; i<15; i++){
                        if(table[i][pileNum].empty() || table[i][pileNum]=="0"){
                            break;
                        }
                    }
                    //int count = sizeof(visible)/sizeof(visible[0]);
                    for(j=0; j<50; j++ ){
                        if(visible[j]=="0"){
                            break;
                        }
                    }
                    visible[j]=table[i-1][pileNum];

                }

                    //move waste 6
                else if(commandArr[0]=="move" && commandArr[1]=="waste"){
                    int i, j,k;
                    int pileNum= atoi(commandArr[2].c_str());

                    for(i=0; i<15; i++){
                        if(table[i][pileNum].empty() || table[i][pileNum]=="0"){
                            break;
                        }
                    }

                    for(k=2; k>-1; k--){
                        if(stockArr[k] != "0"){
                            table[i][pileNum]= stockArr[k];
                            stockArr[k]="0";
                            break;
                        }
                    }
                    // if there is no card open and there are cards in waste open only one
                    if(stockArr[0] == "0" && !stockRem->empty()){
                        for(int a=0; a<20; a++){
                            if(stockRem[a].empty() || stockRem[a] == "0") {
                                stockArr[0] = stockRem[a-1];
                                stockRem[a-1] = "0";
                                break;
                            }
                        }
                    }

                    for(j=0; j<50; j++) {
                        if(visible[j]=="0"){
                            break;
                        }
                    }
                    visible[j] = table[i][pileNum];

                }




            }

            // Close the file
            commandFile.close();

            return 0;

        }
    };






int main(int argc, char **argv){
    Solitaire solitaire;
    solitaire.fileReader(argv[1],argv[2], argv[3]);


    return 0;
}