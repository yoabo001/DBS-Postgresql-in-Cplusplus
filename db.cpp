/*Name: Youssef Abousobie
Matrikelnr : 1312001*/
#include <libpq-fe.h>
#include <iostream>
#include "db.h"
#include <string>
#include <chrono>
#include <thread>
using namespace std;
PGconn* conn;
    

int db_login(const string &user, const string &password, const string &host, const string &port, const string &dbname){
    cout<<"entered db_login function"<<endl;
   
    string connectionstring = "user="+ user + " password="+ password + " host="+ host + " port=" + port + " dbname="+ dbname;
    cout <<connectionstring<<endl;
    conn = PQconnectdb(connectionstring.c_str());
    cout<<"called connectdb function"<<endl;
  
    while (1){
        cout<<PQstatus(conn)<<endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
        switch(PQstatus(conn))
        {
            case CONNECTION_STARTED:
                cout << "Connecting...";
                break;

            // case CONNECTION_MADE:
            //     cout << "Connected to server...";
            //     return 1;
            //     break;
            case CONNECTION_BAD:
                cout << "Verbindung konnte nicht gebaut werden.";
                return -1;
            case CONNECTION_OK:
                cout<<"Connection OK"<<endl;
                return 1;
                
            default:
                cout << "Default Connecting...";
        
        }
        
 
    }
    
}
void db_logout(){
    PQfinish(conn);
}
int db_begin(){
    PGresult *res;
    res = PQexec(conn, "BEGIN;");
    if(PQresultStatus(res) == PGRES_COMMAND_OK){
        cout<<"befehl BEGIN wurde ausgeführt" <<endl;
         PQclear(res);
        return 1;
    }
    else
    {
        cout<<" befehl BEGIN wurde nicht ausgeführt"<< endl;
         PQclear(res);
        return -1;
    }
   

    
}
int db_commit(){
    PGresult *res;
    res = PQexec(conn, "COMMIT;");
        if(PQresultStatus(res) == PGRES_COMMAND_OK){
        cout<<"befehl COMMIT wurde ausgeführt" <<endl;
        PQclear(res);
        return 1;
         }
         else
            {
          cout<<" befehl COMMIT wurde nicht ausgeführt"<< endl;
           PQclear(res);
         return -1;
         }
             

}
int db_rollback(){
    PGresult *res;
    res = PQexec(conn, "ROLLBACK;");
            if(PQresultStatus(res) == PGRES_COMMAND_OK){
        cout<<"befehl ROLLBACK wurde ausgeführt" <<endl;
        PQclear(res);
        return 1;
         }
         else
            {
          cout<<" befehl ROLLBACK wurde nicht ausgeführt"<< endl;
          PQclear(res);
         return -1;
         }
              
    
}



int db_create_table_produkt(){
  
      PGresult* res;
    res = PQexec(conn,"CREATE TABLE produkt(pnr VARCHAR(4) PRIMARY KEY  , name VARCHAR(30), preis NUMERIC(8,2), hnr VARCHAR(4) REFERENCES hersteller(hnr) ON UPDATE CASCADE ON DELETE CASCADE);");
     if(PQresultStatus(res) == PGRES_COMMAND_OK){
        cout<<"Tabelle produkt wurde kreiert" <<endl;
        PQclear(res);
        return 1;
         }
         else
            {
          cout<<" Tabelle produkt wurde nicht kreiert"<< endl;
          PQclear(res);
         return -1;
           
         }
            
}

int db_create_table_hersteller(){
    
    PGresult* res;
    res = PQexec(conn,"CREATE TABLE hersteller( hnr VARCHAR(4) PRIMARY KEY , name VARCHAR(30), stadt VARCHAR(30));");
    //res = PQexec(conn,"CREATE TABLE hersteller( hnr VARCHAR(4) PRIMARY KEY, name VARCHAR(30), stadt VARCHAR(30), FOREIGN KEY(hnr) REFERENCES produkt(hnr));");
         if(PQresultStatus(res) == PGRES_COMMAND_OK){
        cout<<"Tabelle Hersteller wurde kreiert" <<endl;
          PQclear(res);
        return 1;

         }
         else
            {
          cout<<" Tabelle hersteller wurde nicht kreiert"<< endl;
            PQclear(res);
         return -1;

         }
            
}
int db_drop_table(const string &tablename){
    PGresult* res;
    string droptablecommand = "DROP TABLE IF EXISTS " + tablename + ";";
    cout<< droptablecommand <<endl;
    res = PQexec(conn,droptablecommand.c_str());
    if(PQresultStatus(res) == PGRES_COMMAND_OK){
        cout<<"Tabelle "<<tablename <<"wurde gelöscht " <<endl;
        PQclear(res);
        return 1;
    }
    else
    {
        cout<<"Tabelle "<<tablename <<"wurde nicht gelöscht " <<endl;
        PQclear(res);
        return -1;
    }
  
}

int db_insert_produkt(const string &pnr, const string &name, const string &preis, const string &hnr){
     PGresult* res;
     string insertprodukt = "INSERT INTO produkt(pnr,name, preis, hnr) VALUES (\'"+ pnr +"\', \'"+ name +"\', "+ preis +",\'"+ hnr +"\');";
     cout << insertprodukt<< endl;
    res = PQexec(conn,insertprodukt.c_str());
    cout << res << endl;
    if (PQresultStatus(res) == PGRES_COMMAND_OK) {
    cout << " Einfügen war erfolgreich" << endl;
    PQclear(res);
    return 1;
    }
    else
    {
        cout << PQerrorMessage(conn) << endl;
        
         PQclear(res);
         return -1;
        
    }
    
    
    
}
int db_insert_hersteller(const string &hnr, const string &name, const string &stadt){
     PGresult* res;
     string inserthersteller = "INSERT INTO hersteller(hnr,name, stadt ) VALUES (\'"+ hnr +"\', \'"+ name +"\', \'" + stadt + "\');";
     cout << inserthersteller<< endl;
    res = PQexec(conn,inserthersteller.c_str());
    cout << res << endl;
    if (PQresultStatus(res) == PGRES_COMMAND_OK) {
    cout << " Einfügen in Tabelle Hersteller war erfolgreich" << endl;
    PQclear(res);
    return 1;
    }
    else
    {
        cout << PQerrorMessage(conn) << endl;
         PQclear(res);
      
         return -1;
        
    }
    
    
    
}

int db_update_produkt(const string &pnr, const string &name, const string &preis, const string &hnr){

    PGresult* res;
     string updateprodukt = "UPDATE produkt SET name = \'"+ name +"\', preis = \'"+ preis +"\',hnr = \'"+ hnr +"\' WHERE pnr = \'"+ pnr +"\';";
     //string updateprodukt = "UPDATE produkt SET pnr = \'"+ pnr +"\', name = \'" + name + "\' , preis = \'" + preis + "\', hnr = \'"+ hnr +"\' ;"  ;
     cout << updateprodukt<< endl;
    res = PQexec(conn,updateprodukt.c_str());
    cout << res << endl;
    if (PQresultStatus(res) == PGRES_COMMAND_OK) {
    cout << " Update in Tabelle produkt war erfolgreich" << endl;
    PQclear(res);
    return 1;
    }
    else
    {
        cout << PQerrorMessage(conn) << endl;
         PQclear(res);
         db_rollback();
         return -1;
        
    }





}

int db_update_hersteller(const string &hnr, const string &name, const string &stadt){

    PGresult* res;
     string updatehersteller = "UPDATE hersteller SET name = \'" + name + "\' , stadt = \'"+ stadt +"\' WHERE hnr = \'"+ hnr +"\' ;" ;
     cout << updatehersteller<< endl;
    res = PQexec(conn,updatehersteller.c_str());
    cout << res << endl;
    if (PQresultStatus(res) == PGRES_COMMAND_OK) {
    cout << " Update in Tabelle hersteller war erfolgreich" << endl;
    PQclear(res);
    return 1;
    }
    else
    {
        cout << PQerrorMessage(conn) << endl; // update Hnr kann nicht von der Tabelle geupdated werden alles ist updatebar außer hnr
         PQclear(res);
         db_rollback();
         return -1;
        
    }

}
int db_delete_produkt(const string &pnr){
    PGresult* res;
     string deleteprodukt = "DELETE FROM produkt WHERE pnr = \'" + pnr + "\';" ;
     cout << deleteprodukt<< endl;
    res = PQexec(conn,deleteprodukt.c_str());
    cout << res << endl;
    if (PQresultStatus(res) == PGRES_COMMAND_OK) {
    cout << " Delete  in Tabelle produkt war erfolgreich" << endl;
    PQclear(res);
    return 1;
    }
    else
    {
        cout << PQerrorMessage(conn) << endl; 
         PQclear(res);
         return -1;
        
    }
}
int db_delete_hersteller(const string &hnr){
    PGresult* res;
    string deletehersteller = "DELETE FROM hersteller WHERE hnr = \'" + hnr + "\';" ;
    cout << deletehersteller<< endl;
    res = PQexec(conn,deletehersteller.c_str());
    cout << res << endl;
    if (PQresultStatus(res) == PGRES_COMMAND_OK) {
    cout << " Delete  in Tabelle hersteller war erfolgreich" << endl;
    PQclear(res);
    return 1;
    }
    else
    {
        cout << PQerrorMessage(conn) << endl; 
         PQclear(res);
         return -1;
        
    }
}
int db_count(const string &tablename){
    PGresult* res;
    string count = "SELECT COUNT(*) FROM " + tablename +";";
    cout<< count<< endl;
    res = PQexec(conn,count.c_str());
    if (PQresultStatus(res) == PGRES_TUPLES_OK){
        for ( int i = 0; i < PQntuples(res) ;i++){
            cout <<  "anzahl der Tuples = "<< PQgetvalue(res, i , 0)<< endl;
            
             
            
        }
         PQclear(res);
        return 1;
    }
    else
    {
        cout << PQerrorMessage(conn) << endl; 
         PQclear(res);
         return -1;
        
    }
    

}


 int db_check_hnr(const string &hnr)
 { 
     PGresult* res;
     string checkhnr = "SELECT * FROM hersteller WHERE hnr = \'"+ hnr + "\' ;";
     cout<< checkhnr<< endl;
     res = PQexec(conn, checkhnr.c_str());
    
     if (PQresultStatus(res) == PGRES_TUPLES_OK )
     {
        for ( int i = 0; i < PQntuples(res); i++)
        {

            cout<<"Table Entry "<<i<<endl;
            cout<< i+1<< " "<<PQgetvalue(res,i,0)<< endl;
            cout<< i+1<< " "<<PQgetvalue(res,i,1)<< endl;
            cout<< i+1<< " "<<PQgetvalue(res,i,2)<< endl;
       
            
        }
        if (PQntuples(res)==1)
        {
            PQclear(res);
            return 1;
        }
            
        else {
            PQclear(res);
            return 0;
        }
            
        
        
        
     }
     else
     {
         cout<< "Der Befehl zum hnr checken konnte nicht ausgeführt werden"<< endl;
         cout << PQerrorMessage(conn) << endl;
      
        
         PQclear(res);
         return -1;
 
     }
  
    
}
int db_check_pnr(const string &pnr)
 { 
     PGresult* res;
     string checkpnr = "SELECT * FROM produkt WHERE pnr = \'"+ pnr + "\' ;";
     cout<< checkpnr<< endl;
     res = PQexec(conn, checkpnr.c_str());
    
     if (PQresultStatus(res) == PGRES_TUPLES_OK )
     {
        for ( int i = 0; i < PQntuples(res); i++)
        {

            cout<<"Table Entry "<<i<<endl;
            cout<< i+1<< " "<<PQgetvalue(res,i,0)<< endl;
            cout<< i+1<< " "<<PQgetvalue(res,i,1)<< endl;
            cout<< i+1<< " "<<PQgetvalue(res,i,2)<< endl;
            cout<< i+1<< " "<<PQgetvalue(res,i,3)<< endl;
       
            
        }
             if (PQntuples(res)==1)
        {
            PQclear(res);
            return 1;
        }
            
        else {
            PQclear(res);
            return 0;
        }
            
        
     }
     else
     {
         cout<< "Der Befehl zum pnr checken konnte nicht ausgeführt werden"<< endl;
         cout << PQerrorMessage(conn) << endl;
      
        
         PQclear(res);
         return -1;
 
     }
  
    
}



    
  
