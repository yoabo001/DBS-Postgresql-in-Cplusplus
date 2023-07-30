//Name: Youssef Abousobie
//Matrikelnr: 1312001
#include <iostream>
#include "db.h"
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;
int main(int argc, char* argv[]){
if (argc == 1)
return  0;

string username = argv[2];
cout << argv[2]<< endl;
string password = argv[4];
cout << argv[4]<< endl;
string host = argv[6];
cout << argv[6]<< endl;
string port = argv[8];
cout << argv[8]<< endl;
string database = argv[10];
cout << argv[10]<< endl;
int x = db_login(username, password, host, port, database);
cout << x << endl;
 db_begin();

string filename = argv[11];
  string line;
  ifstream testfile (filename);
   
  if (testfile.is_open())
  {
    while ( getline (testfile,line) )
    {
      vector<string> tokens;
      cout << line << '\n';
      istringstream iss(line);
      copy(istream_iterator<string>(iss),
      istream_iterator<string>(),
      back_inserter(tokens));
      for (long unsigned int i = 0; i < tokens.size(); i++) {
        cout << tokens[i] << " " << endl;
      }
      if(tokens[0].compare("n") == 0){

       
        db_drop_table("produkt");
        db_drop_table("hersteller");
        db_create_table_hersteller();
        db_create_table_produkt();
     

      }
      else if(tokens[0].compare("ih") == 0){

      if(db_check_hnr(tokens[1]) == 0)
          {
           db_insert_hersteller(tokens[1], tokens[2], tokens[3]);
          }
          else if (db_check_hnr(tokens[1]) == 1)
          {
            db_update_hersteller(tokens[1],tokens[2], tokens[3]);
          }
          else
          {
            db_rollback();
          }
          



        /*if(db_insert_hersteller(tokens[1], tokens[2], tokens[3]) == -1){
          db_update_hersteller(tokens[1],tokens[2], tokens[3]);
        }*/
      }
      
      else if(tokens[0].compare("ip") == 0){
        
       if(db_check_pnr(tokens[1]) == 0)
          {
           db_insert_produkt(tokens[1], tokens[2], tokens[3], tokens[4]);
          }
          else if (db_check_pnr(tokens[1]) == 1)
          {
            db_update_produkt(tokens[1],tokens[2], tokens[3], tokens[4]);
          }
          else
          {
            db_rollback();
          }
          
          
       /*if(db_insert_produkt(tokens[1], tokens[2], tokens[3], tokens[4]) == -1){
         db_update_produkt(tokens[1],tokens[2], tokens[3], tokens[4]);
        }
      */
      }
      else if(tokens[0].compare("i") == 0){
          if(db_check_hnr(tokens[1])==0){
            db_insert_hersteller(tokens[1],tokens[2],tokens[3]);
          }
            else if (db_check_hnr(tokens[1])==1)
            {
              db_update_hersteller(tokens[1],tokens[2],tokens[3]); 
            }
            else
            {
              db_rollback();
            }
         if (db_check_pnr(tokens[1])==0)
            {
              db_insert_produkt(tokens[4], tokens[5],tokens[6],tokens[1]);
            }
            else if (db_check_pnr(tokens[1])==1)
            {
              db_update_produkt(tokens[4],tokens[5],tokens[6],tokens[1]);
            }
            else
            {
              db_rollback();
            }
          






         /*if(db_insert_hersteller(tokens[1],tokens[2],tokens[3]) == -1){
          db_update_hersteller(tokens[1],tokens[2],tokens[3]); 
        }
        if(db_insert_produkt(tokens[4], tokens[5],tokens[6],tokens[1]) == -1){
          db_update_produkt(tokens[4],tokens[5],tokens[6],tokens[1]);
        }
       */
      }
      else if(tokens[0].compare("dh") == 0){
        db_delete_hersteller(tokens[1]);
      }
      else if(tokens[0].compare("dp") == 0){
        db_delete_produkt(tokens[1]);
      }
      else if(tokens[0].compare("ch") == 0){
      db_count("hersteller");  
      }
      else if(tokens[0].compare("cp") == 0){
      db_count("produkt");
      }
      else {
        cout<<"Command not understood"<<endl;
      }
    }
    testfile.close();
  }

  else cout << "Unable to open file"; 


db_commit();
db_logout();

  return 0;
 

}

