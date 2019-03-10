//
//  program1.cpp
//  
//
//  Created by Ivan on 2/26/19.
//


#include <stdio.h>
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

//opening files
ifstream input("ciphertext.txt");
ifstream dc("dictionary.txt");
ifstream input2("something.txt");
ofstream output("something.txt");


//global variables
int bestKey;
int counter[26];
int stdfreq[] = { 'E','T', 'A', 'O', 'I', 'N', 'S', 'H', 'R', 'D', 'L', 'C', 'U', 'M', 'W', 'F', 'G', 'Y', 'P', 'B', 'V', 'K', 'J', 'X', 'Q', 'Z'};

//prototypes
int  wordCounter(ifstream& in, char w[], int size);
void fqCount(ifstream& in);
void decrypt(ifstream& in, int key);
int  autoDecrypt(ifstream& in, int key);
int  applyDictionary();


//compares the counter array to the stdfreq array
// to determine how many letters are in the correct
// place in a standard frequency table. 
int validate(){
    
    
    int tp, tp2;
    int temp[26];
    int ap[26];
    
    for(int q = 0; q < 26; q++){
        temp[q] = counter[q];
        ap[q] = 65 + q;
    }
    //ordering temp from high to low.
    for(int j = 0; j < 26; j++){
        for(int i = 0; i < j; i++)
            if(temp[j] > temp[i]){
                tp  = temp[i];
                tp2 = ap[i];
                
                temp[i] = temp[j];
                ap[i] = ap[j];
                
                temp[j] = tp;
                ap[j] = tp2;
            }//end if
    }//end for j
    
    
    int count = 0;
    for(int i = 0; i < 26; i++){
        if(ap[i] == stdfreq[i])
            count++;
        if(i > 1 && i < 26)
        if(ap[i] == stdfreq[i+1] || ap[i] == stdfreq[i-1])
            count++;
        
    }//end for
    
    return count;
    
}//end validate
//---------------------------------------------------------------------------

//takes input from a provided ifstream and updates
// the frequency table
void fqCount(ifstream& in){
    //starting file for beginning
    in.clear();
    in.seekg(0, in.beg);
    
    //initializing all letter counters to zero
    for(int j = 0; j < 26; j++)
        counter[j] = 0;
    
    //temporary variables
    char temp;
    int pos;
    
    //counting letters
    while(in){
        in>>temp;
        toupper(temp);
        pos = temp - 'A';
        counter[pos] = counter[pos] + 1;
    }//end while
    
    
}//end fqCount
//---------------------------------------------------------------------------


//decrypts a file from in, with the provided key.
void decrypt(ifstream& in, int key){
    //starting file from beginning
    in.clear();
    in.seekg(0, in.beg);
    
    printf("Decrypting with key %d\n",key);
    printf("The plaintext is:\n\n");
    char temp;
    
    //decrypting
    while(in){
        in>>temp;
        temp = temp + key;
        if(temp < 65)
            temp = temp + 26;
        if(temp > 90)
            temp = temp - 26;
        printf("%c", temp);
    }//end while
    printf("\n");
    
    
}//end decrypt
//---------------------------------------------------------------------------


int autoDecrypt(ifstream& in, int key){
    //base cases
    if(key >= 26)
        return 0;
    if(key <= -26)
        return 0;
    
    //starting file from beginning
    in.clear();
    in.seekg(0, in.beg);
    


    //initializing all letter counters to zero
    for(int j = 0; j < 26; j++)
        counter[j] = 0;
    
    char temp;
    int pos;
    output.open("something.txt");
    
    
    //applying the key and creating frequency table.
    while(in){
        
        in>>temp;
        toupper(temp);
        
        temp = temp + key;
        if(temp < 65)
            temp = temp + 26;
        if(temp > 90)
            temp = temp - 26;

        pos = temp - 'A';
        counter[pos] = counter[pos] + 1;
        
        output << temp;
        
        
    }//end while
    output.close();
    int words = 0;//applyDictionary();
    
    int tempKey = key + 1;
    int tempKey2 = 0 - tempKey;
    int x = validate();
    if(key < 0){
        return x;
    }
    int y = autoDecrypt(in, tempKey);
    int z = autoDecrypt(in, tempKey2);
   // printf("Testing key  %d, validate = %d number of words %d\n", key, x, words);
   // printf("Testing key  %d, validate = %d\n", tempKey2, z);

    
    if(x > y && x > z){
        bestKey = key;
        return x;}
    
    if(y > x && y > z){
        bestKey = key + 1;
        return y;}
    
    if(z > x && z > y){
        bestKey = (key+1) * -1;
        return z;}
    
    if(x == y){
        bestKey = key;
        return x;}
    
    if(z == y){
        bestKey = key + 1;
        return z; }
    
    bestKey = key;
    return x;
    
}//end autoDecrypt
//---------------------------------------------------------------------------




int wordCounter(ifstream& in, char w[], int size){
    //starting file from beginning
    in.clear();
    in.seekg(0, in.beg);
    bool match = false;
    int count = 0, x = 0;
    char temp, buff[size];
    
    //initializing buff array to 0
    for(int j = 0; j < size; j++)
        buff[j] = 0;
    
    
    //counting number of matches for the specific word.
    while(in){
        
        in>>temp;
        
        if(x >= size)
            x = 0;
        
        buff[x] = temp;
        
        for(int j = 0; j < size; j++){
            if(buff[j] == w[j])
                match = true;
            
            else{match = false;j = size;}//end els
        }//end for j
        
        if(match == true)
            count++;
        
        x++;
        
    }//end while
    
    
    
    return count;
}//end wordCounter
//---------------------------------------------------------------------------


int applyDictionary(){
    //starting file from beginning
    dc.clear();
    dc.seekg(0, dc.beg);
    
    char temp, buff[20];
    int  count, size = 0;
 
    while(dc){
        dc>>temp;
        
        
        if(isalpha(temp) && size < 20){
            toupper(temp);
            buff[size] = temp;
            size++;
          
        }//end if
        else{
            //count = count + wordCounter(input, buff, size);
            for(int w = 0; w < 20; w++){
                printf("%c", buff[w]);}//end for
            printf("\n");
            size = 0;
        }//end else
     
        
    }//end while
   
    
    
    
    return count;
    
}//end applyDictionary
//---------------------------------------------------------------------------



int main(){

    //calling frequency counter
    fqCount(input);
    
    
    //printing the frequency table
    int temp = 'A';
    printf("The frequency table for the ciphertextt is\n");
    for(int j = 1; j <= 26; j++){
        printf("%c = %d  ",temp,counter[j-1]);
        temp++;
        if((j % 2) == 0)
            printf("\n");
    }//end for j
    
    
    //decrypting the cypertext with a user inputed key
    printf("Input a decryption key.\n");
    cin >> temp;
    decrypt(input,temp);

    int d = autoDecrypt(input, 0);
    printf("\n\n\nThe best key by mapping to the standard English letter frequency ordering is %d and has %d matches.\n",bestKey, d);
    printf("\n");
    decrypt(input,bestKey);
    
    
   // d = applyDictionary();
   // printf("Number of words found in the text is %d\n", d);
    
   
}//end main
//---------------------------------------------------------------------------
