//
//  main.cpp
//  Thinning
//
//  Created by zerniaovo on 4/3/20.
//  Copyright © 2020 zerniaovo. All rights reserved.
//


#include <iostream>
#include <fstream>

using namespace std;
int numRows, numCols, minVal, maxVal;

int changeFlag;
int cycleCount;

int ** firstAry;
int ** secondAry;

class ThinningSkeleton{
public:
    ThinningSkeleton(ifstream&inFile){
        
    }
    void zeroFrame(int ** Ary){
        for(int j =0; j<numCols+2;j++){
            Ary[0][j] = 0;
            Ary[numRows+1][j] = 0;
        }
        for(int i = 0; i<numRows+2;i++){
            Ary[i][0] = 0;
            Ary[i][numCols+1] = 0;
        }
    }
    
    void loadImage(ifstream &inFile, int ** Ary){
        for(int i = 1; i <numRows+1;i++){
            for(int j = 1; j <numCols+1;j++){
                inFile>>Ary[i][j];
            }
        }
    }
    
    void prettyPrint(int ** Ary, ofstream& outFile){
        for(int i = 1; i <numRows+1; i++){
            for(int j = 1; j <numCols+1; j++){
                if(Ary[i][j]>0){
                   // cout<<Ary[i][j]<<" ";
                    outFile<<Ary[i][j]<<" ";
                }
                else{
                   // cout<<"  ";
                    outFile<<"  ";
                }
            }
           // cout<<endl;
            outFile<<endl;
        }
    }
    
    void doThinning(int ** Ary1, int ** Ary2){
        northThinning(Ary1, Ary2);
        copyAry(Ary1,Ary2);
        southThinning(Ary1, Ary2);
        copyAry(Ary1, Ary2);
        westThinning(Ary1, Ary2);
        copyAry(Ary1, Ary2);
        eastThinning(Ary1, Ary2);
        copyAry(Ary1, Ary2);

    }
    void northThinning(int ** Ary1, int ** Ary2){
        for(int i =1; i <numRows+1; i++){
            for(int j = 1; j <numCols+1; j++){
                Ary2[i][j] = Ary1[i][j];
                if(Ary1[i][j]>0&&Ary1[i-1][j]<=0){
                    if(check3n4Conditions(Ary1, i, j)==true){
                        Ary2[i][j] =0;
                        changeFlag ++;
                    }
                }
            }
        }
    }
    
    void southThinning(int **Ary1, int **Ary2){
        for(int i = 1; i<numRows+1;i++){
            for(int j = 1; j <numCols+1; j++){
                Ary2[i][j] = Ary1[i][j];
                if(Ary1[i][j]>0&&Ary1[i+1][j]<=0){
                    if(check3n4Conditions(Ary1, i, j)==true){
                        Ary2[i][j] = 0;{
                            changeFlag++;
                        }
                    }
                }
            }
        }
    }
    void westThinning(int **Ary1, int **Ary2){
        for(int i = 1; i<numRows+1;i++){
            for(int j = 1; j <numCols+1; j++){
                Ary2[i][j] = Ary1[i][j];
                if(Ary1[i][j]>0&&Ary1[i][j-1]<=0){
                    if(check3n4Conditions(Ary1, i, j)==true){
                        Ary2[i][j] = 0;{
                            changeFlag++;
                        }
                    }
                }
            }
        }
    }
    void eastThinning(int **Ary1, int **Ary2){
        for(int i = 1; i<numRows+1;i++){
            for(int j = 1; j <numCols+1; j++){
                Ary2[i][j] = Ary1[i][j];
                if(Ary1[i][j]>0&&Ary1[i][j+1]<=0){
                    if(check3n4Conditions(Ary1, i, j)==true){
                        Ary2[i][j] = 0;{
                            changeFlag++;
                        }
                    }
                }
            }
        }
    }
    
    void copyAry(int ** Ary1, int ** Ary2){
        for(int i = 0; i <numRows+2;i++){
            for(int j = 0; j <numCols+2; j++){
                Ary1[i][j] = Ary2[i][j];
            }
        }
        
    }
    


    bool check3n4Conditions(int **Ary, int i, int j){
          int a = Ary[i-1][j-1];
          int b = Ary[i-1][j];
          int c = Ary[i-1][j+1];
          int d = Ary[i][j-1];
          int e = Ary[i][j+1];
          int f = Ary[i+1][j-1];
          int g = Ary[i+1][j];
          int h = Ary[i+1][j+1];
          int neighborsum= a+b+c+d+e+f+g+h;
          

          if(neighborsum==7){
              return true;
          }
          if(neighborsum==6){
              if(a+c+f+h==4){
                  return false;
              }
              
              
              else{
                  return true;
                  
              }

          }
          if(neighborsum==5){
              if(a+c+f+h==4){
                  return false;
              }
              if(a+c+f+h==3){
                  if((a+b+c==3)&&(c+e+h==3)){
                      return true;
                  }
                  if((c+e+h==3)&&(f+g+h==3)){
                      return true;
                  }
                  if((f+g+h==3)&&(a+d+f==3)){
                      return true;
                  }
                  if((a+d+f==3)&&(a+b+c==3)){
                      return true;
                  }
                  else{
                      return false;
                  }
                  
              }
              
              
          
              
          }
          if(neighborsum==4){
              if(a+c+h+f==4){
                  return false;
              }
              if(a+c+h+f==3){
                  return false;
              }
              if(a+c+h+f==2){
                  
                  if(a+d+f==3){
                      if(b==1){
                          return true;
                      }
                      if(g==1){
                          return true;
                      }
                      if(e==0){
                          return true;
                      }

                  }
                  if(a+b+c==3){
                      if(d==1){
                          return true;
                      }
                      if(e==1){
                          return true;
                      }
              
                  }
                  if(c+e+h==3){
                      if(b==1){
                          return true;
                      }
                      if(g==1){
                          return true;
                      }
                      if(d==0){
                          return true;
                      }

                  }
                  if(f+g+h==3){
                      if(d==1){
                          return true;
                      }
                      if(e==1){
                          return true;
                      }
                  }
                  if((a+b==2)&&(e+h==2)){
                      return true;
                  }
                  if((f+g==2)&&(c+e==2)){
                      return true;
                  }
                  if((d+a==2)&&(g+h==2)){
                      return true;
                  }
                  if((d+f==2)&&(b+c==2)){
                      return true;
                  }



                  
                  else{
                      return false;
                  }
                  
                  
              }
              if(a+c+h+f==1){
                  return true;
              }

              
          }
          if(neighborsum==3){
              if(a+c+h+f==3){
                  return false;
              }



              if(b+e+g==3){
                  return true;
              }
              if(b+d+g==3){
                  return true;
              }
              if(c+e+g==3){
                  return true;
              }
              if(a+d+g==3){
                  return true;
              }
              if(d+f+b==3){
                  return true;
              }
              if(e+h+b==3){
                  return true;
              }
              if(f+g+e==3){
                  return true;
              }
              

              else{
                  return false;
              }

          }
        
        if(neighborsum<3){
            return false;
        }


          return true;
      }
    
        
};

int main(int args, char**argv){
    ifstream inFile;
    ofstream outFile1, outFile2;
    inFile.open(argv[1]);
    //inFile.open("/Users/Niya/Desktop/opencvc++/Thinning/Thinning/Thinning_image2.txt");
    outFile1.open(argv[2]);
    outFile2.open(argv[3]);
    
    inFile>>numRows>>numCols>>minVal>>maxVal;
    
    outFile1<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<endl;
    
    firstAry = new int*[numRows+2];
    secondAry = new int*[numRows+2];
    
    for(int i = 0; i <numRows+2; i++){
        firstAry[i] = new int[numCols+2];
        secondAry[i] = new int[numCols+2];
    }
    
    ThinningSkeleton *ts = new ThinningSkeleton(inFile);
    ts->zeroFrame(firstAry);
    ts->zeroFrame(secondAry);
    
    ts->loadImage(inFile, firstAry);
    
    ts->prettyPrint(firstAry, outFile2);
    changeFlag=1;
    int counter =0;
    while(changeFlag>0){
    changeFlag=0;
        counter++;
   // cout<<"result of thinning: cycle - "<<counter<<endl<<endl;
    outFile2<<"result of thinning: cycle - "<<counter<<endl<<endl;

    ts->doThinning(firstAry, secondAry);
    ts->prettyPrint(firstAry, outFile2);
    }
    

    

    
    for(int i = 1 ; i <numRows+1;i++){
        for(int j = 1; j <numCols+1;j++){
           // cout<<firstAry[i][j]<<" ";
            outFile1<<firstAry[i][j]<<" ";

        }
       // cout<<endl;
        outFile1<<endl;
    }
    
    inFile.close();
    outFile1.close();
    outFile2.close();
    
}
