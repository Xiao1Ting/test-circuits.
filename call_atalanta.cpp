// call_atalanta.cpp : 定义控制台应用程序的入口点。
//This program includes the following functions:
//     1) Call Atalanta commands (test generation and faults simulation)
//     2) Read the patterns file (c17.pat), and write all patterns into two patterns file 
//        (one file "c17.pat_0" contains all patterns in even rows of file c17.pat; another file "c17.pat_1" contains the left ones)
//     3) Run faults simulation based on the above two patterns file, respectively
//     4) Read the obtained mask file, and count how many "0"s in the mask file



#include "stdafx.h"
#include <stdio.h>     
#include <stdlib.h>     
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

int main ()
{
  //call atalanta to run the test generation
  string bench_name = "c17.bench";
  string pat_name = "c17.pat";
  string flt_name = "c17.flt";
  string rep_name = "c17.rep";
  
  string command_g = "atalanta-M -t " +  pat_name + " -F " +  flt_name + " -W 1 " + bench_name;
  cout << "the command is: " << command_g << "\n";
  
  //run the command
  system(command_g.c_str());
  //system ("atalanta-M -t c17.pat -F c17.flt -W 1 c17.bench");


  //read and write pattern file
  for(int i=0;i<2;i++)
  {
	  //opening the existed pattern file.
	 
      ifstream pat(pat_name);

	  //get a name for new pattern file
	  string pat_name_t;
	  ostringstream oss;
	  oss << i;
	  pat_name_t = pat_name + "_" + oss.str();
	  cout << "the name of pattern file is:" << pat_name_t << "\n";
	  
	  //creat a new pattern file
	  ofstream pat_t(pat_name_t);
	  string tv; //used to temporarily store a test vector

	  //start reading and writing
	  if (pat.is_open()) //if the file is open
      {
         int j=0;
		 while (! pat.eof() ) //while the end of file is NOT reached
         {
           getline (pat, tv); //get one line from the file			 
	       //put tvs in even lines to c17.pat.0 and tvs in odd lines to c17.pat.1
		   if(i==0)
		   {
			   if(j%2==0)
				   pat_t << tv <<"\n";
		   }
		   else
		   {
			   if(j%2==1)
				   pat_t << tv <<"\n";
		   }
		   j++;
		 }
		 //closing the file
		 pat.close(); 
         pat_t.close();        
	  }
      else cout << "Unable to open file"; //if the file is not open output

	  
	  //Run simulation for new pattern file and output the report file

	  //get the name for report file
	  string rep_name_t;
	  rep_name_t = rep_name + "_" + oss.str();
	  cout << "the name of report file is:" << rep_name_t << "\n";
	  string command_s = "atalanta-M -S -t " +  pat_name_t + " -P " + rep_name_t + " " + bench_name;
	  cout << "the command is: " << command_s << "\n";

	  //run the simulation
	  system (command_s.c_str());
  }


  // Read a mask file, and count how many "0"s (undetected faults)

  //run command to get the mask file
  string msk_name = "c17.msk";
  string command_s1 = "atalanta-M -S -t " +  pat_name + " -P " + rep_name + " -m " + msk_name + " " + bench_name;
  cout << "the command is: " << command_s1 << "\n";
  system (command_s1.c_str());

  // Read the obtained file
  ifstream msk(msk_name);
  string bs; //binary string
  getline (msk, bs); //for any mask file obtained from atalanta only contain one line
  cout << "the mask file is: " << bs <<endl;

  //start counting
  int num_zeros=0;
  for (int i = 0; i < bs.size(); i++)
  {
	  if (bs[i] == '0')
		  num_zeros += 1;   
  }

  cout << "the number of undetected faults is: "<< num_zeros <<endl;
  return 0;
}


